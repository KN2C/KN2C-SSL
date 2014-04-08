#ifndef TACTICBLOCKER_H
#define TACTICBLOCKER_H

#include "tactic.h"
#include "geom2/shape2d.h"

class TacticBlocker : public Tactic
{
    Q_OBJECT
public:
    explicit TacticBlocker(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand();
};

#endif // TACTICBLOCKER_H
