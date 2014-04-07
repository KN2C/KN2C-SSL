#include "tacticformation.h"

TacticFormation::TacticFormation(WorldModel *worldmodel, QObject *parent) :
    Tactic("TacticFormation", worldmodel, parent)
{
}

RobotCommand TacticFormation::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;
    return rc;
}
