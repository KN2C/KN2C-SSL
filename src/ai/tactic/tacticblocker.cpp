#include "tacticblocker.h"

TacticBlocker::TacticBlocker(WorldModel *worldmodel, QObject *parent) :
    Tactic("TacticBlocker", worldmodel, parent)
{
}

RobotCommand TacticBlocker::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;    

    wm->ourRobot[id].Status = AgentStatus::Idle;

    double maxRobotSpeed = 1.5;

    int ballOwnerID = -1, recID1 = -1, recID2 = -1, blockerID = -1;
    Position ballOwnerPos;
    double minDist = 100000000;
    double blockDist = 4 * ROBOT_RADIUS;

    double maxDot1 = -2, maxDot2 = -2;

    if(!wm->cmgs.allowedNearBall())
    {
        if(wm->ourRobot[id].pos.loc.dist(wm->ball.pos.loc) - ALLOW_NEAR_BALL_RANGE > 0 &&
           wm->ourRobot[id].pos.loc.dist(wm->ball.pos.loc) - ALLOW_NEAR_BALL_RANGE < 100)
        {
            rc.fin_pos = wm->ourRobot[id].pos;

            rc.maxSpeed = maxRobotSpeed;

            rc.useNav = true;
            rc.isBallObs = false;
            rc.isKickObs = true;

            return rc;
        }
    }

    // Finding opponent ball owner.
    for (int i = 0; i < PLAYERS_MAX_NUM; ++i)
    {
        if(wm->oppRobot[i].isValid)
        {
            double dist = wm->oppRobot[i].pos.loc.dist(wm->ball.pos.loc);
            if(dist < minDist)
            {
                minDist = dist;
                ballOwnerID = i;

                ballOwnerPos = wm->oppRobot[i].pos;
            }
        }
    }

    // Guess who will receive pass of ball owner. [I]
    for(int i = 0; i < PLAYERS_MAX_NUM; ++i)
    {
        if(i != ballOwnerID && wm->oppRobot[i].isValid)
        {
            Vector2D a((wm->oppRobot[i].pos.loc - ballOwnerPos.loc).normalizedVector());
            Vector2D b(cos(ballOwnerPos.dir), sin(ballOwnerPos.dir));

            double dot = a.innerProduct(b);
            if(dot > maxDot1)
            {
                maxDot1 = dot;
                recID1 = i;
            }
        }
    }

    // Guess who will receive pass of ball owner. [II]
    for(int i = 0; i < PLAYERS_MAX_NUM; ++i)
    {
        if(i != ballOwnerID && i!= recID1 && wm->oppRobot[i].isValid)
        {
            Vector2D a((wm->oppRobot[i].pos.loc - ballOwnerPos.loc).normalizedVector());
            Vector2D b(cos(ballOwnerPos.dir), sin(ballOwnerPos.dir));

            double dot = a.innerProduct(b);
            if(dot > maxDot2)
            {
                maxDot2 = dot;
                recID2 = i;
            }
        }
    }

    // Find other blocker id. There is only one else.
    for(int i = 0; i < PLAYERS_MAX_NUM; ++i)
    {
        if(i != id && wm->ourRobot[i].isValid && wm->ourRobot[i].Role == AgentRole::Blocker)
        {
            blockerID = i;
            break;
        }
    }

    Vector2D midPoint1(Vector2D::INVALIDATED), midPoint2(Vector2D::INVALIDATED);
    double myDist1 = 1000000000, myDist2 = 1000000000;
    double blockerDist1 = 1000000000, blockerDist2 = 1000000000;

    if(recID1 != -1)
    {
        midPoint1 = wm->oppRobot[recID1].pos.loc + (wm->ball.pos.loc - wm->oppRobot[recID1].pos.loc).normalizedVector().scale(blockDist);
        myDist1 = midPoint1.dist(wm->ourRobot[id].pos.loc);

        if(blockerID != -1)
        {
            blockerDist1 = midPoint1.dist(wm->ourRobot[blockerID].pos.loc);
        }
    }

    if(recID2 != -1)
    {
        midPoint2 = wm->oppRobot[recID2].pos.loc + (wm->ball.pos.loc - wm->oppRobot[recID2].pos.loc).normalizedVector().scale(blockDist);
        myDist2 = midPoint2.dist(wm->ourRobot[id].pos.loc);

        if(blockerID != -1)
        {
            blockerDist2 = midPoint2.dist(wm->ourRobot[blockerID].pos.loc);
        }
    }

    // Two opponent robots may receive pass.
    if(recID1 != -1 && recID2 != -1)
    {
        // There is another blocker in team.
        if(blockerID != -1)
        {
            if(myDist1 + blockerDist2 < myDist2 + blockerDist1)
            {
                wm->ourRobot[id].Status = AgentStatus::BlockingPass;

                rc.fin_pos.loc = midPoint1;
                rc.fin_pos.dir = (wm->ball.pos.loc - midPoint1).dir().radian();
            }
            else
            {
                wm->ourRobot[id].Status = AgentStatus::BlockingRobot;

                rc.fin_pos.loc = midPoint2;
                rc.fin_pos.dir = (wm->ball.pos.loc - midPoint2).dir().radian();
            }
        }
        // Only I'm the blocker. TODO: we can decide better with more conditioning.
        else
        {
            wm->ourRobot[id].Status = AgentStatus::BlockingPass;

            rc.fin_pos.loc = midPoint1;
            rc.fin_pos.dir = (wm->ball.pos.loc - midPoint2).dir().radian();
        }
    }
    // One opponent robot may receive pass.
    else if(recID1 != -1 && recID2 == -1)
    {
        // There is another blocker in team.
        if(blockerID != -1)
        {
            if(myDist1 < blockerDist1)
            {
                wm->ourRobot[id].Status = AgentStatus::BlockingPass;

                rc.fin_pos.loc = midPoint1;
                rc.fin_pos.dir = (wm->ball.pos.loc - midPoint1).dir().radian();
            }
            else
            {
                rc.fin_pos.loc = Vector2D(Field::ourGoalCenter.x + Field::goalCircleDEF_R + 200, 0);
                rc.fin_pos.dir = 0;
            }
        }
        // Only I'm the blocker.
        else
        {
            wm->ourRobot[id].Status = AgentStatus::BlockingPass;

            rc.fin_pos.loc = midPoint1;
            rc.fin_pos.dir = (wm->ball.pos.loc - midPoint1).dir().radian();
        }
    }
    // No opponent robot may receive pass.
    else
    {
        rc.fin_pos.loc = Vector2D(Field::ourGoalCenter.x + Field::goalCircleDEF_R + 200, 0);
        rc.fin_pos.dir = 0;
    }

    // Prevent from going into golie area.
    if(wm->kn->IsInsideGoalShape(rc.fin_pos.loc, Field::ourGoalCenter.x, Field::goalCircleDEF_R,
                                 (Field::ourGoalCC_L - Field::ourGoalCC_R).length()))
    {
        if(wm->kn->IsInsideGoalShape(wm->ball.pos.loc, Field::ourGoalCenter.x, Field::goalCircleDEF_R,
                                     (Field::ourGoalCC_L - Field::ourGoalCC_R).length()))
        {
            wm->ourRobot[id].Status = AgentStatus::Idle;
            rc.fin_pos.loc = Vector2D(Field::ourGoalCenter.x + Field::goalCircleDEF_R + 200, 0);
            rc.fin_pos.dir = 0;
        }
        else
        {
            if(wm->kn->IsInsideGoalShape((rc.fin_pos.loc + wm->ball.pos.loc * 3) * 0.25, Field::ourGoalCenter.x, Field::goalCircleDEF_R,
                                         (Field::ourGoalCC_L - Field::ourGoalCC_R).length()))
            {
                wm->ourRobot[id].Status = AgentStatus::BlockingRobot;
                rc.fin_pos = wm->kn->AdjustKickPoint(wm->ball.pos.loc, Field::oppGoalCenter);
            }
            else
            {
                wm->ourRobot[id].Status = AgentStatus::BlockingPass;
                rc.fin_pos.loc = (rc.fin_pos.loc + wm->ball.pos.loc * 3) * 0.25;
            }
        }
    }

    if(!wm->cmgs.allowedNearBall())
    {
        if(wm->ourRobot[id].pos.loc.dist(wm->ball.pos.loc) < ALLOW_NEAR_BALL_RANGE)
        {
            rc.fin_pos.loc = wm->ball.pos.loc + (wm->ourRobot[id].pos.loc - wm->ball.pos.loc).normalizedVector().scale(ALLOW_NEAR_BALL_RANGE);
        }
    }

    rc.maxSpeed = maxRobotSpeed;

    rc.useNav = true;
    rc.isBallObs = false;
    rc.isKickObs = true;

    return rc;
}
