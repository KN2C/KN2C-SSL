#ifndef MOBILEOBJECT_H
#define MOBILEOBJECT_H

#include <QDebug>
#include <QTimer>
#include <vector>
using namespace std;

#include "geom.h"
#include "position.h"

class MobileObject : public QObject
{
    Q_OBJECT

public:
    explicit MobileObject();
    void seenAt(vector<Position> p, double t, int camera);

    double time;
    bool isValid;
    Position pos;
    Position vel;

protected:
    Position last_pos;
    double last_time;

private:
    QTimer timer;
    int    timer_interval;
    QTimer timer_vel;
    int    timer_vel_interval;

private slots:
    void timer_timeout();
    void timer_vel_timeout();

};

#endif // MOBILEOBJECT_H
