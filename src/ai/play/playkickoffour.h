#ifndef PLAYKICKOFFOUR_H
#define PLAYKICKOFFOUR_H

#include "play.h"

class PlayKickoffOur : public Play
{
    Q_OBJECT
public:
    explicit PlayKickoffOur(WorldModel *worldmodel, QObject *parent = 0);
    virtual Tactic* getTactic(int id);
    void setCanKick(bool ck) { _cankick=ck; }

protected:
    bool _cankick;

    TacticGoalie* t1;
    TacticDefender* t2;
    TacticDefender* t3;

/*
private:
    bool BeforeNormal();
    bool AfterNormal();
*/

};
#endif // PLAYKICKOFFOUR_H
