#include "tactictestkickrange.h"

static bool CanKick(Position robotPos, Vector2D ballPos, double distLimit, double degLimit)
{
    AngleDeg d1((ballPos - robotPos.loc).dir());
    AngleDeg d2(robotPos.dir * AngleDeg::RAD2DEG);
    if(fabs((d1 - d2).degree()) < degLimit || (360.0 - fabs((d1 - d2).degree())) < degLimit)
    {
        if(robotPos.loc.dist(ballPos) < distLimit)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

TacticTestKickRange::TacticTestKickRange(WorldModel *worldmodel, QObject *parent) :
    Tactic(worldmodel, parent)
{
}

RobotCommand TacticTestKickRange::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;

    if(CanKick(wm->ourRobot[id].pos, wm->ball.pos.loc, wm->var[0], wm->var[1]))
    {
        rc.kickspeedx = 6;
    }

    rc.maxSpeed = 2;

    return rc;
}

