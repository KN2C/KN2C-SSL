#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "controller_struct.h"

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

    double ap=1;
    double am=1;
    Vector2D derived0,derived1;
    Vector2D integral;
    double Rspeed_filer;

    Vector2D err1,err0;
    Vector2D u1;

    double Kp;// = {0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5};
    double Kd;// = {50,50,50,50,50,50,50,50,50,50,50,50};
    double delT;// = {10,10,10,10,10,10,10,10,10,10,10,10};
    double Ki;// = {0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01}; //Kp/Ti

    Vector2D integral[100];
    // int Integral_CNT;
    Vector2D derived;

    double werr1;// = (ci.fin_pos.loc - ci.cur_pos.loc);
    double werr0;
    double wu1;

    double wKp;// = {0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5};
    double wKd;// = {50,50,50,50,50,50,50,50,50,50,50,50};
    double wKi;// = {0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01}; //Kp/Ti
    double wintegral[10];
    double wderived;
    int stateCTRL;

    double dist;

    Vector2D err1,err0;
    Vector2D u1;

    //  double Kp;// = {0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5};
    // double Kd;// = {50,50,50,50,50,50,50,50,50,50,50,50};
    double delT;// = {10,10,10,10,10,10,10,10,10,10,10,10};
    // double Ki;// = {0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01}; //Kp/Ti

    double t0;
    // double t1;
    double s0;
    double s3;
    double s1;
    double v,dt,s,sp,t2,vb;
    double tp;
    double t2p;
    double t3;
    Position targetVel;

    float RotationSpeed,Rspeed_filer,Rspeed_filer_1;

    double werr1;// = (ci.fin_pos.loc - ci.cur_pos.loc);
    double werr0;
    double wu1;

    double wKp;
    double wKd;
    double wKi;
    double wintegral;
    double wderived;
    int stateCTRL;

    // Vector2D integral;
    // int Integral_CNT;
    // Vector2D derived;
};

#endif // CONTROLLER_H
