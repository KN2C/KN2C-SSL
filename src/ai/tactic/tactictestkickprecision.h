#ifndef TACTICTESTKICKPRECISION_H
#define TACTICTESTKICKPRECISION_H

#include "tactic.h"

class TacticTestKickPrecision : public Tactic
{
    Q_OBJECT
public:
    explicit TacticTestKickPrecision(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand();

private:

};

#endif // TACTICTESTKICKPRECISION_H
