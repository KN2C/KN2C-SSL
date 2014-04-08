#ifndef PLAYGAMEON_H
#define PLAYGAMEON_H

#include "play.h"

class PlayGameOn : public Play
{
    Q_OBJECT
public:
    explicit PlayGameOn(WorldModel *worldmodel, QObject *parent = 0);
    virtual void execute();
    //virtual Tactic* getTactic(int id);
    virtual int enterCondition();

private:
    TacticGoalie*   tGolie;
    TacticDefender* tDefenderLeft;
    TacticDefender* tDefenderRight;
    TacticDefender* tDefenderMid;
    TacticBlocker*  tBlocker;
    TacticAttacker* tAttackerLeft;
    TacticAttacker* tAttackerRight;
    TacticAttacker* tAttackerMid;
};

#endif // PLAYGAMEON_H
