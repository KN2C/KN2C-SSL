#ifndef PLAYGAMEON_H
#define PLAYGAMEON_H

#include "play.h"

class PlayGameOn : public Play
{
    Q_OBJECT
public:
    explicit PlayGameOn(WorldModel *worldmodel, QObject *parent = 0);
    virtual Tactic* getTactic(int id);
    virtual int enterCondition();

private:
    TacticGoalie* t1;
    TacticDefender* t2;
    TacticDefender* t3;

};

#endif // PLAYGAMEON_H
