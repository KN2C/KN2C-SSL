#include "playkickoffopp.h"

PlayKickoffOpp::PlayKickoffOpp(WorldModel *worldmodel, QObject *parent) :
    Play(worldmodel, parent)
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

Tactic* PlayKickoffOpp::getTactic(int id)
{
    if(id==0)
        return t1;
    if(id==1)
        return t2;
    if(id==2)
        return t3;

    return NULL;
}
