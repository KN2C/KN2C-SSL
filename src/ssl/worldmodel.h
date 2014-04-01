#ifndef WORLDMODEL_H
#define WORLDMODEL_H

#include <QList>
#include <QDebug>
#include "constants.h"
#include "base.h"
#include "ball.h"
#include "robot.h"
#include "agent.h"
#include "geom.h"
#include "game_state.h"
#include "ref_protocol.h"
#include "knowledge.h"

class WorldModel
{
public:
    explicit WorldModel(OutputBuffer *outputBuffer);
    double time;
    Ball ball;
    Agent ourRobot[PLAYERS_MAX_NUM];
    Robot oppRobot[PLAYERS_MAX_NUM];
    GameStatePacket refgs;
    GameStateType gs;
    GameStateType gs_last;
    GameState cmgs;
    bool isSim;
    Knowledge *kn;
};

#endif // WORLDMODEL_H
