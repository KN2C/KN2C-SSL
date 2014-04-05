#include "play.h"

Play::Play(WorldModel *worldmodel, QObject *parent) :
    QObject(parent)
{
    wm = worldmodel;
    for(int i=0; i<PLAYERS_MAX_NUM; i++)
        tactics[i] = NULL;
}

Tactic* Play::getTactic(int id)
{
    if(id >= PLAYERS_MAX_NUM || id < 0) return NULL;
    return tactics[id];
}
