#ifndef PLAYCALIBRATION_H
#define PLAYCALIBRATION_H

#include "play.h"

class PlayCalibration : public Play
{
    Q_OBJECT
public:
    explicit PlayCalibration(WorldModel *worldmodel, QObject *parent = 0);
    virtual Tactic* getTactic(int id);

private:
    TacticTestKickRange *t0;
    TacticTestKickPrecision *t1;
    TacticTestFriction *t2;

};

#endif // PLAYCALIBRATION_H
