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

    ci.fin_pos = rc.fin_pos;
    ci.maxSpeed = rc.maxSpeed;

    return ci;
}
