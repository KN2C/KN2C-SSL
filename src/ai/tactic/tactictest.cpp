#include "tactictest.h"

TacticTest::TacticTest(WorldModel *worldmodel, QObject *parent) :
    Tactic("TacticTest", worldmodel, parent)
{
    a = 0;
    b = 0;
}

RobotCommand TacticTest::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;
    param = QString::number(a);

    rc.maxSpeed = 1.5;
    switch (a)
    {
    case 0:
        rc.fin_pos.loc = {700,700};
        break;
    case 1:
        rc.fin_pos.loc = {700,-700};
        break;
//    case 2:
//        rc.fin_pos.loc = {700,-700};
//        break;
//    case 3:
//        rc.fin_pos.loc = {-700,-700};
//        break;
//    case 4:
//        rc.fin_pos.loc = {-700,700};
//        break;
    }

    if(wm->kn->ReachedToPos(wm->ourRobot[id].pos, rc.fin_pos, 20, 180))
    {
        a = (a+1) % 2;
        b++;
        if(b>6) a=0;
    }

    rc.useNav = false;
    rc.isBallObs = true;
    rc.isKickObs = true;
    return rc;
}
