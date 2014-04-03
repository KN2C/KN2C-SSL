#include "tactictest2.h"

TacticTest2::TacticTest2(WorldModel *worldmodel, QObject *parent) :
    Tactic(worldmodel, parent)
{
    a = 0;
}

RobotCommand TacticTest2::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;

//    rc.maxSpeed = 2;
    double maxspeed = 2;
    double Vm = maxspeed*0.9;//wm->ourRobot[id].vel.loc.length();
    if (Vm<0.5)
            Vm=0.5;
    double Vb = wm->ball.vel.loc.length();
    double k = Vm/Vb;
    double gamaT = wm->ball.vel.loc.dir().radian();
    //qDebug()<<"ball dir = " << gamaT*(180/3.1416);
    Vector2D delta;

    Vector2D kick_pos =  (wm->ball.pos.loc - Vector2D(3025,0));
    kick_pos.setLength(ROBOT_RADIUS);
    kick_pos = wm->ball.pos.loc + kick_pos;

    delta = (kick_pos - wm->ourRobot[id].pos.loc);
    double landa = atan2(delta.y , delta.x );
    //qDebug()<<"asin0.5=" << asin(0.5);//landa = " << landa*(180/3.1416);
    double theta = gamaT - landa;
    if (theta > M_PI) theta -= 2 * M_PI;
    if (theta < -M_PI) theta += 2 * M_PI;
    double dlta=0;
    if(fabs(sin(theta)/k) < 1)
    {
        dlta=asin(sin(theta)/k);
        //qDebug() << "&&&&&&&&&& hoshshshsha !!! &&&&&&&&&&";
    }
    //else
        //qDebug() << "-------------------------------------";
        double gamaM = dlta + landa;
//    Ray2D rline = Ray2D(wm->ourRobot[id].pos.loc,gamaM);
//    Ray2D bline = Ray2D(wm->ball.pos.loc,gamaT);
//    Vector2D catchpoint = rline.intersection(bline);
    double tf = 1.2*(delta.length()/1000)/(Vm*cos(dlta) - Vb*cos(theta));
    if(tf<0)
        tf=0;
    double catchDist = Vb*tf*1000;//Vm*tf*1000;
    // //Vector2D catchDiff=Vector2D(catchDist*cos(gamaM),catchDist*sin(gamaM));
    Vector2D catchDiff=Vector2D(catchDist*cos(gamaT),catchDist*sin(gamaT));
    //catchDist.setLength(Vm*tf*1000) ;
    //catchDist.setDir(gamaM*(180/3.1416));
    // //Vector2D catchpoint = wm->ourRobot[id].pos.loc + catchDiff;
    Vector2D catchpoint = kick_pos + catchDiff;
    if(fabs(catchpoint.x)>FIELD_MAX_X | fabs(catchpoint.y)>FIELD_MAX_Y )
    {
        catchpoint = kick_pos;
        qDebug()<<"!!!!!!!!!!!!!!!  bil bil !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    }
    qDebug()<< catchpoint.x << catchpoint.y << "----" << Vb << "dlta:" << dlta <<", " << tf;
    //qDebug() << catchDist << "..." << Vm*tf;
    //qDebug()<< catchpoint.x << catchpoint.y << "----" << Vb << "dlta:" << dlta <<", " << tf;
    rc.fin_pos.loc = catchpoint;
    rc.maxSpeed=maxspeed;
//    rc.
    rc.useNav = false;
    //    //rc.fin_pos.dir = 0;
    //Vector2D kick_pos =  (wm->ball.pos.loc - Vector2D(-3025,0));
    //kick_pos.setLength(ROBOT_RADIUS);
    //kick_pos = wm->ball.pos.loc + kick_pos;

    //Vector2D tmp = wm->ball.pos.loc - kick_pos;
    //Vector2D tmp2 = wm->ball.pos.loc - wm->ourRobot[id].pos.loc ;
    //rc.fin_pos.loc = wm->ball.pos.loc - (tmp + tmp2)/2;

    //rc.fin_pos.dir = (Vector2D(-3025,0) - wm->ourRobot[id].pos.loc  ).dir().radian();
    //    switch (a)
    //    {
    //    case 0:
    //        rc.fin_pos.loc = {0,1000};
    //        //rc.fin_pos.dir = M_PI;
    //        break;
    //    case 1:
    //        rc.fin_pos.loc = {1000,1000};
    //       // rc.fin_pos.dir = 0;
    //        //rc.fin_pos.loc = kick_pos;
    //        //rc.fin_pos.dir = (Vector2D(3025,0) - wm->ourRobot[id].pos.loc  ).dir().radian();
    //        break;
    //    case 2:
    //        rc.fin_pos.loc = {1000,1000};
    //        break;
    //    case 3:
    //        rc.fin_pos.loc = {1000,-1000};
    //        break;
    //    }

    //double werr1 = (wm->ball.pos.loc - wm->ourRobot[id].pos.loc).dir().radian()
    //                    - wm->ourRobot[id].pos.dir;
    //if (werr1 > M_PI) werr1 -= 2 * M_PI;
    //if (werr1 < -M_PI) werr1 += 2 * M_PI;

    /*if((wm->ourRobot[id].pos.loc-wm->ball.pos.loc).length()
            <ROBOT_RADIUS+BALL_RADIUS && fabs(werr1*AngleDeg::RAD2DEG)<7)
    {
        qDebug()<<"test:KIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIICK";
        rc.kickspeedx=5;
    }
    else
    {
        //qDebug()<<"---";
    }*/
    //qDebug()<<werr1*AngleDeg::RAD2DEG;

    //    cout <<a<<" "<< (wm->ourRobot[id].pos.dir - rc.fin_pos.dir)*AngleDeg::RAD2DEG<<endl;
    //    if(fabs((wm->ourRobot[id].pos.dir - rc.fin_pos.dir)*AngleDeg::RAD2DEG)<20) a=1;
    //    if((wm->ourRobot[id].pos.loc - rc.fin_pos.loc).length() < 10) a = (a+1) % 2;

    return rc;
}
