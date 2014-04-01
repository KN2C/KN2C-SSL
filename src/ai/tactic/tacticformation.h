#ifndef TACTICFORMATION_H
#define TACTICFORMATION_H

#include "tactic.h"

class TacticFormation : public Tactic
{
    Q_OBJECT
public:
    explicit TacticFormation(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand();

private:

};

#endif // TACTICFORMATION_H
