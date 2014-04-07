#include "play.h"

Play::Play(QString name, WorldModel *worldmodel, QObject *parent) :
    QObject(parent),
    name(name)
{
    wm = worldmodel;
    for(int i=0; i<PLAYERS_MAX_NUM; i++)
        tactics[i] = NULL;
}

QString Play::getName()
{
    return name;
}

Tactic* Play::getTactic(int id)
{
    if(id >= PLAYERS_MAX_NUM || id < 0) return NULL;
    return tactics[id];
}
