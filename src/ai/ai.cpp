#include "ai.h"

AI::AI(WorldModel *worldmodel, OutputBuffer *outputbuffer, QObject *parent) :
    QObject(parent),
    wm(worldmodel),
    outputbuffer(outputbuffer)
{
    qDebug() << "AI Initialization...";
    connect(&timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));

}

void AI::Start()
{
    qDebug() << "AI::Start";
    timer.start(AI_TIMER);
}

void AI::Stop()
{
    qDebug() << "AI::Stop";

}

void AI::timer_timeout()
{
    static int a = 0;
    RobotCommand rc;
    rc.maxSpeed = 5;

    qDebug() << a;
    switch (a)
    {
    case 0:
        rc.fin_pos.loc = {0,-2000};
        break;
    case 1:
        rc.fin_pos.loc = {0,2000};
        break;
    case 2:
        rc.fin_pos.loc = {2000,2000};
        break;
    case 3:
        rc.fin_pos.loc = {2000,-2000};
        break;
    }

    if((wm->ourRobot[0].pos.loc - rc.fin_pos.loc).length2() < 1000) a = (a+1) % 2;

    if(wm->ourRobot[0].isValid)
        wm->ourRobot[0].SendCommand(rc);
}
