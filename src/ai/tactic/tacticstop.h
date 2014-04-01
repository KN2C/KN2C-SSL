#ifndef TACTICSTOP_H
#define TACTICSTOP_H

#include "tactic.h"

class TacticStop : public Tactic
{
    Q_OBJECT
public:
    explicit TacticStop(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand();
    void setStopID(int total, int current);

protected:
    int stop_t;
    int stop_c;

};

#endif // TACTICSTOP_H
