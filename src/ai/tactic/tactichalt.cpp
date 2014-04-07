#include "tactichalt.h"

TacticHalt::TacticHalt(WorldModel *worldmodel, QObject *parent) :
    Tactic("TacticHalt", worldmodel, parent)
{
}

RobotCommand TacticHalt::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;
    return rc;
}
