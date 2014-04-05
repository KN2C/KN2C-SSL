#include "tactictestkickprecision.h"

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

static Position AdjustKickPoint(Vector2D ballPos, Vector2D target, int kickSpeed = 5)
{
    Position p;
    Vector2D dir = (ballPos - target).normalizedVector();
    dir.scale(ROBOT_RADIUS - 20);

    p.loc = ballPos + dir;
    p.dir = (-dir).dir().radian();

    return p;
}

static bool IsReadyForKick(Position current, Position desired, Vector2D ballPos, double distThreshold, double degThreshold, double degThreshold2)
{
    if(fabs((current.dir - desired.dir) * AngleDeg::RAD2DEG) < degThreshold2 ||
            (360.0 - fabs((current.dir - desired.dir) * AngleDeg::RAD2DEG)) < degThreshold2)
    {
        return CanKick(current, ballPos, distThreshold, degThreshold);
    }
    else
    {
        return false;
    }
}


TacticTestKickPrecision::TacticTestKickPrecision(WorldModel *worldmodel, QObject *parent) :
    Tactic(worldmodel, parent)
{
}

RobotCommand TacticTestKickPrecision::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;

    Position target = AdjustKickPoint(wm->ball.pos.loc, Field::ourGoalCenter);

    if(IsReadyForKick(wm->ourRobot[id].pos, target, wm->ball.pos.loc, wm->var[0], wm->var[1], wm->var[2]))
    {
        rc.kickspeedx = 6;
    }

    rc.fin_pos = target;

    rc.maxSpeed = 1.5;

    return rc;
}


