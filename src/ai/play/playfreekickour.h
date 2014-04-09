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
    TacticFixedPos* tFixedPos;
    TacticDefender* tDefenderLeft;
    TacticDefender* tDefenderRight;
    TacticDefender* tDefenderMid;
    TacticBlocker*  tBlocker;
    TacticAttacker* tAttackerLeft;
    TacticAttacker* tAttackerRight;
    TacticAttacker* tAttackerMid;

};

#endif // PLAYFREEKICKOUR_H
