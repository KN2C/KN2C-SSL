#include "playtest2.h"

PlayTest2::PlayTest2(WorldModel *worldmodel, QObject *parent) :
    Play("PlayTest2", worldmodel, parent)
{
    // Goaler.
    t = new TacticTest(wm);
 }

int PlayTest2::enterCondition()
{
    return 100000000;
}

void PlayTest2::execute()
{
    tactics[wm->ref_goalie_our] = t;
    tactics[4] = t;

}

