#include "playtest2.h"

PlayTest2::PlayTest2(WorldModel *worldmodel, QObject *parent) :
    Play("PlayTest2", worldmodel, parent)
{
    // Goaler.
    tGolie = new TacticGoalie(wm);
    tCircle = new TacticCircle(wm);
    tCircle->setCircle({0, 0}, 1000, {0, 0});
}

int PlayTest2::enterCondition()
{
    return 0;
}

void PlayTest2::execute()
{
    tactics[wm->ref_goalie_our] = tGolie;
    tactics[3] = tCircle;
}

