#ifndef TACTICGOALIE_H
#define TACTICGOALIE_H

#include "tactic.h"

class TacticGoalie : public Tactic
{
    Q_OBJECT
public:
    explicit TacticGoalie(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand();

private:
    void LimitToGoalArea(Vector2D *pos);
};

#endif // TACTICGOALIE_H
