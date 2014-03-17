#ifndef AGENT_H
#define AGENT_H

#include <QObject>
#include "robot.h"
#include "robotcommand.h"

class WorldModel;

class Agent : public Robot
{
    Q_OBJECT
public:
    explicit Agent();
    void setID(int id);
    void setOutputBuffer(OutputBuffer *outputBuffer);
    void setWorldModel(WorldModel *wm);

    void SendCommand(RobotCommand rc);
    void Halt();

    RobotData rd;

private:
    int id;
    Controller ctrl;
    OutputBuffer *outputBuffer;
    WorldModel *wm;

};

#endif // AGENT_H
