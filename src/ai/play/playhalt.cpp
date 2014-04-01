#include "playhalt.h"

PlayHalt::PlayHalt(WorldModel *worldmodel, QObject *parent) :
    Play(worldmodel, parent)
{

}

Tactic* PlayHalt::getTactic(int id)
{
    return NULL;
}
