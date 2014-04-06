
#include "tactictestfriction.h"

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


TacticTestFriction::TacticTestFriction(WorldModel *worldmodel, QObject *parent) :
    Tactic(worldmodel, parent)
{
}

RobotCommand TacticTestFriction::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;

    double speed = 1.45;

    if(wm->ball.isValid && wm->ball.pos.loc.x >100 && wm->ball.pos.loc.x < 2000)
    {
        Vector2D v;
        if(wm->ball.vel.loc.length() > 0.06)
        {
            v = wm->kn->PredictDestination(wm->ourRobot[id].pos.loc, wm->ball.pos.loc, speed , wm->ball.vel.loc, wm->var[3] / 250);
        }
        else
        {
            v = wm->ball.pos.loc;
        }

        //Vector2D v = PredictDestination(wm->ball.pos.loc, wm->ball.vel.loc, wm->ourRobot[id].pos.loc, 1.7, wm->var[3] / 500, 50);
        Position p = AdjustKickPoint(v, Field::oppGoalCenter);

        rc.fin_pos = p;

        if(IsReadyForKick(wm->ourRobot[id].pos, p, wm->ball.pos.loc, wm->var[0], wm->var[1], wm->var[2]))
        {
            rc.kickspeedx = 6;
        }
    }
    else
    {
        rc.fin_pos.loc = Vector2D(200,900);
    }

    rc.fin_pos.dir = (Field::oppGoalCenter - wm->ourRobot[id].pos.loc).dir().radian();

    rc.maxSpeed = speed;

    return rc;
}


