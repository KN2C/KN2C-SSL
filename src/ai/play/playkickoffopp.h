#ifndef PLAYKICKOFFOPP_H
#define PLAYKICKOFFOPP_H

#include "play.h"

class PlayKickoffOpp : public Play
{
    Q_OBJECT
public:
    explicit PlayKickoffOpp(WorldModel *worldmodel, QObject *parent = 0);
    virtual void execute();
    //virtual Tactic* getTactic(int id);
    virtual int enterCondition();

private:
    TacticGoalie*   tGolie;
    TacticDefender* tDefenderLeft;
    TacticDefender* tDefenderRight;
    TacticDefender* tDefenderMid;
    TacticFixedPos* tFixedPosM;
    TacticFixedPos* tFixedPosL;
    TacticFixedPos* tFixedPosR;
};

#endif // PLAYKICKOFFOPP_H
