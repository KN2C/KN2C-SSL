#ifndef TACTICTESTKICKRANGE_H
#define TACTICTESTKICKRANGE_H

#include "tactic.h"

class TacticTestKickRange : public Tactic
{
    Q_OBJECT
public:
    explicit TacticTestKickRange(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand();

private:

};

#endif // TACTICTESTKICKRANGE_H
