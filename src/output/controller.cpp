#include "controller.h"
#include "constants.h"

#define ROBOTRADIUS 0.090
#define SpeedToRPM 1375.14
Controller::Controller(QObject *parent) :
    QObject(parent)
    ,out("/home/kn2c/Desktop/Untitled Folder/Data.txt")
{

    qDebug() << "Controller Initialization...";

    timer.start();

    err0 = {0,0};
    err1 = {0,0};
    u1 = {0,0};
    derived0 = {0,0};
    derived1 = {0,0};
    integral = {0,0};

    wu1 = 0;
    wintegral = 0;
    werr0 = 0;
    werr1 = 0;
    wderived0 = 0;
    wderived1 = 0;

    stateCTRL = 0;
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
    //double time = timer.elapsed()/1000;
    //timer.restart();

    double ap=4;
    double am=4;

    /******************************Linear Speed Controller************************************/
    Vector2D LinearSpeed;
    //ci.mid_pos.loc = {0,0};
    //ci.maxSpeed = 2;
    err1 = (ci.mid_pos.loc - ci.cur_pos.loc)*.001;
    err1.setLength((ci.fin_pos.loc - ci.cur_pos.loc).length()*.001);
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

    if(err1.length()<.45)
    {
        kp = 2.5;//2.2;//1.5;
        ki = 0.15;
        kd = 0.1;//0.14;
        integral = integral + (err1*(AI_TIMER/1000.0));
    }
    else
    {
        kp = 2.5;//2.5;
        ki = 0.15;//0.05;
        kd = 0.2;//0;
        integral = {0,0};
    }
    derived1 = (ci.cur_pos.loc*0.001 - err0)/(AI_TIMER/1000.0);
    derived0 = derived0 + (derived1 - derived0)*0.1;
    err0 = ci.cur_pos.loc*0.001;
    LinearSpeed = err1*kp + integral*ki - derived0*kd;
    //    }
    //    else
    //        integral = {0,0};
    if(LinearSpeed.length()>ci.maxSpeed)
    {
        LinearSpeed.setLength(ci.maxSpeed);
    }

    //cout<<LinearSpeed.x<<" "<<err1.x<<" "<<integral.x<<" "<<time<<endl;
    Vector2D RotLinearSpeed=LinearSpeed;
    RotLinearSpeed.x = LinearSpeed.x * cos(ci.cur_pos.dir) + LinearSpeed.y * sin(ci.cur_pos.dir);
    RotLinearSpeed.y = -LinearSpeed.x * sin(ci.cur_pos.dir) + LinearSpeed.y * cos(ci.cur_pos.dir);

    /*************************************Rotation ctrl**********************/
    double wkp,wki,wkd,wu1;
    double MAXROTATIONSPEED = 2.5,RotationSpeed;

    werr1 = ci.mid_pos.dir - ci.cur_pos.dir;
    if (werr1 > M_PI) werr1 -= 2 * M_PI;
    if (werr1 < -M_PI) werr1 += 2 * M_PI;
    if(err1.length()<.4)
    {
        if(fabs(werr1)*AngleDeg::RAD2DEG<30)
        {
            wkp = 3;
            wki = 0;
            wkd = 0.3;
            RotationSpeed = 0;
            wintegral = wintegral + (werr1*0.020);
        }
        else
        {
            wkp = 2;
            wki = 0;
            wkd = 0;
            RotationSpeed = 0.5*sign(werr1);
            //        kp = 3;
            //        ki = 0;
            //        kd = 0;
            wintegral = 0;
        }
    }
    else
    {

        wkp = 0;
        wki = 0;
        wkd = 0;
    }

    wderived1 = (ci.cur_pos.dir - werr0)/3;
    wderived0 = wderived0 + (wderived1 - wderived0)*0.1;
    werr0 = ci.cur_pos.dir;

    wu1 = (werr1*wkp) + wintegral*wki - wderived1*wkd;
    if (wu1>MAXROTATIONSPEED) wu1=MAXROTATIONSPEED;
    if (wu1<-MAXROTATIONSPEED) wu1=-MAXROTATIONSPEED;
    RotationSpeed = wu1;

    out << ci.cur_pos.loc.x <<" "<< ci.cur_pos.loc.y <<" "<< 0 << endl;

    //cout<<wintegral<<" "<<werr1<<" "<<ci.mid_pos.dir<<endl;
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
    float RotationSpeed;

    RotationSpeed = wu1;
    Vector2D RotLinearSpeed;
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
    }


    RobotSpeed ans;

    ans.VX = RotLinearSpeed.x;
    ans.VY = RotLinearSpeed.y;
    ans.VW = RotationSpeed;

    return ans;
}

RobotSpeed Controller::calcRobotSpeed_test(ControllerInput &ci)
{

    float RotationSpeed;

    double ap=1;
    double am=1;
    double am2=1;
    double t0;
    //double t1,t2;
    double s0;
    double s3;
    double s1;
    double v,dt,s,sp,vb;
    double tp;
    double t2p;
    double t3;
    /******************************Linear Speed Controller************************************/
    Vector2D RotLinearSpeed;
    err0 = err1;
    err1 = (ci.fin_pos.loc - ci.cur_pos.loc)*.001;

    t0 = -ci.cur_vel.loc.length()/ap;
    //t1 = (ci.maxSpeed/ap)+t0;
    s0 = -ci.cur_vel.loc.length()*t0/2;
    s3 = pow(ci.fin_vel.loc.length(),2)/(2*am);
    v = 0;//sqrt(s1*2*ap);
    tp = (v/ap)+t0;
    t3 = (v/am) + tp;
    t2p = t3 - (ci.fin_vel.loc.length()/am);
    //t2 = t2p;

    if(v>ci.maxSpeed)
    {
        s = err1.length() + s0 + s3;
        sp = s * pow((v-ci.maxSpeed)/v,2);
        dt = sp/ci.maxSpeed;
        t3 = t3 + dt;
        t2p = t2p + dt;
        //t2 = t3 - (ci.maxSpeed/am);
    }

    double dist;
    vb=ci.maxSpeed/2.0;
    if (ci.cur_vel.loc.length()<vb)
    {
        dist = (pow(ci.fin_vel.loc.length(),2)-pow(ci.cur_vel.loc.length(),2))/(-2.0*am2);
    }
    else
    {

        dist =(pow(vb,2)-pow(ci.cur_vel.loc.length(),2))/(-2.0*am);
        dist+=(pow(ci.fin_vel.loc.length(),2)-pow(vb,2))/(-2.0*am2);


    }

    u1 = err1;

    if(err1.length()<=dist)
    {
        if(ci.cur_vel.loc.length()<vb)
            u1.setLength(sqrt(2.0*am2*(err1.length())+pow(ci.fin_vel.loc.length(),2)));
        else

            u1.setLength(sqrt(2.0*am*(err1.length())+pow(ci.fin_vel.loc.length(),2)));
    }
    else if(err1.length()>dist)
    {
        t0 = -ci.cur_vel.loc.length()/ap;

        s0 = -ci.cur_vel.loc.length()*t0/2;
        s3 = pow(ci.fin_vel.loc.length(),2)/(2*am);
        s1 = (err1.length()+s0+s3)/(1+ap/am);
        v = sqrt(s1*2*ap);
        tp = (v/ap)+t0;
        t3 = (v/am) + tp;
        t2p = t3 - (ci.fin_vel.loc.length()/am);
        //t2 = t2p;
        double Sm = (pow(v,2)-pow(ci.fin_vel.loc.length(),2))/(2.0*am);
        u1.setLength(sqrt(2.0*ap*((err1.length()-Sm))+pow(v,2)));

    }

    if(u1.length()>ci.maxSpeed)
    {
        u1.setLength(ci.maxSpeed);
    }


    Vector2D LinearSpeed = u1;
    RotLinearSpeed.x = LinearSpeed.x * cos(ci.cur_pos.dir) + LinearSpeed.y * sin(ci.cur_pos.dir);
    RotLinearSpeed.y = -LinearSpeed.x * sin(ci.cur_pos.dir) + LinearSpeed.y * cos(ci.cur_pos.dir);
    RotationSpeed=0;
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
