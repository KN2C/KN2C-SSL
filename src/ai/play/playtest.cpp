#include "playtest.h"
#include "Tactic/tactictest.h"

PlayTest::PlayTest(WorldModel *worldmodel, QObject *parent) :
    Play(worldmodel, parent)
{
    t1 = new TacticTest(wm);
    t1->setID(0);
}

Tactic* PlayTest::getTactic(int id)
{
    return t1;
}
