#ifndef PLAYTEST2_H
#define PLAYTEST2_H

#include "play.h"

class PlayTest2 : public Play
{
    Q_OBJECT
public:
    explicit PlayTest2(WorldModel *worldmodel, QObject *parent = 0);
    virtual void execute();
    //virtual Tactic* getTactic(int id);
    virtual int enterCondition();

private:
    TacticGoalie* tGolie;
    TacticCircle* tCircle;
};

#endif // PLAYTEST2_H
