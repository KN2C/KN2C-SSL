#include "tacticballtracker.h"

TacticBallTracker::TacticBallTracker(WorldModel *worldmodel, QObject *parent) :
    Tactic("TacticBallTracker", worldmodel, parent)
{
    _face = Vector2D(3025,0);
}

RobotCommand TacticBallTracker::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;
    return rc;
}
