#ifndef MOBILEOBJECT_H
#define MOBILEOBJECT_H

#include <QDebug>
#include <QTimer>
#include <vector>
using namespace std;

#include "geom.h"
#include "position.h"

#define LAST_COUNT 4

struct PositionTimeCamera
{
    Position pos;
    double time;
    int camera;
};

class MobileObject : public QObject
{
    Q_OBJECT

public:
    explicit MobileObject();
    void seenAt(vector<Position> p, double t, int c);

    bool isValid;

    Position pos;
    double time;
    int camera;

    Position vel;

protected:
    PositionTimeCamera last_postc[LAST_COUNT];
    void appendPostc(PositionTimeCamera &postc);

private:
    QTimer timer_seen;
    int    timer_seen_interval;
    QTimer timer_vel;
    int    timer_vel_interval;

private slots:
    void timer_seen_timeout();
    void timer_vel_timeout();

};

#endif // MOBILEOBJECT_H
