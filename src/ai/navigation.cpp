#include "navigation.h"
#include "worldmodel.h"

Navigation::Navigation(QObject *parent) :
    QObject(parent)
{
    wm = 0;
    id = -1;
}

void Navigation::setID(int id)
{
    this->id = id;
}

void Navigation::setWorldModel(WorldModel *wm)
{
    this->wm = wm;
}

ControllerInput Navigation::calc(RobotCommand rc)
{
    Position mypos = wm->ourRobot[id].pos;
    Position myvel = wm->ourRobot[id].vel;

    ControllerInput ci;
    ci.cur_pos = mypos;
    ci.cur_vel = myvel;
    ci.fin_pos  = rc.fin_pos;
    ci.fin_vel  = rc.fin_vel;

    rc.useNav = false; //FIXME
    if(rc.useNav == false)
    {
        ci.mid_pos = rc.fin_pos;
        ci.mid_vel = rc.fin_vel;
        ci.fin_dist = (ci.cur_pos.loc - ci.fin_pos.loc).length();
    }
    else
    {
        QList<Vector2D> points;
        ci.fin_dist = getPath(rc, &points);
        if(points.size() >= 2)
            ci.mid_pos.loc = points[1];
        else
            ci.mid_pos.loc = ci.fin_pos.loc;
        ci.mid_pos.dir = ci.fin_pos.dir;
        ci.mid_vel = rc.fin_vel;
    }

    ci.maxSpeed  = rc.maxSpeed;
    ci.angleMode = rc.angleMode;
    return ci;
}

double Navigation::getPath(RobotCommand rc, QList<Vector2D> *points)
{
    Q_UNUSED(points);
    Position mypos = wm->ourRobot[id].pos;
    // TODO
    return (mypos.loc - rc.fin_pos.loc).length();
}
