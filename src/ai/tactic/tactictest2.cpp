#include "tactictest2.h"

TacticTest2::TacticTest2(WorldModel *worldmodel, QObject *parent) :
    Tactic(worldmodel, parent)
{
}

RobotCommand TacticTest2::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;

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

    //double gamaM = dlta + landa;
    double tf = 1.2*(delta.length()/1000)/(Vm*cos(dlta) - Vb*cos(theta));
    if(tf<0)
        tf=0;
    double catchDist = Vb*tf*1000; //Vm*tf*1000;
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
    rc.maxSpeed = maxspeed;
    rc.useNav = false;
    return rc;
}
