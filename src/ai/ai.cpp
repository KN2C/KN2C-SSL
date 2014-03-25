#include "ai.h"
#include "play/playtest.h"

AI::AI(WorldModel *worldmodel, OutputBuffer *outputbuffer, QObject *parent) :
    QObject(parent),
    wm(worldmodel),
    outputbuffer(outputbuffer)
{
    qDebug() << "AI Initialization...";
    connect(&timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));

    play = new PlayTest(wm);
}

void AI::Start()
{
    qDebug() << "AI::Start";
    timer.start(AI_TIMER);
}

void AI::Stop()
{
    qDebug() << "AI::Stop";
    timer.stop();
}

void AI::timer_timeout()
{
    for(int i=0; i<PLAYERS_MAX_NUM; i++)
    {
        Tactic *tactic = play->getTactic(i);
        if(tactic == NULL) continue;
        RobotCommand rc = tactic->getCommand();
        wm->ourRobot[i].SendCommand(rc);
    }
}
