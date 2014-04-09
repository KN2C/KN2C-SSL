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

    Vector2D left, mid, right;

    mid = ballPos + r;
    left = ballPos + r.rotatedVector(28);
    right = ballPos + r.rotatedVector(-28);

    bool bl = false, bm = false, br = false;

    // Mid.
    for(int i = 0; i < PLAYERS_MAX_NUM; ++i)
    {
        if(wm->ourRobot[i].isValid && wm->ourRobot[i].pos.loc.dist(mid) < ROBOT_RADIUS)
        {
            bm = true;
            break;
        }

        if(wm->ourRobot[i].isValid && wm->ourRobot[i].pos.loc.dist(left) < ROBOT_RADIUS)
        {
            bl = true;
            break;
        }

        if(wm->ourRobot[i].isValid && wm->ourRobot[i].pos.loc.dist(right) < ROBOT_RADIUS)
        {
            br = true;
            break;
        }
    }

    if(!bm)
    {
        rc.fin_pos.loc = mid;
        rc.fin_pos.dir = (-r).dir().radian();
    }
    else if(!bl)
    {
        rc.fin_pos.loc = left;
        rc.fin_pos.dir = (-(r.rotatedVector(28))).dir().radian();
    }
    else if(!br)
    {
        rc.fin_pos.loc = right;
        rc.fin_pos.dir = (-(r.rotatedVector(-28))).dir().radian();
    }
    else
    {
        rc.fin_pos = wm->ourRobot[id].pos;
    }

    rc.useNav = true;

    return rc;
}

