#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <QObject>
#include "controller.h"
#include "robotcommand.h"

class WorldModel;

class Navigation : public QObject
{
    Q_OBJECT
public:
    explicit Navigation(QObject *parent = 0);
    ControllerInput calc(RobotCommand rc);
    void setID(int id);
    void setWorldModel(WorldModel *wm);

public:
    double getPath(RobotCommand rc, QList<Vector2D> *points = 0);

private:
    int id;
    WorldModel *wm;

};

#endif // NAVIGATION_H
