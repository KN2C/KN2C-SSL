#include "tacticstop.h"

TacticStop::TacticStop(WorldModel *worldmodel, QObject *parent) :
    Tactic("TacticStop", worldmodel, parent)
{
}

void TacticStop::setStopID(int total, int current)
{
    stop_t = total;
    stop_c = current;
}

RobotCommand TacticStop::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;
    return rc;
}
