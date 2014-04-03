#ifndef TACTICTEST2_H
#define TACTICTEST2_H

#include "tactic.h"

class TacticTest2 : public Tactic
{
    Q_OBJECT
public:
    explicit TacticTest2(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand();

private:
    int a;
};

#endif // TACTICTEST2_H
