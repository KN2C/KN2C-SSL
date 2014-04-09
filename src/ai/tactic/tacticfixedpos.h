#ifndef TACTICFIXEDPOS_H
#define TACTICFIXEDPOS_H

#include "tactic.h"

class TacticFixedPos : public Tactic
{
    Q_OBJECT
public:
    explicit TacticFixedPos(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand();

    void SetPosition(Position p);

private:
    Position destination;
};

#endif // TACTICFIXEDPOS_H
