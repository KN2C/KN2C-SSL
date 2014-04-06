#ifndef TACTICATTACKER_H
#define TACTICATTACKER_H

#include "tactic.h"

class TacticAttacker : public Tactic
{
    Q_OBJECT
public:
    explicit TacticAttacker(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand();
    void setAttackerID(int total, int current);

protected:
    int att_t;
    // 0= Left, 1= Right
    int att_c;

private:
    bool CheckSweeping(int kickerID, double y, bool clearState = false);
    Vector2D GotoDefaultLoc(int total, int current);
    uint CastRayToGoal(Vector2D origin, const Robot *oppRobots, double offsetStep, uint maxCount, Vector2D *vOut, double beamWidth = 0);
    Vector2D ScanFieldFreePos(Vector2D pos, const Robot *oppRobots, double gridSize = 400, double localRange = 750, double castStep = 40);
};

#endif // TACTICATTACKER_H
