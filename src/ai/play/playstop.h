#ifndef PLAYSTOP_H
#define PLAYSTOP_H

#include "play.h"

class PlayStop : public Play
{
    Q_OBJECT
public:
    explicit PlayStop(WorldModel *worldmodel, QObject *parent = 0);
    virtual void execute();
    //virtual Tactic* getTactic(int id);
    virtual int enterCondition();

private:
    TacticGoalie* t1;
    TacticDefender* t2;
    TacticDefender* t3;

};

#endif // PLAYSTOP_H
