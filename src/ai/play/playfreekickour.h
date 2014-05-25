#ifndef PLAYFREEKICKOUR_H
#define PLAYFREEKICKOUR_H

#include "play.h"

class PlayFreeKickOur : public Play
{
    Q_OBJECT
public:
    explicit PlayFreeKickOur(WorldModel *worldmodel, QObject *parent = 0);
    virtual void execute();
    //virtual Tactic* getTactic(int id);
    virtual int enterCondition();

private:
    TacticGoalie*   tGolie;
    TacticFixedPos* tFixedPosM;
    TacticFixedPos* tFixedPosL;
    TacticFixedPos* tFixedPosR;
    TacticDefender* tDefenderLeft;
    TacticDefender* tDefenderRight;
    TacticDefender* tDefenderMid;
    TacticAttacker* tAttacker;
};

#endif // PLAYFREEKICKOUR_H
