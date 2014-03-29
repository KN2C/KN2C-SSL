#ifndef TACTICGOALER_H
#define TACTICGOALER_H

#include <QObject>
#include "tactic.h"

class TacticGoaler : public Tactic
{
    Q_OBJECT
public:
    explicit TacticGoaler(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand();

private:
    int a;
};

#endif // TACTICGOALER_H
