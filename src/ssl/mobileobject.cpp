#include "mobileobject.h"

MobileObject::MobileObject() :
    QObject(0),
    time(0),
    isValid(false),
    last_time(0)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));
    connect(&timer_vel, SIGNAL(timeout()), this, SLOT(timer_vel_timeout()));
    timer_interval = 500;
    timer_vel_interval = 40;
}

void MobileObject::timer_timeout()
{
    if(isValid==true)
    {
        //qDebug() << "isValid == false";
        isValid = false;
    }
}

void MobileObject::timer_vel_timeout()
{
    if(!isValid)
    {
        vel.loc = {0, 0};
        vel.dir = 0;
        return;
    }

    vel.loc = ((vel.loc + (((pos.loc - last_pos.loc) / (time - last_time)) - vel.loc)*.22));
    //vel.loc.x=((int)(vel.loc.x*1000))/1000;
    //vel.loc.y=((int)(vel.loc.y*1000))/1000;
    vel.dir = (pos.dir - last_pos.dir) / (time - last_time);
    last_pos = pos;
    last_time = time;
}

void MobileObject::seenAt(vector<Position> p, double t, int camera)
{
    if(p.size()<1) return;

    Q_UNUSED(camera);
    time = t;
    isValid = true;

    timer.start(timer_interval);
    if(!timer_vel.isActive())
        timer_vel.start(timer_vel_interval);

    int min_i = 0;
    double min_d = (pos.loc - p[0].loc).length2();

    for(size_t i=0; i < p.size(); i++)
    {
        auto d = (pos.loc - p[i].loc).length2();
        if(d < min_d)
        {
            min_d = d;
            min_i = i;
        }
    }

    pos.loc = pos.loc + (p[min_i].loc - pos.loc) * 0.5;
    pos.dir = pos.dir + (p[min_i].dir - pos.dir) * 0.8;
}


