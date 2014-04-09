#ifndef PLAYHALT_H
#define PLAYHALT_H

#include "play.h"

class PlayHalt : public Play
{
    Q_OBJECT
public:
    explicit PlayHalt(WorldModel *worldmodel, QObject *parent = 0);
    virtual void execute();
    virtual Tactic* getTactic(int id);
    virtual int enterCondition();

private:
    TacticHalt *tHalt;

};

#endif // PLAYHALT_H
