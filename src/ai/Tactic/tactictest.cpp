#include "tactictest.h"

TacticTest::TacticTest(WorldModel *worldmodel, QObject *parent) :
    Tactic(worldmodel, parent)
{
    a = 0;
}

RobotCommand TacticTest::getCommand()
{
    RobotCommand rc;
    rc.maxSpeed = 1;
rc.fin_pos.dir = M_PI/2;
    switch (a)
    {
    case 0:
        rc.fin_pos.loc = {0,-1000};
        //rc.fin_pos.dir = M_PI;
        break;
    case 1:
        rc.fin_pos.loc = {0,1000};
       // rc.fin_pos.dir = 0;
        break;
    case 2:
        rc.fin_pos.loc = {1000,1000};
        break;
    case 3:
        rc.fin_pos.loc = {1000,-1000};
        break;
    }
//cout <<a<<" "<< (wm->ourRobot[id].pos.dir - rc.fin_pos.dir)*AngleDeg::RAD2DEG<<endl;
    //if(fabs((wm->ourRobot[id].pos.dir - rc.fin_pos.dir)*AngleDeg::RAD2DEG)<20) a=1;
    if((wm->ourRobot[id].pos.loc - rc.fin_pos.loc).length() < 10) a = 1;//(a+1) % 2;

    return rc;
}
