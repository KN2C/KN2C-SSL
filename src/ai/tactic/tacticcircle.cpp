#include "tacticcircle.h"

TacticCircle::TacticCircle(WorldModel *worldmodel, QObject *parent) :
    Tactic("TacticCircle", worldmodel, parent)
{
}

RobotCommand TacticCircle::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;
    Vector2D vc;
    vc = wm->ourRobot[id].pos.loc;
    vc.setLength(radius);
    vc.rotate(30);
    rc.fin_pos.loc = vc + center;
    rc.fin_pos.dir = (pointTo - wm->ourRobot[id].pos.loc).dir().radian();
    rc.maxSpeed = 1;
    return rc;
}
