#ifndef PLAYTEST_H
#define PLAYTEST_H

#include "play.h"

class PlayTest : public Play
{
    Q_OBJECT
public:
    explicit PlayTest(WorldModel *worldmodel, QObject *parent = 0);
    virtual Tactic* getTactic(int id);

private:
    TacticGoalie* t0;
    TacticDefender* t1;
    TacticDefender* t2;
    TacticAttacker* t3;
};

#endif // PLAYTEST_H
