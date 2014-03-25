#ifndef AGENT_H
#define AGENT_H

#include <QObject>
#include "robot.h"
#include "robotcommand.h"
#include "navigation.h"

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

    RobotData rd;
    Position vel2;

private:
    int id;
    Controller ctrl;
    Navigation nav;
    OutputBuffer *outputBuffer;
    WorldModel *wm;

};

#endif // AGENT_H
