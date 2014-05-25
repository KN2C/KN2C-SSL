#include "tactictestkickprecision.h"

TacticTestKickPrecision::TacticTestKickPrecision(WorldModel *worldmodel, QObject *parent) :
    Tactic("TacticTestKickPrecision", worldmodel, parent)
{
}

RobotCommand TacticTestKickPrecision::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;

    Position target = wm->kn->AdjustKickPoint(wm->ball.pos.loc, Field::ourGoalCenter);

    if(wm->kn->IsReadyForKick(wm->ourRobot[id].pos, target, wm->ball.pos.loc))
    {
        rc.kickspeedx = 255;
    }

    rc.fin_pos = target;

    rc.maxSpeed = 1.5;

    return rc;
}


