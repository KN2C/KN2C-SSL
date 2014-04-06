#include "tacticattacker.h"

TacticAttacker::TacticAttacker(WorldModel *worldmodel, QObject *parent) :
    Tactic(worldmodel, parent)
{
}

void TacticAttacker::setAttackerID(int total, int current)
{
    att_t = total;
    att_c = current;
}

uint TacticAttacker::CastRayToGoal(Vector2D origin, const Robot *oppRobots, double offsetStep, uint maxCount, Vector2D *vOut, double beamWidth )
{
    if(offsetStep <= 0 || maxCount < 1 || origin == Vector2D::INVALIDATED || oppRobots == nullptr)
    {
        return 0;
    }

    uint ibest = 0;
    Vector2D target;
    Vector2D v[100];

    double cy = Field::oppGoalCC_R.y;
    double offset = offsetStep;

    bool s, noRobot = true;
    while(ibest < maxCount && cy < Field::oppGoalPost_L.y)
    {
        s = false;
        target = Vector2D(Field::MaxX, cy);
        Line2D l(origin, target);
        for(int i = 0; i < PLAYERS_MAX_NUM; ++i)
        {
            if(oppRobots[i].isValid)
            {
                if(l.dist(oppRobots[i].pos.loc) < ROBOT_RADIUS + beamWidth)
                {
                    s = true;
                    break;
                }

                noRobot = false;
            }
        }

        if(!s)
        {
            v[ibest++] = target;
        }

        cy += offset;
    }

    if(!noRobot)
    {
        if(ibest != 0)
        {
            if(vOut != NULL)
            {
                for(uint i = 0; i < ibest; ++i)
                {
                    vOut[i] = v[i];
                }
            }
        }
    }
    else
    {
        if(vOut != NULL)
        {
            vOut[0] = Field::oppGoalCenter;
        }

        ibest = 1;
    }

    return ibest;
}

Vector2D TacticAttacker::ScanFieldFreePos(Vector2D pos, const Robot *oppRobots, double gridSize, double localRange, double castStep)
{
    if(localRange <= 0 || castStep <= 0 || oppRobots == nullptr || gridSize <= 0 || pos == Vector2D::INVALIDATED)
        return Vector2D::INVALIDATED;

    uint maxRet = 0;

    double cx, cy;
    Vector2D p, bestPoint;

    cy = Field::MinY;
    while(cy <= Field::MaxY)
    {
        cx = Field::MinX;
        while(cx <= Field::MaxX)
        {
            p = Vector2D(cx, cy);
            if(p.dist(pos) <= localRange)
            {
                uint r = CastRayToGoal(p, oppRobots, castStep, 70, nullptr);
                if(r > maxRet)
                {
                    maxRet = r;
                    bestPoint = p;
                }
            }
            cx += 500;
        }
        cy += 300;
    }

    if(maxRet != 0)
    {
        return bestPoint;
    }
    else
    {
        return Vector2D::INVALIDATED;
    }
}

Vector2D TacticAttacker::GotoDefaultLoc(int total, int current)
{
    Vector2D out;

    switch (total) {
    case 1: // One attacker.
    {
        out.x = -Field::centerCircle_R;
        out.y = 0;
    }
        break;
    case 2: // Two attackers.
    {
        switch (current) {
        case 0: // Left attacker.
        {
            out.x = -500;
            out.y = Field::MaxY - 600;
        }
            break;
        case 1: // Right attacker.
        {
            out.x = -500;
            out.y = Field::MinY + 600;
        }
            break;
        }
    }
        break;
    case 3: // Three attackers.
    {
        switch (current) {
        case 0: // Left attacker.
        {
            out.x = -500;
            out.y = Field::MaxY - 600;
        }
            break;
        case 1: // Right attacker.
        {
            out.x = -500;
            out.y = Field::MinY + 600;
        }
            break;
        case 2: // Mid attacker.
        {
            out.x = -500 - ROBOT_RADIUS - 30;
            out.y = 0;
        }
            break;
        }
    }
        break;
    }

    return out;
}

bool TacticAttacker::CheckSweeping(int kickerID, double y, bool clearState)
{
   static double lastY = 0;
   static int rev = 0, counter = 0;
   static int lastKickerID = -1;
   static bool sweeped = false;

   if(clearState)
   {
       rev = 0;
       counter = 0;
       lastKickerID = -1;
       sweeped = false;
       return false;
   }

   if(lastKickerID != kickerID)
   {
       rev = 1;
       counter = 0;
       lastY = y;
       sweeped = false;
       lastKickerID = kickerID;

       return false;
   }
   else if(!sweeped)
   {
       if((y > lastY && rev == -1) || (y < lastY && rev == 1))
       {
           rev *= -1;
           ++counter;
       }
       lastY = y;
   }

   if(counter > 1)
   {
       sweeped = true;
       return true;
   }
   else
   {
       return false;
   }
}

RobotCommand TacticAttacker::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid || att_t < 1 || att_c >= att_t) return rc;
    wm->ourRobot[id].Status = AgentStatus::Idle;

    double ballSlidingForce = 2.5;
    double maxRobotSpeed = 2;
    double ballMovingThreshold = 0.4, ballKickingThreshold = 1;
    double ballReachableRange = 700, ballInHandRange = ROBOT_RADIUS + 70;
    Vector2D workingRectTL = Vector2D(-1800, Field::MaxY);
    Vector2D workingRectBR = Vector2D(Field::MaxX, Field::MinY);
    int oppGoalerID = 0;

    // Keep it simple :)

    if(wm->ball.isValid && wm->kn->IsInsideField(wm->ball.pos.loc))
    {
        int ourNearestID = -1;
        double friendDistToBall = 100000000;

        double myPredDist, friendPredDist;
        double myDistToBall = wm->ball.pos.loc.dist(wm->ourRobot[id].pos.loc);
        Vector2D myPredPos;

        // Check if another robot is near ball.
        for(int i = 0; i < PLAYERS_MAX_NUM; ++i)
        {
            if(wm->ourRobot[i].isValid)
            {
                double dist;
                if(i != id && (dist = wm->ball.pos.loc.dist(wm->ourRobot[i].pos.loc)) < friendDistToBall)
                {
                    friendDistToBall = dist;
                    ourNearestID = i;
                }
            }

            if(wm->oppRobot[i].isValid)
            {
                // Take care of position passed in!
                if(wm->kn->IsInsideGolieArea(-wm->oppRobot[i].pos.loc))
                {
                    oppGoalerID = i;
                }
            }
        }

        // Clear sweeping state if no one has the ball.
        if(myDistToBall > ballReachableRange && friendDistToBall > ballReachableRange)
        {
            CheckSweeping(id, 0, true);
        }

        // Who is nearer to ball, use prediction if necessary.
        if(wm->ball.vel.loc.length() < ballMovingThreshold)
        {
            qDebug() << "Not using prediction...";
            myPredDist = myDistToBall;
            friendPredDist = friendDistToBall;
        }
        // Use prediction.
        else
        {
            qDebug() << "Using prediction...";
            myPredPos = wm->kn->PredictDestination(wm->ourRobot[id].pos.loc, wm->ball.pos.loc, maxRobotSpeed, wm->ball.vel.loc, wm->var[3]);
            myPredDist = wm->ourRobot[id].pos.loc.dist(myPredPos);
            if(ourNearestID != -1)
            {
                friendPredDist = wm->ourRobot[ourNearestID].pos.loc.dist(wm->kn->PredictDestination(wm->ourRobot[ourNearestID].pos.loc,
                                                                                                    wm->ball.pos.loc, maxRobotSpeed, wm->ball.vel.loc, wm->var[3]));
            }
            else
            {
                friendPredDist = friendDistToBall;
            }
        }

        // Ball is in territory.
        if(wm->kn->IsInsideRect(wm->ball.pos.loc, workingRectTL, workingRectBR))
        {
            // I'm the nearest.
            if(myPredDist < friendPredDist)
            {
                // I can Kick.
                if(myDistToBall < ballInHandRange)
                {
                    uint bestID = 0;
                    Vector2D t[100];
                    uint rcount = CastRayToGoal(wm->ourRobot[id].pos.loc, wm->oppRobot, 30, 70, t, 2 * BALL_RADIUS);

                    // There is a hole :)
                    if(rcount > 0)
                    {
                        wm->ourRobot[id].Status = AgentStatus::Kicking;

                        double dmax = -1;
                        // TODO: select the best one.
                        for(uint i = 0; i < rcount; ++i)
                        {
                            double d;
                            if((d = wm->oppRobot[oppGoalerID].pos.loc.dist(t[i])) > dmax)
                            {
                                dmax = d;
                                bestID = i;
                            }
                        }

                        // Anti sweeping algorithm.
                        if(!CheckSweeping(id, t[bestID].y))
                        {
                            Position pos = wm->kn->AdjustKickPoint(wm->ball.pos.loc, t[bestID]);
                            rc.fin_pos = pos;

                            // Calculate precise kick threshold.
                            double d1 = (Vector2D(t[bestID].x, t[bestID].y + 30) - wm->ball.pos.loc).dir().degree();
                            double d2 = (Vector2D(t[bestID].x, t[bestID].y - 30) - wm->ball.pos.loc).dir().degree();
                            double d = fabs(d1 - d2) / 2;
                            if(d < 2)
                            {
                                d = 2;
                            }

                            // We can kick right now.
                            if(wm->kn->IsReadyForKick(wm->ourRobot[id].pos, pos, wm->ball.pos.loc))
                            {
                                qDebug() << "Kicking to y = " << t[bestID].y;
                                rc.kickspeedx = 6;
                            }
                        }
                        else
                        {
                            qDebug() <<"Sweeper detected, dont waste time, Kick!";

                            wm->ourRobot[id].Status = AgentStatus::Kicking;

                            Position pos = wm->kn->AdjustKickPoint(wm->ball.pos.loc, Field::oppGoalCenter);
                            rc.fin_pos = pos;

                            // We can kick right now.
                            if(wm->kn->IsReadyForKick(wm->ourRobot[id].pos, pos, wm->ball.pos.loc))
                            {
                                qDebug() << "Kicking to y = " << pos.loc.y;
                                rc.kickspeedx = 6;
                            }
                        }
                    }
                    // There is no hole, pass to other robots.
                    else
                    {
                        qDebug() << "No Room to goal!";

                        // Limit pass direction.
                        Ray2D ray = Ray2D(wm->ourRobot[id].pos.loc, wm->ourRobot[ourNearestID].pos.loc);
                        Vector2D ss = ray.intersection(Line2D(workingRectTL, Vector2D(workingRectTL.x, workingRectBR.y)));

                        if(ourNearestID != -1 && (ss == Vector2D::INVALIDATED || !wm->kn->IsInsideField(ss)))
                        {
                            qDebug() << "Passing...";
                            wm->ourRobot[id].Status = AgentStatus::Passing;

                            Vector2D passTarget = Vector2D(wm->ourRobot[ourNearestID].pos.loc.x + ROBOT_RADIUS,
                                                           wm->ourRobot[ourNearestID].pos.loc.y);
                            // Calculate pass speed.
                            double speed = sqrt(2 * ballSlidingForce * wm->ball.pos.loc.dist(passTarget) / 1000);
                            Position pos = wm->kn->AdjustKickPoint(wm->ball.pos.loc, passTarget, speed);
                            rc.fin_pos = pos;

                            // We can pass right now.
                            if(wm->kn->IsReadyForKick(wm->ourRobot[id].pos, pos, wm->ball.pos.loc))
                            {
                                qDebug() << "Passing from " << id << " to " << ourNearestID;
                                rc.kickspeedx = speed;
                            }
                        }
                        // There is no nearby robot to pass, so kick.
                        else
                        {
                            if(ourNearestID == -1)
                            {
                                qDebug() <<"No nearby robot to pass, Kick!";
                            }
                            else
                            {
                                qDebug() <<"Nearby robot position is bad, can't pass, Kick!";
                            }

                            wm->ourRobot[id].Status = AgentStatus::Kicking;

                            Position pos = wm->kn->AdjustKickPoint(wm->ball.pos.loc, Field::oppGoalCenter);
                            rc.fin_pos = pos;

                            // We can kick right now.
                            if(wm->kn->IsReadyForKick(wm->ourRobot[id].pos, pos, wm->ball.pos.loc))
                            {
                                qDebug() << "Kicking to y = " << pos.loc.y;
                                rc.kickspeedx = 6;
                            }
                        }
                    }
                }
                // I'm near the ball.
                else
                {
                    // Go to ball position, use prediction if necessary.
                    if(wm->ball.vel.loc.length() < ballMovingThreshold)
                    {
                        rc.fin_pos = wm->kn->AdjustKickPoint(wm->ball.pos.loc, Field::oppGoalCenter);
                    }
                    // Use prediction.
                    else
                    {
                        // Dont follow ball if we passed the ball.
                        Ray2D rb(wm->ball.pos.loc, wm->ball.pos.loc - wm->ball.vel.loc);
                        Circle2D circle = Circle2D(wm->ourRobot[id].pos.loc, ROBOT_RADIUS);
                        if(circle.intersection(rb, nullptr, nullptr) > 0 && wm->ball.vel.loc.length() > ballKickingThreshold)
                        {
                            qDebug() << "Dont follow my kicking ball.";
                            rc.fin_pos = wm->ourRobot[id].pos;
                        }
                        else
                        {
                            Position v = wm->kn->AdjustKickPoint(myPredPos, Field::oppGoalCenter);
                            rc.fin_pos = v;
                        }
                    }
                }
            }
            // Friend robot is near to ball, may pass to me.
            else
            {
                // TODO: goto best pos.
                if(wm->ourRobot[ourNearestID].Status != AgentStatus::Passing)
                {
                    qDebug() << "Scanning field for free pos...";
                    Vector2D v, scanPoint;

                    scanPoint.x = wm->ourRobot[ourNearestID].pos.loc.x;
                    if(fabs(wm->ourRobot[ourNearestID].pos.loc.y) > 750)
                    {
                        scanPoint.y = -wm->ourRobot[ourNearestID].pos.loc.y;
                    }
                    else
                    {
                        if(wm->ourRobot[ourNearestID].pos.loc.y >= 0)
                        {
                            if(wm->ourRobot[id].pos.loc.y >= 0)
                            {
                                scanPoint.y = wm->ourRobot[ourNearestID].pos.loc.y + 500;
                            }
                            else
                            {
                                scanPoint.y = -wm->ourRobot[ourNearestID].pos.loc.y / 5 - 750;
                            }
                        }
                        else
                        {
                            if(wm->ourRobot[id].pos.loc.y <= 0)
                            {
                                scanPoint.y = wm->ourRobot[ourNearestID].pos.loc.y - 500;
                            }
                            else
                            {
                                scanPoint.y = wm->ourRobot[ourNearestID].pos.loc.y / 5 + 750;
                            }
                        }
                    }

                    v = ScanFieldFreePos(scanPoint, wm->oppRobot);
                    if(v != Vector2D::INVALIDATED)
                    {
                        qDebug() << "Found at: " << v.x << ", " << v.y;
                        Position kp = wm->kn->AdjustKickPoint(v, Field::oppGoalCenter);
                        wm->kn->ClampToRect(&v, workingRectTL, workingRectBR);
                        rc.fin_pos = kp;
                    }
                    else
                    {
                        qDebug() << "Not found!";
                        rc.fin_pos = wm->ourRobot[id].pos;
                    }
                }
                // Stay here to receive pass.
                else
                {
                    qDebug() << "Receiving a pass...";
                    rc.fin_pos = wm->ourRobot[id].pos;
                }
            }
        }
        // Ball is out of territory.
        else
        {
            // I'm the nearest.
            if(myPredDist < friendPredDist && wm->kn->IsInsideRect(myPredPos, workingRectTL, workingRectBR))
            {
                // Go to ball position, use prediction.

                Position v = wm->kn->AdjustKickPoint(myPredPos, Field::oppGoalCenter);
                if(wm->kn->IsInsideRect(v.loc, workingRectTL, workingRectBR))
                {
                    rc.fin_pos = v;
                }
                else
                {
                    rc.fin_pos = wm->ourRobot[id].pos;
                }
            }
            // Friend robot can catch the ball, may pass to me.
            else
            {
                // TODO: goto best pos.
                if(wm->ourRobot[ourNearestID].Status != AgentStatus::Passing)
                {
                    qDebug() << "Scanning field for free pos...";
                    Vector2D v, scanPoint;

                    scanPoint.x = wm->ourRobot[ourNearestID].pos.loc.x;
                    if(fabs(wm->ourRobot[ourNearestID].pos.loc.y) > 750)
                    {
                        scanPoint.y = -wm->ourRobot[ourNearestID].pos.loc.y;
                    }
                    else
                    {
                        if(wm->ourRobot[ourNearestID].pos.loc.y >= 0)
                        {
                            if(wm->ourRobot[id].pos.loc.y >= 0)
                            {
                                scanPoint.y = wm->ourRobot[ourNearestID].pos.loc.y + 500;
                            }
                            else
                            {
                            scanPoint.y = -wm->ourRobot[ourNearestID].pos.loc.y / 5 - 750;
                            }
                        }
                        else
                        {
                            if(wm->ourRobot[id].pos.loc.y <= 0)
                            {
                                scanPoint.y = wm->ourRobot[ourNearestID].pos.loc.y - 500;
                            }
                            else
                            {
                                scanPoint.y = wm->ourRobot[ourNearestID].pos.loc.y / 5 + 750;
                            }
                        }
                    }

                    v = ScanFieldFreePos(scanPoint, wm->oppRobot);

                    if(v != Vector2D::INVALIDATED)
                    {
                        qDebug() << "Found at: " << v.x << ", " << v.x;
                        Position kp = wm->kn->AdjustKickPoint(v, Field::oppGoalCenter);

                        if(att_t > 1)
                        {
                            // Clamp to left attacker area.
                            if(att_c == 0)
                            {
                                wm->kn->ClampToRect(&v, workingRectTL, Vector2D(workingRectBR.x, 1250));
                            }
                            // Clamp to right attacker area.
                            else if(att_c == 1)
                            {
                                wm->kn->ClampToRect(&v, Vector2D(workingRectTL.x, -1250),workingRectBR);
                            }
                            else
                            {
                                wm->kn->ClampToRect(&v, workingRectTL, workingRectBR);
                            }
                        }
                        else
                        {
                            wm->kn->ClampToRect(&v, workingRectTL, workingRectBR);
                        }

                        rc.fin_pos = kp;
                    }
                    else
                    {
                        qDebug() << "Not found!";
                        rc.fin_pos = wm->ourRobot[id].pos;
                    }
                }
                // Stay here to receive pass.
                else
                {
                    qDebug() << "Receiving a pass...";
                    rc.fin_pos = wm->ourRobot[id].pos;
                }
            }
        }
    }
    // There is no ball, do nothing.
    else
    {
        rc.fin_pos.loc = GotoDefaultLoc(att_t, att_c);
        rc.fin_pos.dir = 0;
    }

    rc.maxSpeed = maxRobotSpeed;

    rc.useNav = true;
    rc.isBallObs = false;
    rc.isKickObs = true;

    if(rc.fin_pos.loc == wm->ball.pos.loc)
        qDebug() << "AAAAAAAAA";


    return rc;
}
