#include "tactic.h"

Tactic::Tactic(WorldModel *worldmodel, QObject *parent) :
    QObject(parent)
{
    wm = worldmodel;
    id = -1;
}

void Tactic::setID(int id)
{
    this->id = id;
}
