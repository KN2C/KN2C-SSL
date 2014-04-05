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
    TacticGoalie* t1;
    TacticDefender* t2;
    TacticDefender* t3;

};

#endif // PLAYFREEKICKOUR_H
