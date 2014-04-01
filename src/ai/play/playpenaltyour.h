#ifndef PLAYPENALTYOUR_H
#define PLAYPENALTYOUR_H

#include "play.h"

class PlayPenaltyOur : public Play
{
    Q_OBJECT
public:
    explicit PlayPenaltyOur(WorldModel *worldmodel, QObject *parent = 0);
    virtual Tactic* getTactic(int id);
    void setCanKick(bool ck) { _cankick=ck; }

protected:
    bool _cankick;

    TacticGoalie* t1;
    TacticDefender* t2;
    TacticDefender* t3;

private:


};

#endif // PLAYPENALTYOUR_H
