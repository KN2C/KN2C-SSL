#ifndef TACTICTEST_H
#define TACTICTEST_H

#include <QObject>
#include "tactic.h"

class TacticTest : public Tactic
{
    Q_OBJECT
public:
    explicit TacticTest(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand();

private:
    int a;
};

#endif // TACTICTEST_H
