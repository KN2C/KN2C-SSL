#include "tacticattacker.h"

TacticAttacker::TacticAttacker(WorldModel *worldmodel, QObject *parent) :
    Tactic(worldmodel, parent)
{
}

RobotCommand TacticAttacker::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;
    return rc;
}
