
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


static Vector2D PredictDestination(Vector2D mPos, Vector2D mVel, Vector2D pos, double speed, double slideFactor, double timeStep = 200, double timeout = 5000)
{
    if(speed <= 0 || timeStep <= 0 || timeout / timeStep > 250)
    {
        qDebug() << "Prediction error.";
        return mPos;
    }

    Vector2D cx;
    Vector2D d = mVel.normalizedVector();
    for(double t = 0; t < timeout; t+= timeStep)
    {
        cx = mPos + mVel * t - d * ((0.5 * slideFactor * t * t) + ROBOT_RADIUS + 30);
        if(pos.dist(cx) < speed * t)
        {
            qDebug() << "Predicted at t: " << t;
            return cx;
        }
    }

    qDebug() << "Prediction failed.";
    return mPos;
}


TacticTestFriction::TacticTestFriction(WorldModel *worldmodel, QObject *parent) :
    Tactic(worldmodel, parent)
{
}

RobotCommand TacticTestFriction::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;

    if(wm->ball.isValid && wm->ball.vel.loc.length() > 0.07 && wm->ball.pos.loc.x >100)
    {
    Vector2D v = PredictDestination(wm->ball.pos.loc, wm->ball.vel.loc, wm->ourRobot[id].pos.loc, 1.7, wm->var[3] / 500, 50);
    Position p = AdjustKickPoint(v, Field::oppGoalCenter);

    rc.fin_pos = p;
    }
    else
    {
        rc.fin_pos.loc = Vector2D(200,900);
    }


    rc.maxSpeed = 1.7;

    return rc;
}


