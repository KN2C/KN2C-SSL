#ifndef PLAY_H
#define PLAY_H

#include <QObject>
#include "worldmodel.h"
#include "tactic.h"

class Play : public QObject
{
    Q_OBJECT
public:
    explicit Play(WorldModel *worldmodel, QObject *parent = 0);
    virtual Tactic* getTactic(int id) = 0;

private:
    WorldModel *wm;

};

#endif // PLAY_H
