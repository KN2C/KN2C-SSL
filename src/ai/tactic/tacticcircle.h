#ifndef TACTICCIRCLE_H
#define TACTICCIRCLE_H

#include "tactic.h"

class TacticCircle : public Tactic
{
    Q_OBJECT
public:
    explicit TacticCircle(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand();
    void setCircle(Vector2D c, float r, Vector2D p) { center = c; radius = r; pointTo = p; }

private:
    Vector2D center;
    float radius;
    Vector2D pointTo;

};

#endif // TACTICCIRCLE_H
