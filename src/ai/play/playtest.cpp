#include "playtest.h"

PlayTest::PlayTest(WorldModel *worldmodel, QObject *parent) :
    Play(worldmodel, parent)
{
    t1 = new TacticGoalie(wm);
    t1->setID(0);

    t2 = new TacticTest(wm);
    t2->setID(1);

    t3 = new TacticDefender(wm);
    t3->setID(2);
    t3->setDefenderID(2, 0);

    t4 = new TacticDefender(wm);
    t4->setID(3);
    t4->setDefenderID(2, 1);
}

Tactic* PlayTest::getTactic(int id)
{
    if(id==0)
        return t1;
    if(id==1)
        return t2;
    if(id==2)
        return t3;
    if(id==3)
        return t4;

    return NULL;
}
