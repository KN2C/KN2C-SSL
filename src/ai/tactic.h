#ifndef TACTIC_H
#define TACTIC_H

#include <QObject>
#include "robotcommand.h"
#include "worldmodel.h"

class Tactic : public QObject
{
    Q_OBJECT
public:
    explicit Tactic(QString name, WorldModel *worldmodel, QObject *parent = 0);
    QString getName();
    virtual RobotCommand getCommand() = 0;
    void setID(int id);

protected:
    WorldModel *wm;
    QString param;
    int id;

private:
    QString name;

};

#endif // TACTIC_H
