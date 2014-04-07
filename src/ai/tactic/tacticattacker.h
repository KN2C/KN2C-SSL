#ifndef TACTICATTACKER_H
#define TACTICATTACKER_H

#include "tactic.h"
#include "geom2/shape2d.h"

class TacticAttacker : public Tactic
{
    Q_OBJECT
public:
    explicit TacticAttacker(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand();
    void setAttackerID(int total, int current);    

    Shape2D WorkingArea;

protected:
    int att_t;
    // 0 = Left, 1 = Right, 2 = Mid
    int att_c;

private:
    bool CheckSweeping(int kickerID, double y, bool clearState = false);
    Vector2D GotoDefaultLoc(int total, int current);
    uint CastRayToGoal(Vector2D origin, double offsetStep, uint maxCount, Vector2D *vOut, double beamWidth = 0);
    Vector2D ScanFieldFreePos(Vector2D pos, double gridSize = 400, double localRange = 500, double castStep = 2 * BALL_RADIUS);
    Vector2D DetectScanPos(int passerRobotID);
    Vector2D CalculatePassTarget(Vector2D sourcePos, Vector2D destPos);
};

#endif // TACTICATTACKER_H
