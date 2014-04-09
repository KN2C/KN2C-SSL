#ifndef PLAYKICKOFFOUR_H
#define PLAYKICKOFFOUR_H

#include "play.h"

class PlayKickoffOur : public Play
{
    Q_OBJECT
public:
    explicit PlayKickoffOur(WorldModel *worldmodel, QObject *parent = 0);
    virtual void execute();
    //virtual Tactic* getTactic(int id);
    virtual int enterCondition();
    void setCanKick(bool ck) { _cankick=ck; }

protected:
    bool _cankick;

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
#endif // PLAYKICKOFFOUR_H
