#include "playcalibration.h"

PlayCalibration::PlayCalibration(WorldModel *worldmodel, QObject *parent) :
    Play("PlayCalibration", worldmodel, parent)
{
    //t0 = new TacticTestKickRange(wm);
    //t0->setID(0);

    //t1 = new TacticTestKickPrecision(wm);
    //t1->setID(0);

//    t2 = new TacticTestFriction(wm);
//    t2->setID(0);

    tt = new TacticTechnical(wm);
}

int PlayCalibration::enterCondition()
{
    return 0;
}

void PlayCalibration::execute()
{
    tactics[4] = tt;
}
