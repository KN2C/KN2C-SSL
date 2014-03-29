#include "playtest.h"
#include "Tactic/tactictest.h"
#include "Tactic/tacticgoalie.h"

PlayTest::PlayTest(WorldModel *worldmodel, QObject *parent) :
    Play(worldmodel, parent)
{
    t1 = new TacticGoalie(wm);
    t2 = new TacticTest(wm);
    t1->setID(1);
    t2->setID(0);
}

Tactic* PlayTest::getTactic(int id)
{
    if(id==0)
        return t2;
    if(id==1)
        return t1;
    return NULL;
}
