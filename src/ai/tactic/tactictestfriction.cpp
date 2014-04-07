#include "tactictestfriction.h"

TacticTestFriction::TacticTestFriction(WorldModel *worldmodel, QObject *parent) :
    Tactic("TacticTestFriction", worldmodel, parent)
{
}

RobotCommand TacticTestFriction::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;

    double speed = 1.6;

    if(wm->ball.isValid && wm->ball.pos.loc.x >100 && wm->ball.pos.loc.x < 2000)
    {
        Vector2D v;
        if(wm->ball.vel.loc.length() > 0.06)
        {
            v = wm->kn->PredictDestination(wm->ourRobot[id].pos.loc, wm->ball.pos.loc, speed , wm->ball.vel.loc);
        }
        else
        {
            v = wm->ball.pos.loc;
        }

        //Vector2D v = PredictDestination(wm->ball.pos.loc, wm->ball.vel.loc, wm->ourRobot[id].pos.loc, 1.7, wm->var[3] / 500, 50);
        Position p = wm->kn->AdjustKickPoint(v, Field::oppGoalCenter);

        rc.fin_pos = p;

        if(wm->kn->IsReadyForKick(wm->ourRobot[id].pos, p, wm->ball.pos.loc))
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


