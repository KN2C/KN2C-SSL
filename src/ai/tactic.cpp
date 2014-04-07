#include "tactic.h"

Tactic::Tactic(QString name, WorldModel *worldmodel, QObject *parent) :
    QObject(parent),
    name(name)
{
    wm = worldmodel;
    id = -1;
}

QString Tactic::getName()
{
    if(param.size() > 0)
        return name + " => " + param;
    else
        return name;
}

void Tactic::setID(int id)
{
    this->id = id;
}
