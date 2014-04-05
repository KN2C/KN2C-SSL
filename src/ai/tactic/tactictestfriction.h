#ifndef TACTICTESTFRICTION_H
#define TACTICTESTFRICTION_H

#include "tactic.h"

class TacticTestFriction : public Tactic
{
    Q_OBJECT
public:
    explicit TacticTestFriction(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand();

private:

};

#endif // TACTICTESTFRICTION_H
