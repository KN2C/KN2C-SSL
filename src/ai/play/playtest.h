#ifndef PLAYTEST_H
#define PLAYTEST_H

#include <QObject>
#include "play.h"

class PlayTest : public Play
{
    Q_OBJECT
public:
    explicit PlayTest(WorldModel *worldmodel, QObject *parent = 0);
    virtual Tactic* getTactic(int id);

private:
    Tactic* t1;

};

#endif // PLAYTEST_H
