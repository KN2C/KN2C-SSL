#include "tactictest.h"

TacticTest::TacticTest(WorldModel *worldmodel, QObject *parent) :
    Tactic(worldmodel, parent)
{
    a = 0;
}

RobotCommand TacticTest::getCommand()
{
    RobotCommand rc;
    //qDebug() << a;
    switch (a)
    {
    case 0:
        rc.fin_pos.loc = {0,-1000};
        break;
    case 1:
        rc.fin_pos.loc = {00,1000};
        break;
    case 2:
        rc.fin_pos.loc = {2000,2000};
        break;
    case 3:
        rc.fin_pos.loc = {2000,-2000};
        break;
    }

    if((wm->ourRobot[id].pos.loc - rc.fin_pos.loc).length2() < 1000) a = (a+1) % 2;

    return rc;
}
