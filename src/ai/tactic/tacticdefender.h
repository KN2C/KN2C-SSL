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

private:
    int  ResolvePosition(Vector2D pos, double range, Vector2D *a, Vector2D *b);
    bool IsInsideDefendArea(Vector2D pos);
    bool ExtractDefendPoint(Vector2D origin, Vector2D target, Vector2D *vOut);
    Vector2D GotoDefaultLoc(int total, int current);
};

#endif // TACTICDEFENDER_H
