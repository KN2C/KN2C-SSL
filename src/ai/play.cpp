#include "play.h"

Play::Play(WorldModel *worldmodel, QObject *parent) :
    QObject(parent)
{
    wm = worldmodel;
}
