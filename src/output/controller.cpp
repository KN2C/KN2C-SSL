#include "controller.h"
#include "constants.h"

#define ROBOTRADIUS 0.090
#define SpeedToRPM 1375.14
Controller::Controller(QObject *parent) :
    QObject(parent)
{

    qDebug() << "Controller Initialization...";
    timer.start();
}

ControllerResult Controller::calc(ControllerInput &ci)
{
    ControllerResult ctrlresult;

    ctrlresult.rs = calcRobotSpeed_main(ci);
    //ctrlresult.rs = calcRobotSpeed_adjt(ci);
    //ctrlresult.rs = calcRobotSpeed_test(ci);

    ctrlresult.msR = calcReal(ctrlresult.rs);
    ctrlresult.msS = calcSimul(ctrlresult.rs);
    return ctrlresult;
}

RobotSpeed Controller::calcRobotSpeed_main(ControllerInput &ci)
{
double time = timer.elapsed()/1000;
timer.restart();
    static Vector2D err0,err1;
    static Vector2D u1;

    double ap=5;
    double am=5;

    /******************************Linear Speed Controller************************************/
    Vector2D LinearSpeed;

    err1 = (ci.fin_pos.loc - ci.cur_pos.loc)*.001;
    double dist;

    dist = (pow(ci.fin_vel.loc.length(),2)-pow(u1.length(),2))/(-2.0*am);

    LinearSpeed = err1;
    if(err1.length()<=dist)
    {
        LinearSpeed.setLength(u1.length()-am*.055);
    }
    else if(err1.length()>dist)
    {
        LinearSpeed.setLength(u1.length()+ap*.055);
    }

    if(LinearSpeed.length()>ci.maxSpeed)
    {
        LinearSpeed.setLength(ci.maxSpeed);
    }

    //LinearSpeed = u1 + (LinearSpeed - u1)*0.4;

    u1 = LinearSpeed;

    double kp;
    double ki;
    double kd;
    static Vector2D derived0,derived1;
    static Vector2D integral;
    if(err1.length()<400)
    {
        kp = 3;
        ki = 0;
        kd = .014;
        integral = integral + (err1*0.040);
    }
    else
    {
        kp = 2;
        ki = 0;
        kd = 0;
        integral = {0,0};
    }
    derived1 = (ci.cur_pos.loc*0.001 - err0)/0.040;
    derived0 = derived0 + (derived1 - derived0)*0.1;
    err0 = ci.cur_pos.loc*0.001;
    LinearSpeed = err1*kp + integral*ki - derived0*kd;
    if(LinearSpeed.length()>ci.maxSpeed)
    {
        LinearSpeed.setLength(ci.maxSpeed);
    }

    cout<<LinearSpeed.x<<" "<<err1.x<<" "<<integral.x<<" "<<time<<endl;
    Vector2D RotLinearSpeed=LinearSpeed;
    RotLinearSpeed.x = LinearSpeed.x * cos(ci.cur_pos.dir) + LinearSpeed.y * sin(ci.cur_pos.dir);
    RotLinearSpeed.y = -LinearSpeed.x * sin(ci.cur_pos.dir) + LinearSpeed.y * cos(ci.cur_pos.dir);

    /*************************************Rotation ctrl**********************/
    double wkp,wki,wkd,wu1;
    double MAXROTATIONSPEED = 4,RotationSpeed;
    static double wintegral,werr0,werr1;
    static double wderived0,wderived1;

    werr1 = ci.fin_pos.dir - ci.cur_pos.dir;
    if (werr1 > M_PI) werr1 -= 2 * M_PI;
    if (werr1 < -M_PI) werr1 += 2 * M_PI;
    wkp = 3.5;
    wki = 0;
    wkd = 1.5;
    if(fabs(werr1)*AngleDeg::RAD2DEG<90)
    {
        RotationSpeed = 0;
        wintegral = wintegral + (werr1*0.020);
    }
    else
    {
        RotationSpeed = 0.5*sign(werr1);
//        kp = 3;
//        ki = 0;
//        kd = 0;
        wintegral = 0;
    }
    wderived1 = (ci.cur_pos.dir - werr0)/3;
    wderived0 = wderived0 + (wderived1 - wderived0)*0.1;
    werr0 = ci.cur_pos.dir;

    wu1 = (werr1*wkp) + wintegral*wki - wderived1*wkd;
    //if (wu1>MAXROTATIONSPEED) wu1=MAXROTATIONSPEED;
   // if (wu1<-MAXROTATIONSPEED) wu1=-MAXROTATIONSPEED;
    RotationSpeed = wu1;
    //cout<<wintegral<<" "<<werr1<<" "<<ci.fin_pos.dir<<endl;
   // cout<<RotLinearSpeed.length()<<endl;
//    if(fabs(RotationSpeed)<.5)
//    {
//        RotationSpeed = .5*sign(RotationSpeed);
//    }
//    if(fabs(werr1)*AngleDeg::RAD2DEG<2)
//    {
//        RotationSpeed = 0;
//    }
    RobotSpeed ans;

    ans.VX = RotLinearSpeed.x;
    ans.VY = RotLinearSpeed.y;
    ans.VW = RotationSpeed;

    return ans;
}

RobotSpeed Controller::calcRobotSpeed_adjt(ControllerInput &ci)
{
    float MAXROBOTSPEED = 4;//MAXMOTORSRPM * M_PI * WHEELDIAMETER * cos(M_PI / 6) / 60;
    //float MAXROTATIONSPEED = MAXMOTORSRPM * WHEELDIAMETER / (60 * ROBOTRADIUS * 2);
    //float ROBOTSPEED;
    float RotationSpeed;

    static Vector2D err1,err0;
    static Vector2D u1;

    static double Kp;// = {0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5};
    static double Kd;// = {50,50,50,50,50,50,50,50,50,50,50,50};
    static double delT;// = {10,10,10,10,10,10,10,10,10,10,10,10};
    static double Ki;// = {0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01}; //Kp/Ti

    static Vector2D integral[100];
    //static int Integral_CNT;
    static Vector2D derived;

    static double werr1;// = (ci.fin_pos.loc - ci.cur_pos.loc);
    static double werr0;
    static double wu1;

    static double wKp;// = {0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5};
    static double wKd;// = {50,50,50,50,50,50,50,50,50,50,50,50};
    static double wKi;// = {0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01}; //Kp/Ti
    static double wintegral[10];
    static double wderived;
    static int stateCTRL;

    delT= .020;



    /******************************Linear Speed Controller************************************/

    err0 = err1;
    //err1 = (Vector2D(-500,0) - ci.cur_pos.loc)*.001;
    err1 = (ci.fin_pos.loc - ci.cur_pos.loc)*.001;
    //qDebug()<<"data :"<<ci.fin_vel.loc.length()*1.5;
    if(err1.length()<0.50)
    {/******************************************************************************/
        Kp = 5.0;//3;//_wm->slider[0]/20.0;
        Kd = 0.5;//_wm->slider[1]/20.0;
        Ki = 1;//_wm->slider[2]/20.0;
    }
    else
    {/******************************************************************************/
        Kp = 5.0;//3;//_wm->slider[0]/20.0;
        Kd = 0.5;//_wm->slider[1]/20.0;
        Ki = 1;//_wm->slider[2]/20.0;

        integral[0]=Vector2D(0,0);
    }

    integral[0] = integral[0] + (err1*delT);

    derived = (err1-err0)/delT;

    u1 = (err1*Kp) + (integral[0]*Ki) + derived*Kd;

    if(u1.length()>MAXROBOTSPEED)
        u1.setLength(MAXROBOTSPEED);

    //qDebug()<<"current location"<<ci.cur_pos.loc.x<<" : "<<ci.cur_pos.loc.y;
    Vector2D LinearSpeed = u1;
    Vector2D RotLinearSpeed=LinearSpeed;
    RotLinearSpeed.x = LinearSpeed.x * cos(ci.cur_pos.dir) + LinearSpeed.y * sin(ci.cur_pos.dir);
    RotLinearSpeed.y = -LinearSpeed.x * sin(ci.cur_pos.dir) + LinearSpeed.y * cos(ci.cur_pos.dir);

    /******************************Rotation Speed Controller************************************/
    werr0 = werr1;
    // if(ci.rid==5)
    werr1 = ci.fin_pos.dir - ci.cur_pos.dir;
    // else
    // werr1 = ci.fin_pos.dir - ci.cur_pos.dir;


    if (werr1 > M_PI) werr1 -= 2 * M_PI;
    if (werr1 < -M_PI) werr1 += 2 * M_PI;
    //    if(ci.rid == 0)
    // qDebug()<<"dir: "<< _wm->ball.pos.dir;//<<;werr1;
    if(werr1<.500)
    {/**************************************************************************************/
        wKp = 2.5;
        wKd = .5;
        wKi = 1;
    }
    else
    {/**************************************************************************************/
        wKp = 3.0;
        wKd = .5;
        wKi = 0;
        wintegral[0]=0;
    }
    if(fabs(werr1)< .4)
        wintegral[0] = wintegral[0] + (werr1*delT);
    else wintegral[0] = 0;
    wderived = (fabs(werr1)-fabs(werr0))/delT;

    wu1 = (werr1*wKp) + (wintegral[0]*wKi) + wderived*wKd;



    //    wintegral = wintegral + (werr1*delT);
    //    wu1 = (werr1*wKp) + (wintegral*wKi);
    //    if(fabs(wu1) > MAXROTATIONSPEED*2)
    //        wu1 = sign(wu1)*MAXROTATIONSPEED*2;
    //    if(fabs(werr1)> 5*AngleDeg::DEG2RAD)
    //        wu1 += .001*(float)(sign(wu1));
    RotationSpeed = wu1;
    //stateCTRL=2;
    switch(stateCTRL)
    {
    case 0://jelo
        RotationSpeed = 0;
        werr1 = ((Vector2D(1500,0)-ci.cur_pos.loc).dir().radian()) - ci.cur_pos.dir;
        if (werr1 > M_PI) werr1 -= 2 * M_PI;
        if (werr1 < -M_PI) werr1 += 2 * M_PI;
        RotLinearSpeed = Vector2D(1.5,0);//sorate robot jelo
        if((Vector2D(1500,0) - ci.cur_pos.loc).length()>500 && fabs(werr1)>M_PI/2.0)
        {
            stateCTRL = 3;
        }
        break;
    case 1://aghab
        RotationSpeed = 0;
        werr1 = ((Vector2D(1500,0)-ci.cur_pos.loc).dir().radian()) - ci.cur_pos.dir;
        if (werr1 > M_PI) werr1 -= 2 * M_PI;
        if (werr1 < -M_PI) werr1 += 2 * M_PI;
        RotLinearSpeed = Vector2D(-.3,0);//sorate robot aghab
        if((Vector2D(1500,0) - ci.cur_pos.loc).length()>500&& fabs(werr1)<M_PI/2.0)
        {
            stateCTRL = 2;
        }
        break;
    case 2://charkhesh
        RotLinearSpeed = Vector2D(0,0);
        werr1 = ((Vector2D(1500,0)-ci.cur_pos.loc).dir().radian()) - ci.cur_pos.dir;
        if (werr1 > M_PI) werr1 -= 2 * M_PI;
        if (werr1 < -M_PI) werr1 += 2 * M_PI;

        if(werr1<-(M_PI/18.0))
        {
            RotationSpeed = -1;
        }
        else if(werr1>(M_PI/18.0))
        {
            RotationSpeed = 1;
        }
        else
        {
            RotationSpeed = 0;
            stateCTRL = 0;
        }

        break;
    case 3://charkhesh
        RotLinearSpeed = Vector2D(0,0);
        werr1 = ((Vector2D(1200,0)-ci.cur_pos.loc).dir().radian() + M_PI) - ci.cur_pos.dir;
        if (werr1 > M_PI) werr1 -= 2 * M_PI;
        if (werr1 < -M_PI) werr1 += 2 * M_PI;

        if(werr1<-(M_PI/18.0))
        {
            RotationSpeed = -1;
        }
        else if(werr1>(M_PI/18.0))
        {
            RotationSpeed = 1;
        }
        else
        {
            RotationSpeed = 0;
            stateCTRL = 1;
        }
        break;

    case 4:

        break;
    case 5:

        break;


    }


    RobotSpeed ans;

    ans.VX = RotLinearSpeed.x;
    ans.VY = RotLinearSpeed.y;
    ans.VW = RotationSpeed;

    return ans;
}

RobotSpeed Controller::calcRobotSpeed_test(ControllerInput &ci)
{
    //int time = timer.elapsed();
    //float MAXROBOTSPEED = MAXMOTORSRPM * M_PI * WHEELDIAMETER * cos(M_PI / 6) / 60;
    //float MAXROTATIONSPEED = MAXMOTORSRPM * WHEELDIAMETER / (60 * ROBOTRADIUS * 2);
    //float ROBOTSPEED;
    static float RotationSpeed,Rspeed_filer,Rspeed_filer_1;
    float MAXROTATIONSPEED=4;

    static Vector2D err1,err0;
    static Vector2D u1;


    // static double Kp;// = {0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5};
    //static double Kd;// = {50,50,50,50,50,50,50,50,50,50,50,50};
    static double delT;// = {10,10,10,10,10,10,10,10,10,10,10,10};
    //static double Ki;// = {0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01}; //Kp/Ti

    static double ap=1;
    static double am=1;
    static double am2=1;
    static double vmax = 1;// MAXROBOTSPEED * ci.speed;
    static double t0;
    //static double t1;
    static double s0;
    static double s3;
    static double s1;
    static double v,dt,s,sp,t2,vb;
    static double tp;
    static double t2p;
    static double t3;
    static Position targetVel;
    targetVel.loc = Vector2D(0,0);
    //static Vector2D integral;
    //static int Integral_CNT;
    //static Vector2D derived;

    static double werr1;// = (ci.fin_pos.loc - ci.cur_pos.loc);
    static double werr0;
    static double wu1;

    static double wKp;
    static double wKd;
    static double wKi;
    static double wintegral;
    static double wderived;
    static int stateCTRL;

    delT = .02;



    /******************************Linear Speed Controller************************************/
    Vector2D RotLinearSpeed;
    err0 = err1;
    err1 = (ci.fin_pos.loc - ci.cur_pos.loc)*.001;

    t0 = -ci.cur_vel.loc.length()/ap;
    //t1 = (vmax/ap)+t0;
    s0 = -ci.cur_vel.loc.length()*t0/2;
    s3 = pow(ci.fin_vel.loc.length(),2)/(2*am);
    v = sqrt(s1*2*ap);
    tp = (v/ap)+t0;
    t3 = (v/am) + tp;
    t2p = t3 - (ci.fin_vel.loc.length()/am);
    t2 = t2p;

    if(v>vmax)
    {
        s = err1.length() + s0 + s3;
        sp = s * pow((v-vmax)/v,2);
        dt = sp/vmax;
        t3 = t3 + dt;
        t2p = t2p + dt;
        t2 = t3 - (vmax/am);
    }

    if(t2>0)
    {
        if(ci.cur_vel.loc.length()>vmax + am*delT)
            u1.setLength(ci.cur_vel.loc.length() - am*delT);
        else
            u1.setLength(ci.cur_vel.loc.length() + ap*delT);
    }
    else
    {
        u1.setLength(ci.cur_vel.loc.length() - am*delT);
    }

    Vector2D LinearSpeed = u1;
    RotLinearSpeed=LinearSpeed;
    RotLinearSpeed.x = LinearSpeed.x * cos(ci.cur_pos.dir) + LinearSpeed.y * sin(ci.cur_pos.dir);
    RotLinearSpeed.y = -LinearSpeed.x * sin(ci.cur_pos.dir) + LinearSpeed.y * cos(ci.cur_pos.dir);

    /******************************Rotation Speed Controller************************************/
    werr0 = werr1;
    werr1 = ci.fin_pos.dir - ci.cur_pos.dir;
    if (werr1 > M_PI) werr1 -= 2 * M_PI;
    if (werr1 < -M_PI) werr1 += 2 * M_PI;

    if(fabs(werr1)< .4)
        wintegral = wintegral + (werr1*delT);
    else wintegral = 0;
    wderived = (fabs(werr1)-fabs(werr0))/delT;

    ci.fin_pos.dir=0;

    switch(stateCTRL)
    {
    case 0://jelo
        ci.fin_pos.loc = Vector2D(500,1000);

        //RotationSpeed = 0;

        if (werr1 > M_PI) werr1 -= 2 * M_PI;
        if (werr1 < -M_PI) werr1 += 2 * M_PI;

        //if(ci.cur_pos.loc.x>0)
        //{

        err1 = (ci.fin_pos.loc - ci.cur_pos.loc)*.001;
        if(err1.length()<.30)
        {
            ci.fin_pos.dir = ((Vector2D(2200,0)-ci.cur_pos.loc).dir().radian()) ;

        }
        else ci.fin_pos.dir = ((ci.fin_pos.loc-ci.cur_pos.loc).dir().radian()) ;
        targetVel.loc = Vector2D(1,1);
        targetVel.loc.setLength(0);
        if(err1.length()<.005)
        {
            stateCTRL = 1;
        }
        break;
    case 1://aghab
        RotationSpeed = 0;
        ci.fin_pos.loc = Vector2D(500,-1000);
        werr1 = ((Vector2D(1500,0)-ci.cur_pos.loc).dir().radian()) - ci.cur_pos.dir;
        if (werr1 > M_PI) werr1 -= 2 * M_PI;
        if (werr1 < -M_PI) werr1 += 2 * M_PI;
        err1 = (ci.fin_pos.loc - ci.cur_pos.loc)*.001;

        if(err1.length()<.30)
        {
            ci.fin_pos.dir = ((Vector2D(500,0)-ci.cur_pos.loc).dir().radian()) ;
        }
        else ci.fin_pos.dir = ((ci.fin_pos.loc-ci.cur_pos.loc).dir().radian()) ;
        targetVel.loc = Vector2D(1,1);
        targetVel.loc.setLength(0);
        if(err1.length()<.010)
        {
            stateCTRL = 2;
        }
        break;
    case 2://charkhesh
        RotationSpeed = 0;
        werr1 = ((Vector2D(2500,-1000)-ci.cur_pos.loc).dir().radian()) - ci.cur_pos.dir;
        if (werr1 > M_PI) werr1 -= 2 * M_PI;
        if (werr1 < -M_PI) werr1 += 2 * M_PI;
        ci.fin_pos.loc = Vector2D(2500,-1000);
        //ci.fin_pos.dir = ((ci.fin_pos.loc-ci.cur_pos.loc).dir().radian()) ;
        err1 = (ci.fin_pos.loc - ci.cur_pos.loc)*.001;
        if(err1.length()<.30)
        {
            ci.fin_pos.dir = ((Vector2D(2200,-1500)-ci.cur_pos.loc).dir().radian()) ;
        }
        else ci.fin_pos.dir = ((ci.fin_pos.loc-ci.cur_pos.loc).dir().radian()) ;

        targetVel.loc = Vector2D(1,1);
        targetVel.loc.setLength(0);
        if(err1.length()<.010)
        {
            stateCTRL = 3;
        }
        break;
    case 3://charkhesh
        RotLinearSpeed = Vector2D(0,0);
        werr1 = ((Vector2D(2500,500)-ci.cur_pos.loc).dir().radian() + M_PI) - ci.cur_pos.dir;
        if (werr1 > M_PI) werr1 -= 2 * M_PI;
        if (werr1 < -M_PI) werr1 += 2 * M_PI;
        ci.fin_pos.loc = Vector2D(2500,1000);
        //ci.fin_pos.dir = ((ci.fin_pos.loc-ci.cur_pos.loc).dir().radian()) ;
        err1 = (ci.fin_pos.loc - ci.cur_pos.loc)*.001;
        if(err1.length()<.30)
        {
            ci.fin_pos.dir = ((Vector2D(2200,-1500)-ci.cur_pos.loc).dir().radian()) ;
        }
        else ci.fin_pos.dir = ((ci.fin_pos.loc-ci.cur_pos.loc).dir().radian()) ;

        targetVel.loc = Vector2D(1,1);
        targetVel.loc.setLength(0);
        if(err1.length()<.010)
        {
            stateCTRL = 0;
        }

        break;

        ci.fin_pos.dir=0;
        wKp = 3.0;
        wKd = 0;
        wKi = 0;
        werr0 = werr1;
        werr1 = ci.fin_pos.dir - ci.cur_pos.dir;
        if (werr1 > M_PI) werr1 -= 2 * M_PI;
        if (werr1 < -M_PI) werr1 += 2 * M_PI;
        //werr1=werr0+0.2*(werr1-werr0);

        Rspeed_filer_1=Rspeed_filer;
        wu1 = (werr1*wKp) + (wintegral*wKi) + wderived*wKd;
        Rspeed_filer = wu1;

        Rspeed_filer=Rspeed_filer_1+(0.2*(Rspeed_filer-Rspeed_filer_1));

        if (Rspeed_filer>MAXROTATIONSPEED) Rspeed_filer=MAXROTATIONSPEED;
        if (Rspeed_filer<-MAXROTATIONSPEED) Rspeed_filer=-MAXROTATIONSPEED;
        RotationSpeed = Rspeed_filer ;

        //qDebug()<<ci.cur_pos.loc.x<<ci.cur_pos.loc.y<<ci.fin_pos.loc.x<<ci.fin_pos.loc.y;
        ///////////////////////////////////////////INJA
        static double dist;
        vb=vmax/2.0;
        if (ci.cur_vel.loc.length()<vb)
        {
            dist = (pow(targetVel.loc.length(),2)-pow(ci.cur_vel.loc.length(),2))/(-2.0*am2);
        }
        else
        {

            dist =(pow(vb,2)-pow(ci.cur_vel.loc.length(),2))/(-2.0*am);
            dist+=(pow(targetVel.loc.length(),2)-pow(vb,2))/(-2.0*am2);


        }
        // if(ci.cur_vel.loc.x > 0)
        // dist = (pow(targetVel.loc.length(),2)-pow(ci.cur_vel.loc.length(),2))/(-2.0*am);
        //dist2 = (pow(targetVel.loc.length(),2)-pow((vmax/3.0),2))/(-2.0*am2);
        // else
        //     dist = (pow(targetVel.loc.x,2)-pow(ci.cur_vel.loc.x,2))/(2.0*am);
        u1 = err1;
        // qDebug()<<"dist: "<<dist <<"err: "<< err1.length()<<"tvel: "<<targetVel.loc.length();
        //qDebug()<<ci.cur_pos.loc.x<<ci.cur_pos.loc.y;
        if(err1.length()<=dist)
        {

            // u1 .setLength( ci.cur_vel.loc.length() - am*delT);
            //u1.setLength(sqrt(pow(ci.cur_vel.loc.length(),2)-2.0*am*((err1-err0).length()*.001)));

            if(ci.cur_vel.loc.length()<vb)
                u1.setLength(sqrt(2.0*am2*(err1.length())+pow(targetVel.loc.length(),2)));
            else

                u1.setLength(sqrt(2.0*am*(err1.length())+pow(targetVel.loc.length(),2)));

        }
        else if(err1.length()>dist)
        {

            //            if(ci.cur_vel.loc.x > 0)
            //            {
            // t0 = -ci.cur_vel.loc.length()/ap;
            // t1 = (vmax/ap)+t0;

            //double Sm = (pow(vmax,2)-(pow(ci.cur_vel.loc.length(),2))/(2.0*ap));
            // u1.setLength(vmax - (2.0*(vmax*t1-Sm)/t1));
            //u1.setLength( ci.cur_vel.loc.length() + ap*delT);
            t0 = -ci.cur_vel.loc.length()/ap;
            //t1 = (vmax/ap)+t0;
            s0 = -ci.cur_vel.loc.length()*t0/2;
            s3 = pow(targetVel.loc.length(),2)/(2*am);
            s1 = (err1.length()+s0+s3)/(1+ap/am);
            v = sqrt(s1*2*ap);
            tp = (v/ap)+t0;
            t3 = (v/am) + tp;
            t2p = t3 - (targetVel.loc.length()/am);
            t2 = t2p;
            double Sm = (pow(v,2)-pow(targetVel.loc.length(),2))/(2.0*am);
            u1.setLength(sqrt(2.0*ap*((err1.length()-Sm))+pow(v,2)));


            //u1.setLength((2.0*(err1.length()-Sm)/t1)-vmax);
            // u1.setLength(err1.length());
            // u1.setLength(sqrt(pow(ci.cur_vel.loc.length(),2)+2.0*ap*(((err1-err0).length()+1)*.001)));
            //            }
            //            else
            //                u1.x = ci.cur_vel.loc.x - ap*delT;

            //u1.setLength(sqrt(pow(ci.cur_vel.loc.length(),2)+2.0*ap*((err1-err0).length()*.001)));

        }
        //        if(err1.length() < .010)
        //            u1.setLength(0);

        if(u1.length()>vmax)
        {
            u1.setLength(vmax);
        }
        //        else if(u1.x<-vmax)
        //        {
        //            u1.x=-vmax;
        //        }


        //  u1.y=0;
        //////////////////////////////////////////ta INJA

        LinearSpeed = u1;
        // LinearSpeed.setLength(1);//
        //LinearSpeed.setLength(1.5);
        RotLinearSpeed.x = LinearSpeed.x * cos(ci.cur_pos.dir) + LinearSpeed.y * sin(ci.cur_pos.dir);
        RotLinearSpeed.y = -LinearSpeed.x * sin(ci.cur_pos.dir) + LinearSpeed.y * cos(ci.cur_pos.dir);




    }



    RobotSpeed result;

    result.VX = RotLinearSpeed.x;
    result.VY = RotLinearSpeed.y;
    result.VW = RotationSpeed;

    return result;
}

MotorSpeed Controller::calcReal(RobotSpeed rs)
{
    double motor[4][1],rotate[4][3],speed[3][1];

    speed[0][0] = -rs.VX;
    speed[1][0] = -rs.VY;
    speed[2][0] = -rs.VW;

    rotate[0][0] =  cos( 0.18716 * M_PI);//cos(M_PI /4.0);//-sin(rangle + M_PI);//7/4
    rotate[1][0] =  sin( M_PI / 4.0 );//-cos(0.22 * M_PI);//-sin(rangle - M_PI / 3);//0.218
    rotate[2][0] =  -cos( M_PI / 4.0 );//-sin(0.22 * M_PI);//-sin(rangle + M_PI / 3);//0.78
    rotate[3][0] =  -cos( 0.18716 * M_PI);//cos(M_PI /4.0);//-sin(rangle + M_PI);//5/4
    rotate[0][1] =  -sin(0.18716 * M_PI );//cos(M_PI /4.0);//cos(rangle + M_PI);//7/4
    rotate[1][1] = cos(M_PI / 4.0 );//- sin(0.22 * M_PI);// cos(rangle - M_PI / 3);//0.218
    rotate[2][1] = sin(M_PI / 4.0);//cos(0.22 * M_PI);//cos(rangle + M_PI / 3);//0.187
    rotate[3][1] = -sin(0.18716 * M_PI);//-cos(M_PI /4.0);//cos(rangle + M_PI);//5/4

    rotate[0][2] = -ROBOTRADIUS;
    rotate[1][2] = -ROBOTRADIUS;
    rotate[2][2] = -ROBOTRADIUS;
    rotate[3][2] = -ROBOTRADIUS;

    motor[0][0] = (rotate[0][0] * speed[0][0] + rotate[0][1] * speed[1][0] + rotate[0][2] * speed[2][0])*SpeedToRPM;
    motor[1][0] = (rotate[1][0] * speed[0][0] + rotate[1][1] * speed[1][0] + rotate[1][2] * speed[2][0])*SpeedToRPM;
    motor[2][0] = (rotate[2][0] * speed[0][0] + rotate[2][1] * speed[1][0] + rotate[2][2] * speed[2][0])*SpeedToRPM;
    motor[3][0] = (rotate[3][0] * speed[0][0] + rotate[3][1] * speed[1][0] + rotate[3][2] * speed[2][0])*SpeedToRPM;

    //double MaxMotorSpeed = 3.47;//MAXMOTORSRPM * M_PI * WHEELDIAMETER / 60000;

    MotorSpeed result;

    result.M0 = (motor[0][0]);
    result.M1 = (motor[1][0]);
    result.M2 = (motor[2][0]);
    result.M3 = (motor[3][0]);

    //    double max = max4(fabs(result.M0),fabs(result.M1),fabs(result.M2),fabs(result.M3));

    //    if(max > 127)
    //    {
    //        result.M0 = (int)((result.M0 * 127.0)/max);
    //        result.M1 = (int)((result.M1 * 127.0)/max);
    //        result.M2 = (int)((result.M2 * 127.0)/max);
    //        result.M3 = (int)((result.M3 * 127.0)/max);
    //    }

    return result;
}

MotorSpeed Controller::calcSimul(RobotSpeed rs)
{
    double motor[4][1],rotate[4][3],speed[3][1];

    speed[0][0] = rs.VX;
    speed[1][0] = rs.VY;
    speed[2][0] = rs.VW;

    rotate[0][0] = sin(M_PI / 3);//-sin(rangle - M_PI / 3);
    rotate[1][0] = sin(3 * M_PI / 4);//-sin(rangle + M_PI / 3);
    rotate[2][0] = sin(5 * M_PI / 4);//-sin(rangle + M_PI);
    rotate[3][0] = sin(5 * M_PI / 3);//-sin(rangle + M_PI);
    rotate[0][1] = -cos(M_PI / 3);//cos(rangle - M_PI / 3);
    rotate[1][1] = -cos(3 * M_PI / 4);//cos(rangle + M_PI / 3);
    rotate[2][1] = -cos(5 * M_PI / 4);//cos(rangle + M_PI);
    rotate[3][1] = -cos(5 * M_PI / 3);//cos(rangle + M_PI);
    rotate[0][2] = -ROBOTRADIUS;
    rotate[1][2] = -ROBOTRADIUS;
    rotate[2][2] = -ROBOTRADIUS;
    rotate[3][2] = -ROBOTRADIUS;

    motor[0][0] = rotate[0][0] * speed[0][0] + rotate[0][1] * speed[1][0] + rotate[0][2] * speed[2][0];
    motor[1][0] = rotate[1][0] * speed[0][0] + rotate[1][1] * speed[1][0] + rotate[1][2] * speed[2][0];
    motor[2][0] = rotate[2][0] * speed[0][0] + rotate[2][1] * speed[1][0] + rotate[2][2] * speed[2][0];
    motor[3][0] = rotate[3][0] * speed[0][0] + rotate[3][1] * speed[1][0] + rotate[3][2] * speed[2][0];


    MotorSpeed result;

    result.M0 = -motor[0][0]*100;
    result.M1 = -motor[1][0]*100;
    result.M2 = -motor[2][0]*100;
    result.M3 = -motor[3][0]*100;

    return result;
}
