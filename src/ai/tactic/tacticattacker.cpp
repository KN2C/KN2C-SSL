#include "tacticattacker.h"

static bool ReachedToPos(Position current, Position desired, double distThreshold, double degThreshold)
{
    if(current.loc.dist(desired.loc) < distThreshold)
    {
        if(fabs((current.dir - desired.dir) * AngleDeg::RAD2DEG) < degThreshold ||
                (360.0 - fabs((current.dir - desired.dir) * AngleDeg::RAD2DEG)) < degThreshold)
        {
            return true;
        }
        else
        {
            qDebug() << "currentD: " << current.dir << " desiredD: " <<  desired.dir;

            return false;
        }
    }
    else
    {
        qDebug() << "currentP: " << current.loc.x << " desiredP: " <<  desired.loc.x;

        return false;
    }
}

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

static bool IsReadyForKick(Position current, Position desired, Vector2D ballPos, double distThreshold, double degThreshold, double degThreshold2)
{
//    Vector2D a;
//    Vector2D b;
//    Vector2D robotFront;
//    Circle2D(current.loc, ROBOT_RADIUS - 40).intersection(Ray2D(current.loc,AngleDeg(current.dir * AngleDeg::RAD2DEG)), &robotFront, nullptr);
//    double alpha;

//    if(fabs((current.dir - desired.dir) * AngleDeg::RAD2DEG) < degThreshold ||
//            (360.0 - fabs((current.dir - desired.dir) * AngleDeg::RAD2DEG)) < degThreshold)
//    {
//        a = (robotFront - current.loc).normalizedVector();
//        b = (ballPos - robotFront).normalizedVector();

//        alpha = acos(a.x * b.x + a.y * b.y) * AngleDeg::RAD2DEG;

//        if(alpha < degThreshold2)
//        {
//            if(robotFront.dist(ballPos) < distThreshold)
//            {
//                return true;
//            }
//            else
//            {
//                qDebug() << "currentP: " << current.loc.x << " desiredP: " <<  desired.loc.x;
//            }
//        }
//        else
//        {
//            return false;
//        }
//    }
//    else
//    {
//        qDebug() << "currentD: " << current.dir << " desiredD: " <<  desired.dir;

//        return false;
//    }


    if(fabs((current.dir - desired.dir) * AngleDeg::RAD2DEG) < degThreshold ||
            (360.0 - fabs((current.dir - desired.dir) * AngleDeg::RAD2DEG)) < degThreshold)
    {
        return CanKick(current, ballPos, ROBOT_RADIUS + distThreshold, degThreshold2);
    }
    else
    {
        return false;
    }
}

static bool IsInsideRect(Vector2D pos, Vector2D topLeft, Vector2D bottomRight)
{
    return (pos.x > topLeft.x && pos.x < bottomRight.x &&
            pos.y > bottomRight.y && pos.y < topLeft.y);
}

static bool IsInsideField(Vector2D pos)
{
    return pos.x > Field::MinX && pos.x < Field::MaxX &&
           pos.y > Field::MinY && pos.y < Field::MaxY;
}


static bool IsInsideGoalShape(Vector2D pos, double goalLeftX, double goalRadius, double goalCcOffset)
{
    double x = pos.x - goalLeftX;
    Vector2D ccl(goalLeftX, goalCcOffset / 2), ccr(goalLeftX, -goalCcOffset / 2);

    return (pos.dist(ccl) <= goalRadius || pos.dist(ccr) <= goalRadius ||
       (x >= 0 && x <= goalRadius && fabs(pos.y) <= goalCcOffset / 2));
}


static bool IsInsideGolieArea(Vector2D pos)
{
    return IsInsideGoalShape(pos, Field::ourGoalCenter.x, Field::goalCircle_R,
                             Field::ourGoalCC_L.dist(Field::ourGoalCC_R));
}

static uint CastRayToGoal(Vector2D origin, const Robot *oppRobots, double offsetStep, uint maxCount, Vector2D **vOut)
{
    if(offsetStep <= 0 || maxCount < 1 || origin == Vector2D::INVALIDATED || oppRobots == nullptr)
    {
        return 0;
    }

    int ibest = 0;
    Vector2D target;
    Vector2D *v = new Vector2D[maxCount];

    double cy = Field::oppGoalCC_R.y;
    double offset = (Field::oppGoalCC_L.y - Field::oppGoalCC_R.y) / offsetStep;

    bool s, ts = false;
    while(ibest < maxCount && cy < Field::oppGoalPost_L.y)
    {
        s = false;
        target = Vector2D(Field::MaxX, cy);
        for(int i = 0; i < PLAYERS_MAX_NUM; ++i)
        {
            if(oppRobots[i].isValid)
            {
                Circle2D c(oppRobots[i].pos.loc, ROBOT_RADIUS);                
                Line2D l(origin, target);
                if(c.intersection(l, nullptr, nullptr))
                {
                    s = true;
                    ts = true;
                    break;
                }
            }
        }

        if(!s)
        {
            v[ibest++] = target;
        }

        cy += offset;
    }

    if(ts)
    {
        *vOut = new Vector2D[ibest];
        for(int i = 0; i < ibest; ++i)
        {
            (*vOut)[i] = v[i];
        }
    }
    else
    {
        *vOut = new Vector2D[1];
        (*vOut)[0] = Field::oppGoalCenter;
        ibest = 1;
    }

    delete[] v;

    return ibest;
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

TacticAttacker::TacticAttacker(WorldModel *worldmodel, QObject *parent) :
    Tactic(worldmodel, parent)
{
}

RobotCommand TacticAttacker::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;

    double ballReachableRange = 800, ballInHandRange = ROBOT_RADIUS + 50;
    Vector2D workingRectTL = Vector2D(-1500, Field::MaxY);
    Vector2D workingRectBR = Vector2D(Field::MaxX, Field::MinY);
    int oppGoalerID = 0;

    // Keep it simple :)

    if(wm->ball.isValid)
    {
        double mydistToBall = wm->ball.pos.loc.dist(wm->ourRobot[id].pos.loc);

        // Ball is in territory.
        if(IsInsideRect(wm->ball.pos.loc, workingRectTL, workingRectBR))
        {
            int ourNearestID = -1;
            double dist, minDist = 100000000;

            // Check if another robot is near ball.
            for(int i = 0; i < PLAYERS_MAX_NUM; ++i)
            {
                if(wm->ourRobot[i].isValid)
                {
                    if((dist = wm->ball.pos.loc.dist(wm->ourRobot[i].pos.loc)) < minDist)
                    {
                        minDist = dist;
                        ourNearestID = i;
                    }

                }

                if(wm->oppRobot[i].isValid)
                {
                    // Take care of position passed in!
                    if(IsInsideGolieArea(-wm->oppRobot[i].pos.loc))
                    {
                        oppGoalerID = i;
                    }
                }
            }

            // I can Kick.
            if(mydistToBall < ballInHandRange)
            {
                int bestID = 0;
                Vector2D *t;
                uint rcount = CastRayToGoal(wm->ourRobot[id].pos.loc, wm->oppRobot, 30, 70, &t);

                // There is a hole :)
                if(rcount > 0)
                {
                    double d, dmax = -1;
                    // TODO: select the best one.
                    for(int i = 0; i < rcount; ++i)
                    {
                        if((d = wm->oppRobot[oppGoalerID].pos.loc.dist(t[i])) > dmax)
                        {
                            dmax = d;
                            bestID = i;
                        }
                    }

                    Position pos = AdjustKickPoint(wm->ball.pos.loc, t[bestID]);
                    rc.fin_pos = pos;

                    // We can kick right now.
                    if(IsReadyForKick(wm->ourRobot[id].pos, pos, wm->ball.pos.loc, 15, 2, 4))
                    {
                        //qDebug() << "Kick dir:" << Ray2D(wm->ourRobot[id].pos.loc, AngleDeg(wm->ourRobot[id].pos.dir * AngleDeg::RAD2DEG)).intersection(Field::rightLine).y;
                        qDebug() << "Kick dir:" << wm->ourRobot[id].pos.dir * AngleDeg::RAD2DEG;
                        qDebug() << "Kick adjust dir: " << pos.dir * AngleDeg::RAD2DEG;
                        rc.kickspeedx = 5;
                    }

                    delete[] t;
                }
                // There is no hole.
                else
                {
                    // TODO: think for a pass.
                    Position pos = AdjustKickPoint(wm->ball.pos.loc, Field::oppGoalCenter);
                    rc.fin_pos = pos;

                    // We can kick right now.
                    if(IsReadyForKick(wm->ourRobot[id].pos, pos, wm->ball.pos.loc, 15, 2, 7))
                    {
                        rc.kickspeedx = 5;
                    }
                }
            }
            // I'm near the ball.
            else if(mydistToBall < ballReachableRange)
            {
                // Go to ball and ready for a kick.
                rc.fin_pos.loc = wm->ball.pos.loc;
                rc.fin_pos.dir = 0;
            }
            // Our robots have access to ball.
            else if(minDist < ballReachableRange)
            {
                rc.fin_pos = wm->ourRobot[id].pos;
            }
            // No robot has access to ball.
            else
            {
                // Go to ball and ready for a kick.
                rc.fin_pos.loc = wm->ball.pos.loc;
                rc.fin_pos.dir = 0;
            }
        }
        // Ball is out of territory.
        else
        {
            rc.fin_pos = wm->ourRobot[id].pos;
        }

    }
    // There is no ball, do nothing.
    else
    {
        rc.fin_pos = wm->ourRobot[id].pos;
    }


    rc.maxSpeed = 2;

    rc.useNav = true;
    rc.isBallObs = false;
    rc.isKickObs = true;
}
