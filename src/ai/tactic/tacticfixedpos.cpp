#include "tacticfixedpos.h"

TacticFixedPos::TacticFixedPos(WorldModel *worldmodel, QObject *parent) :
    Tactic("TacticFixedPos", worldmodel, parent)
{
}

void TacticFixedPos::SetPosition(Position p)
{
    destination = p;
}

RobotCommand TacticFixedPos::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;

    if(wm->ourRobot[id].Role == AgentRole::FixedPositionLeft ||
            wm->ourRobot[id].Role == AgentRole::FixedPositionRight ||
            wm->ourRobot[id].Role == AgentRole::FixedPositionMid)
    {
        if(wm->ourRobot[id].Role == AgentRole::FixedPositionMid)
        {
            rc.maxSpeed = 0.4;
        }
        else
        {
            rc.maxSpeed = 1;
        }
        rc.fin_pos = destination;
    }
    else
    {
        Vector2D ballPos = wm->ball.pos.loc;
        Vector2D r = Field::ourGoalCenter - ballPos;
        r.normalize();
        r.scale(ALLOW_NEAR_BALL_RANGE);

        switch (wm->ourRobot[id].Role) {
        case AgentRole::ArcMid:
            rc.fin_pos.loc = ballPos + r;
            break;
        case AgentRole::ArcLeft:
            rc.fin_pos.loc = ballPos + r.rotatedVector(30);
            break;
        case AgentRole::ArcRight:
            rc.fin_pos.loc = ballPos + r.rotatedVector(-30);
            break;
        }

        rc.maxSpeed = 1;
    }

    rc.useNav = true;

    return rc;
}

