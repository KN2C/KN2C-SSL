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

void PlayTest::execute()
{
    tactics[0] = t0;
    tactics[1] = t1;
    tactics[2] = t2;
    tactics[3] = t3;
    tactics[4] = t4;
}
