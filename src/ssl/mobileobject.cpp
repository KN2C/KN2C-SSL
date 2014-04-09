#include "mobileobject.h"

MobileObject::MobileObject() :
    QObject(0)
{
    time = 0;
    camera = 0;
    isValid = false;

    connect(&timer_seen, SIGNAL(timeout()), this, SLOT(timer_seen_timeout()));
    connect(&timer_vel, SIGNAL(timeout()), this, SLOT(timer_vel_timeout()));

    timer_seen_interval = 500;
    timer_vel_interval  = 40;

    for(int i=0; i<LAST_COUNT; i++)
        last_postc[i].time = -1;
}

void MobileObject::timer_seen_timeout()
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

    PositionTimeCamera last = vel_postc;
    vel.loc = vel.loc + (((pos.loc - last.pos.loc) / (time - last.time)) - vel.loc) * 0.22;
    vel.dir = (pos.dir - last.pos.dir) / (time - last.time);

    vel_postc.pos = pos;
    vel_postc.time = time;
}

void MobileObject::seenAt(vector<Position> p, double t, int c)
{
    if(p.size()<1) return;
    isValid = true;
    timer_seen.start(timer_seen_interval); //restart
    if(!timer_vel.isActive()) timer_vel.start(timer_vel_interval);

    PositionTimeCamera ans;
    ans.time = t;
    ans.camera = c;

    int min_i = 0;
    double min_d = pos.loc.dist2(p[0].loc);
    for(size_t i=0; i < p.size(); i++)
    {
        double d = pos.loc.dist2(p[i].loc);
        if(d < min_d)
        {
            min_d = d;
            min_i = i;
        }
    }

    Position sel_pos = p[min_i];
    ans.pos.loc = pos.loc + (sel_pos.loc - pos.loc) * 0.5;
    ans.pos.dir = fabs(pos.dir) + (fabs(sel_pos.dir) - fabs(pos.dir)) * 0.8;
    if(sel_pos.dir < 0 ) ans.pos.dir *= -1;

    appendPostc(ans);
    /*
    camera = ans.camera;
    time = ans.time;
    pos = ans.pos;
    return;
    */

    min_i = 0;
    min_d = pos.loc.dist2(last_postc[0].pos.loc);
    for(int i=0; i<LAST_COUNT; i++)
    {
        if(last_postc[i].time < 0) continue;
        double d = pos.loc.dist2(last_postc[i].pos.loc);
        if(d < min_d)
        {
            min_d = d;
            min_i = i;
        }
    }

    PositionTimeCamera res = last_postc[min_i];
    camera = res.camera;
    time = res.time;
    pos = res.pos;
}

void MobileObject::appendPostc(PositionTimeCamera &postc)
{
    for(int i = LAST_COUNT-1; i>0; i--)
        last_postc[i] = last_postc[i-1];
    last_postc[0] = postc;
}
