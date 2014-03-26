#ifndef CONTROLLER_STRUCT_H
#define CONTROLLER_STRUCT_H

#include <QObject>
#include <QtCore>
#include <QDebug>
#include <QMap>
#include <math.h>
#include <cmath>
using namespace std;

#include "position.h"
#include "util.h"

struct ControllerInput
{
    Position cur_pos;
    Position cur_vel;

    Position mid_pos;
    Position mid_vel;

    Position fin_pos;
    Position fin_vel;
    double   fin_dist;

    char angleMode;
    double maxSpeed;
};

struct RobotSpeed
{
    int RID;
    float VX;
    float VY;
    float VW;
};

struct MotorSpeed
{
    float M0;
    float M1;
    float M2;
    float M3;
};

struct ControllerResult
{
    RobotSpeed rs;
    MotorSpeed msR;
    MotorSpeed msS;
};


#endif // CONTROLLER_STRUCT_H
