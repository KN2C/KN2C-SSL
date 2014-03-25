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
    static Vector2D derived0,derived1;
    static Vector2D integral;
    static double Rspeed_filer;

    static Vector2D err1,err0;
    static Vector2D u1;

    static double Kp;// = {0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5};
    static double Kd;// = {50,50,50,50,50,50,50,50,50,50,50,50};
    static double delT;// = {10,10,10,10,10,10,10,10,10,10,10,10};
    static double Ki;// = {0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01}; //Kp/Ti

    static Vector2D integral[100];
    //static int Integral_CNT;
    static Vector2D derived;

    static double werr1;// = (ci.fin_pos.loc - ci.cur_pos.loc);
    static double werr0;
    static double wu1;

    static double wKp;// = {0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5};
    static double wKd;// = {50,50,50,50,50,50,50,50,50,50,50,50};
    static double wKi;// = {0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01}; //Kp/Ti
    static double wintegral[10];
    static double wderived;
    static int stateCTRL;

    static double dist;

    static Vector2D err1,err0;
    static Vector2D u1;

    // static double Kp;// = {0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5};
    //static double Kd;// = {50,50,50,50,50,50,50,50,50,50,50,50};
    static double delT;// = {10,10,10,10,10,10,10,10,10,10,10,10};
    //static double Ki;// = {0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01}; //Kp/Ti

    static double t0;
    //static double t1;
    static double s0;
    static double s3;
    static double s1;
    static double v,dt,s,sp,t2,vb;
    static double tp;
    static double t2p;
    static double t3;
    static Position targetVel;

    static float RotationSpeed,Rspeed_filer,Rspeed_filer_1;

    static double werr1;// = (ci.fin_pos.loc - ci.cur_pos.loc);
    static double werr0;
    static double wu1;

    static double wKp;
    static double wKd;
    static double wKi;
    static double wintegral;
    static double wderived;
    static int stateCTRL;

    //static Vector2D integral;
    //static int Integral_CNT;
    //static Vector2D derived;
};

#endif // CONTROLLER_H
