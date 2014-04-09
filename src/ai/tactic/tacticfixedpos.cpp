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

    Vector2D ballPos = wm->ball.pos.loc;

    Vector2D r = Field::ourGoalCenter - ballPos;
    r.normalize();
    r.scale(ALLOW_NEAR_BALL_RANGE);

    switch (wm->ourRobot[id].Role) {
    case AgentRole::FixedPositionMid:
        rc.fin_pos.loc = ballPos + r;
        break;
    case AgentRole::FixedPositionLeft:
        rc.fin_pos.loc = ballPos + r.rotatedVector(30);
        break;
    case AgentRole::FixedPositionRight:
        rc.fin_pos.loc = ballPos + r.rotatedVector(-30);
        break;
    }

    rc.useNav = true;

    return rc;
}

