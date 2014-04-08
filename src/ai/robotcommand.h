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

    bool useNav;
    bool isBallObs;
    bool isKickObs;

    RobotCommand()
    {
        angleMode = 0;
        maxSpeed = 1;
        kickspeedx = 0;
        kickspeedz = 0;
        useNav = true;
        isBallObs = false;
        isKickObs = true;
        /*
         * ball | kick | result
         * --------------------
         *  0   |  0   | bokhor
         *  0   |  1   | az posht nakhor, az jelo bokhor.
         *  1   |  d   | nakhor
         */
    }
};

#endif // ROBOTCOMMAND_H
