#ifndef TACTIC_H
#define TACTIC_H

#include <QObject>
#include "robotcommand.h"
#include "worldmodel.h"

class Tactic : public QObject
{
    Q_OBJECT
public:
    explicit Tactic(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand() = 0;
    void setID(int id);

private:
    WorldModel *wm;
    int id;

};

#endif // TACTIC_H
