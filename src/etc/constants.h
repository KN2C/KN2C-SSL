#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "geom.h"

// global constants
#define PLAYERS_MAX_NUM 12
#define FIELD_MAX_X 3250
#define FIELD_MAX_Y 2250
#define BALL_RADIUS 22
#define ROBOT_RADIUS 90
//public static double MaxX = 3010;
//public static double MinX = -1500;
//public static double MaxY = 2025;
//public static double MinY = -2025;


// Vision constants
#define CAMERA_NUM 2
#define VOBJ_MAX_NUM 5
#define MIN_CONF 0.1
#define VISION_TIMEDOUT 1000.0
#define MAX_RADIOUS 2000


// Timer speeds
#define TRANSMITTER_TIMER 50
#define AI_TIMER 40
#define AGENT_TIMER 20
#define COACH_TIMER 40

//public static int PLAYER_THREAD_SLEEP = 30;
//public static int COACH_THREAD_SLEEP = 20;
//public static int TRACKER_THREAD_SLEEP = 10;

namespace Field
{
const Line2D leftLine(Vector2D(-3025,-2025), Vector2D(-3025, 2025));
const Line2D midLine(Vector2D(0,-2025), Vector2D(0, 2025));
const Line2D rightLine(Vector2D(3025,-2025), Vector2D(3025, 2025));

const Line2D upLine(Vector2D(-3025, 2025), Vector2D(3025, 2025));
const Line2D bottomLine(Vector2D(-3025,-2025), Vector2D(3025, -2025));

const double centerCircle_R = 500;
const double goalCircle_R = 800;
const double goalCircleEX_R = 800 + ROBOT_RADIUS + 60;

const Vector2D ourGoalCenter(-3025, 0);
const Vector2D ourGoalPost_L(-3025, 350);
const Vector2D ourGoalPost_R(-3025, -350);
const Vector2D ourGoalCC_L(-3025, 175);
const Vector2D ourGoalCC_R(-3025, -175);
//const Vector2D ourGoalFront_R(-3025, -175);

const Vector2D ourDef_L(-3025, 975);
const Vector2D ourDef_R(-3025, -975);

//const Vector2D oppGoal(3025, 0);
//const Vector2D center(0, 0);
//const Vector2D oppPenalty(2560, 0);
//const Vector2D ourPenalty(-2560, 0);


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
