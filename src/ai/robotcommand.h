#ifndef ROBOTCOMMAND_H
#define ROBOTCOMMAND_H

#include "position.h"

struct RobotCommand
{
    Position fin_pos;
    Position fin_vel;

    char angleMode;
    double maxSpeed;

    float kickspeedx;
    float kickspeedz;

    bool isBallObs;
    bool useNav;

    RobotCommand()
    {
        angleMode = 0;
        maxSpeed = 0;
        kickspeedx = 0;
        kickspeedz = 0;
        isBallObs = false;
        useNav = true;
    }
};

#endif // ROBOTCOMMAND_H
