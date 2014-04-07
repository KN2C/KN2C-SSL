#include "ai.h"
#include "play/playcalibration.h"
#include "play/playfreekickopp.h"
#include "play/playfreekickour.h"
#include "play/playgameon.h"
#include "play/playgameondefensive.h"
#include "play/playhalt.h"
#include "play/playkickoffopp.h"
#include "play/playkickoffour.h"
#include "play/playpenaltyopp.h"
#include "play/playpenaltyour.h"
#include "play/playstop.h"
#include "play/playtest.h"

AI::AI(WorldModel *worldmodel, OutputBuffer *outputbuffer, QObject *parent) :
    QObject(parent),
    wm(worldmodel),
    outputbuffer(outputbuffer)
{
    qDebug() << "AI Initialization...";
    connect(&timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));

    plays.append(new PlayCalibration(wm));
    plays.append(new PlayFreeKickOpp(wm));
    plays.append(new PlayFreeKickOur(wm));
    plays.append(new PlayGameOn(wm));
    plays.append(new PlayGameOnDefensive(wm));
    plays.append(new PlayHalt(wm));
    plays.append(new PlayKickoffOpp(wm));
    plays.append(new PlayKickoffOur(wm));
    plays.append(new PlayPenaltyOpp(wm));
    plays.append(new PlayPenaltyOur(wm));
    plays.append(new PlayStop(wm));
    plays.append(new PlayTest(wm));
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
    int max_i = 0;
    int max_p = 0;

    for(int i=0; i<plays.size(); i++)
    {
        int p = plays[i]->enterCondition();
        if(p > max_p)
        {
            max_i = i;
            max_p = p;
        }
    }

    Play *play = plays[max_i];
    play->execute();

    for(int i=0; i<PLAYERS_MAX_NUM; i++)
    {
        Tactic *tactic = play->getTactic(i);
        if(tactic == NULL) continue;
        tactic->setID(i);
        RobotCommand rc = tactic->getCommand();
        wm->ourRobot[i].SendCommand(rc);
    }
}
