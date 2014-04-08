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
    GameStateType gs;
    GameStateType gs_last;
    GameState cmgs; //referee
    int ref_goalie_our;
    int ref_goalie_opp;
    bool isSim;
    Knowledge *kn;

    static double var[10];
    // 0: kick activation dist
    // 1: kick activation angle
    // 2: kick precision angle
    // 3: prediction sliding factor
};

#endif // WORLDMODEL_H
