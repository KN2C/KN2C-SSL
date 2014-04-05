#ifndef PLAYPENALTYOPP_H
#define PLAYPENALTYOPP_H

#include "play.h"

class PlayPenaltyOpp : public Play
{
    Q_OBJECT
public:
    explicit PlayPenaltyOpp(WorldModel *worldmodel, QObject *parent = 0);
    virtual void execute();
    //virtual Tactic* getTactic(int id);
    virtual int enterCondition();

private:
    TacticGoalie* t1;
    TacticDefender* t2;
    TacticDefender* t3;

};

#endif // PLAYPENALTYOPP_H
