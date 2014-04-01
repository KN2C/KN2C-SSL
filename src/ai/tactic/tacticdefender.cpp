#include "tacticdefender.h"

TacticDefender::TacticDefender(WorldModel *worldmodel, QObject *parent) :
    Tactic(worldmodel, parent)
{
}

void TacticDefender::setDefenderID(int total, int current)
{
    def_t = total;
    def_c = current;
}

RobotCommand TacticDefender::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;









    rc.useNav = false;
    rc.isBallObs = false;
    rc.isKickObs = false;

    return rc;
}
