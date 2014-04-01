#ifndef TACTICATTACKER_H
#define TACTICATTACKER_H

#include "tactic.h"

class TacticAttacker : public Tactic
{
    Q_OBJECT
public:
    explicit TacticAttacker(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand();

private:

};

#endif // TACTICATTACKER_H
