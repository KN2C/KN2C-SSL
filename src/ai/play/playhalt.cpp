#include "playhalt.h"

PlayHalt::PlayHalt(WorldModel *worldmodel, QObject *parent) :
    Play("PlayHalt", worldmodel, parent)
{

}

int PlayHalt::enterCondition()
{
    return 0;
}

void PlayHalt::execute()
{

}

Tactic* PlayHalt::getTactic(int id)
{
    return NULL;
}
