#ifndef PLAYFREEKICKOPP_H
#define PLAYFREEKICKOPP_H

#include "play.h"

class PlayFreeKickOpp : public Play
{
    Q_OBJECT
public:
    explicit PlayFreeKickOpp(WorldModel *worldmodel, QObject *parent = 0);
    virtual void execute();
    //virtual Tactic* getTactic(int id);
    virtual int enterCondition();

private:
    TacticGoalie*   tGolie;
    TacticDefender* tDefenderLeft;
    TacticDefender* tDefenderRight;
    TacticDefender* tDefenderMid;
    TacticBlocker*  tBlocker;
    TacticFixedPos* tFixedPosM;
    TacticFixedPos* tFixedPosL;
    TacticFixedPos* tFixedPosR;
};

#endif // PLAYFREEKICKOPP_H
