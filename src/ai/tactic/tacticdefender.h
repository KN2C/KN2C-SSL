#ifndef TACTICDEFENDER_H
#define TACTICDEFENDER_H

#include "tactic.h"

class TacticDefender : public Tactic
{
    Q_OBJECT
public:
    explicit TacticDefender(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand();
    void setDefenderID(int total, int current);

protected:
    int def_t;
    // 0= Left, 1= Right, 2= Mid
    int def_c;

};

#endif // TACTICDEFENDER_H
