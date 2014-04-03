#include "playtest.h"

PlayTest::PlayTest(WorldModel *worldmodel, QObject *parent) :
    Play(worldmodel, parent)
{
    t0 = new TacticGoalie(wm);
    t0->setID(0);

    t1 = new TacticDefender(wm);
    t1->setID(1);
    t1->setDefenderID(2, 0);

    t2 = new TacticDefender(wm);
    t2->setID(2);
    t2->setDefenderID(2, 1);

    t3 = new TacticAttacker(wm);
    t3->setID(3);
}

Tactic* PlayTest::getTactic(int id)
{
    if(id==0)
        return t0;
    if(id==1)
        return t1;
    if(id==2)
        return t2;
    if(id==3)
        return t3;

    return NULL;
}
