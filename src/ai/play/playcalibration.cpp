#include "playcalibration.h"

PlayCalibration::PlayCalibration(WorldModel *worldmodel, QObject *parent) :
    Play(worldmodel, parent)
{
    //t0 = new TacticTestKickRange(wm);
    //t0->setID(0);

    //t1 = new TacticTestKickPrecision(wm);
    //t1->setID(0);

    t2 = new TacticTestFriction(wm);
    t2->setID(0);
}

int PlayCalibration::enterCondition()
{
    return 100;
}

void PlayCalibration::execute()
{
    tactics[0] = t2;
}
