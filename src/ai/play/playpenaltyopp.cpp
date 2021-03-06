#include "playpenaltyopp.h"

PlayPenaltyOpp::PlayPenaltyOpp(WorldModel *worldmodel, QObject *parent) :
    Play("PlayPenaltyOpp", worldmodel, parent)
{
    t1 = new TacticGoalie(wm);
    t1->setID(0);

    t2 = new TacticDefender(wm);
    t2->setID(1);
    t2->setDefenderID(2, 0);

    t3 = new TacticDefender(wm);
    t3->setID(2);
    t3->setDefenderID(2, 1);
}

int PlayPenaltyOpp::enterCondition()
{
    if(wm->cmgs.theirPenaltyKick())
        return 100;
    else
        return 0;
}

void PlayPenaltyOpp::execute()
{
    tactics[0] = t1;
    tactics[1] = t2;
    tactics[2] = t3;
}
