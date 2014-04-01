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
    TacticGoalie* t1;
    TacticTest* t2;
    TacticDefender* t3;
    TacticDefender* t4;
};

#endif // PLAYTEST_H
