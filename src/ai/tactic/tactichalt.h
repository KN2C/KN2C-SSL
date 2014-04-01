#ifndef TACTICHALT_H
#define TACTICHALT_H

#include "tactic.h"

class TacticHalt : public Tactic
{
    Q_OBJECT
public:
    explicit TacticHalt(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand();

private:

};

#endif // TACTICHALT_H
