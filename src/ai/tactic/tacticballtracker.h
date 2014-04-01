#ifndef TACTICBALLTRACKER_H
#define TACTICBALLTRACKER_H

#include "tactic.h"

class TacticBallTracker : public Tactic
{
    Q_OBJECT
public:
    explicit TacticBallTracker(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand();
    void setFace(Vector2D face) {_face = face;}

private:
    Vector2D _face;

};

#endif // TACTICBALLTRACKER_H
