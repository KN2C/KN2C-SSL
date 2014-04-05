#ifndef TACTICATTACKER_H
#define TACTICATTACKER_H

#include "tactic.h"

class TacticAttacker : public Tactic
{
    Q_OBJECT
public:
    explicit TacticAttacker(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand();
    void setAttackerID(int total, int current);

protected:
    int att_t;
    // 0= Left, 1= Right
    int att_c;
};

#endif // TACTICATTACKER_H
