#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "geom.h"

// global constants
#define PLAYERS_MAX_NUM 12
#define FIELD_MAX_X     3250
#define FIELD_MAX_Y     2250
#define BALL_RADIUS     22
#define ROBOT_RADIUS    90

// Vision constants
#define CAMERA_NUM      2
#define VOBJ_MAX_NUM    5
#define MIN_CONF        0.1
#define VISION_TIMEDOUT 1000.0
#define MAX_RADIOUS     2000


// Timer speeds
#define TRANSMITTER_TIMER   50
#define AI_TIMER            40
#define AGENT_TIMER         20
#define COACH_TIMER         40

//public static int PLAYER_THREAD_SLEEP = 30;
//public static int COACH_THREAD_SLEEP = 20;
//public static int TRACKER_THREAD_SLEEP = 10;

namespace Field
{
const double MaxX = 3025;
const double MinX = -3025;
const double MaxY = 2025;
const double MinY = -2025;

const Vector2D upperLeftCorner(MinX, MaxY);
const Vector2D upperRightCorner(MaxX, MaxY);
const Vector2D bottomLeftCorner(MinX, MinY);
const Vector2D bottomRightCorner(MaxX, MinY);

const Line2D leftLine(Vector2D(MinX,MinY), Vector2D(MinX, MaxY));
const Line2D midLine(Vector2D(0,MinY), Vector2D(0, MaxY));
const Line2D rightLine(Vector2D(MaxX, MinY), Vector2D(MaxX, MaxY));

const Line2D upLine(Vector2D(MinX, MaxY), Vector2D(MaxX, MaxY));
const Line2D bottomLine(Vector2D(MinX, MinY), Vector2D(MaxX, MinY));

const double centerCircle_R = 500;
const double goalCircle_R = 800;
const double goalCircleEX_R = 800 + ROBOT_RADIUS + 60;
const double goalCircleDEF_R = goalCircleEX_R + 400;

// Ours.
const Vector2D ourGoalCenter(MinX, 0);
const Vector2D ourGoalPost_L(MinX, 350);
const Vector2D ourGoalPost_R(MinX, -350);
const Vector2D ourGoalCC_L(MinX, 175);
const Vector2D ourGoalCC_R(MinX, -175);

const Vector2D ourDefPost_L(MinX, 975);
const Vector2D ourDefPost_R(MinX, -975);

// Opps.
const Vector2D oppGoalCenter(MaxX, 0);
const Vector2D oppGoalPost_L(MaxX, 350);
const Vector2D oppGoalPost_R(MaxX, -350);
const Vector2D oppGoalCC_L(MaxX, 175);
const Vector2D oppGoalCC_R(MaxX, -175);

//const Vector2D oppPenalty_Shoot = new Vector2D(3010, 500);

//const Vector2D MidField_R = new Vector2D((-ConstVars.ROBOT_RADIUS)*2.5, -1800);
//const Vector2D MidField_L = new Vector2D((-ConstVars.ROBOT_RADIUS)*2.5, 1800);

//const Vector2D P1(-2510, 175);
//const Vector2D P2(-2510, -175);

//public static double BALL_OBSTACLE = ROBOT_RADIUS * 2;
//public static double PASS_RADIUS = 2000;

//public static double MAX_KICKABLE_DISTANCE = ROBOT_RADIUS + 30;
//public static double MAX_KICKABLE_ANGLE = 20*System.Math.PI/180;

//public static double HEADING_ANGLE_TRESHOLD = System.Math.PI/3;
//public static double MAX_AHEAD_OPP_DISTANCE = 200;
}

#endif // CONSTANTS_H
