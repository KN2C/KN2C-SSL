#ifndef PLAY_H
#define PLAY_H

#include <QObject>
#include "worldmodel.h"
#include "tactics.h"

class Play : public QObject
{
    Q_OBJECT
public:
    explicit Play(WorldModel *worldmodel, QObject *parent = 0);
    virtual Tactic* getTactic(int id) = 0;
    virtual int enterCondition() = 0;

protected:
    WorldModel *wm;

};

#endif // PLAY_H
