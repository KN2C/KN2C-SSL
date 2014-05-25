#ifndef AGENT_H
#define AGENT_H

#include <QObject>
#include "robot.h"
#include "robotcommand.h"
#include "navigation.h"

enum AgentStatus
{
    Idle,
    Passing,
    Kicking,
    FollowingBall,
    BlockingPass,
    BlockingRobot
};

enum AgentRole
{
    NoRole = 0,
    ArcMid,
    ArcLeft,
    ArcRight,
    FixedPositionLeft,
    FixedPositionRight,
    FixedPositionMid,
    AttackerLeft,
    AttackerRight,
    AttackerMid,
    Blocker,
    DefenderLeft,
    DefenderRight,
    DefenderMid,
    Golie
};

class WorldModel;

class Agent : public Robot
{
    Q_OBJECT
public:
    explicit Agent();
    void setID(int id);
    void setWorldModel(WorldModel *wm);
    void setOutputBuffer(OutputBuffer *outputBuffer);

    void SendCommand(RobotCommand rc);
    void Halt();

    AgentRole   Role;
    AgentStatus Status;

    RobotData rd;   //received data from robot
    Position  vel2; //speed from motors speed received from robot

private:
    int id;
    Controller ctrl;
    Navigation nav;
    OutputBuffer *outputBuffer;
    WorldModel *wm;

};

#endif // AGENT_H
