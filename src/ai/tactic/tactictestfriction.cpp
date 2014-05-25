#include "tactictestfriction.h"

TacticTestFriction::TacticTestFriction(WorldModel *worldmodel, QObject *parent) :
    Tactic("TacticTestFriction", worldmodel, parent)
{
}

RobotCommand TacticTestFriction::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;

    double speed = 1.5;

    if(wm->ball.isValid && wm->ball.pos.loc.x <-100 && wm->ball.pos.loc.x > -2000)
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

        Position p = wm->kn->AdjustKickPoint(v, Field::ourGoalCenter);

        rc.fin_pos = p;

        if(wm->kn->IsReadyForKick(wm->ourRobot[id].pos, p, wm->ball.pos.loc))
        {
            qDebug() << "KIKKKIKIKIKIKIK " << wm->ball.pos.loc.x;
            rc.kickspeedx = 255;
        }
    }
    else
    {
        rc.fin_pos.loc = Vector2D(-200,900);
        rc.fin_pos.dir = AngleDeg::PI;
    }

    rc.fin_pos.dir = (Field::ourGoalCenter - wm->ourRobot[id].pos.loc).dir().radian();

    rc.maxSpeed = speed;

    rc.useNav = false;

    return rc;
}


