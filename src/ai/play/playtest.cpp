#include "playtest.h"

PlayTest::PlayTest(WorldModel *worldmodel, QObject *parent) :
    Play(worldmodel, parent)
{
    t0 = new TacticGoalie(wm);
    t0->setID(0);

    // Left defender.
    t1 = new TacticDefender(wm);
    t1->setID(1);
    t1->setDefenderID(2, 0);

    // Right defender.
    t2 = new TacticDefender(wm);
    t2->setID(2);
    t2->setDefenderID(2, 1);

    // Left attacker.
    t3 = new TacticAttacker(wm);
    t3->setID(3);
    t3->setAttackerID(2, 0);

    // Right attacker.
    t4 = new TacticAttacker(wm);
    t4->setID(4);
    t4->setAttackerID(2, 1);
}

int PlayTest::enterCondition()
{
    return 0;
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
    if(id==4)
        return t4;

    return NULL;
}
