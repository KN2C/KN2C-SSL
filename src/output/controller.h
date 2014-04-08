#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "controller_struct.h"
#include <fstream>
using namespace std;

class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(QObject *parent = 0);
    ControllerResult calc(ControllerInput &ci);

private:
    QTime timer;

    RobotSpeed calcRobotSpeed_main(ControllerInput &ci);
    RobotSpeed calcRobotSpeed_adjt(ControllerInput &ci);
    RobotSpeed calcRobotSpeed_test(ControllerInput &ci);

    MotorSpeed calcReal(RobotSpeed rs);
    MotorSpeed calcSimul(RobotSpeed rs);

private:

    Vector2D err0,err1;
    Vector2D u1;
    Vector2D derived0,derived1;
    Vector2D integral;

    double wu1,wintegral,werr0,werr1;
    double wderived0,wderived1;

    int stateCTRL;

    ofstream out;
};

#endif // CONTROLLER_H
