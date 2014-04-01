#include "tacticgoalie.h"

static bool IsInsideGolieArea(Vector2D pos)
{
    double x = (pos.x - Field::ourGoalCenter.x);

    return (pos.dist(Field::ourGoalCC_L)<=Field::goalCircle_R ||
       pos.dist(Field::ourGoalCC_R)<=Field::goalCircle_R ||
       (x >= 0 && x <= Field::goalCircle_R && fabs(pos.y) <=
        Field::ourGoalCC_L.dist(Field::ourGoalCC_R) / 2));
}

TacticGoalie::TacticGoalie(WorldModel *worldmodel, QObject *parent) :
    Tactic(worldmodel, parent)
{
}

RobotCommand TacticGoalie::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;

    // There is a ball in the field.
    if(wm->ball.isValid)
    {
        // Ball is not inside golie area.
        if(!IsInsideGolieArea(wm->ball.pos.loc))
        {
            int attackerID = -1;
            double attackerDist = 1000000;
            for(int i = 0; i < PLAYERS_MAX_NUM; ++i)
            {
                if(wm->oppRobot[i].isValid)
                {
                    double dist;
                    if((dist = wm->oppRobot[i].pos.loc.dist(wm->ball.pos.loc)) < attackerDist && dist < 1000)
                    {
                        attackerID = i;
                        attackerDist = dist;
                    }
                }
            }

            // No attacker found.
            if(attackerID == -1)
            {
                // Ball is moving.
                if(wm->ball.vel.loc.length() > 0.03)
                {
                    Line2D ballLine = Line2D(Vector2D(wm->ball.pos.loc), Vector2D(wm->ball.pos.loc + wm->ball.vel.loc * 1000));
                    Vector2D s = Field::leftLine.intersection(ballLine);

                    // Ball is moving toward goal.
                    if(wm->ball.vel.loc.x < 0 && (s.y <= Field::ourGoalPost_L.y + 100 && s.y >= Field::ourGoalPost_R.y - 100))
                    {                        
                        rc.fin_pos.loc = s;
                        rc.fin_pos.loc.x += 60;
                        rc.maxSpeed = 2;
                    }
                    // Ball is not moving toward goal.
                    else
                    {
                        double dL = wm->ball.pos.loc.dist(Field::ourGoalPost_L);
                        double dR = wm->ball.pos.loc.dist(Field::ourGoalPost_R);

                        Vector2D dest = Field::ourGoalCenter;
                        dest.y += (dR - dL) / 2;
                        dest.x += 60;

                        rc.fin_pos.loc = dest;
                        rc.maxSpeed = 2;
                    }
                }                
                // Ball is not moving.
                else
                {
                    double dL = wm->ball.pos.loc.dist(Field::ourGoalPost_L);
                    double dR = wm->ball.pos.loc.dist(Field::ourGoalPost_R);

                    Vector2D dest = Field::ourGoalCenter;
                    dest.y += (dR - dL) / 2;
                    dest.x += 60;

                    rc.fin_pos.loc = dest;
                    rc.maxSpeed = 2;
                }
            }
            // An opp robot may kick.
            else
            {                                
                // Ball is moving.
                if(wm->ball.vel.loc.length() > 0.7)
                {
                    Line2D ballLine = Line2D(Vector2D(wm->ball.pos.loc), Vector2D(wm->ball.pos.loc + wm->ball.vel.loc * 1000));
                    Vector2D s = Field::leftLine.intersection(ballLine);

                    // Ball is moving toward goal.
                    if(wm->ball.vel.loc.x < 0 && (s.y <= Field::ourGoalPost_L.y + 100 && s.y >= Field::ourGoalPost_R.y - 100))
                    {
                        rc.fin_pos.loc = s;
                        rc.fin_pos.loc.x += 60;
                        rc.maxSpeed = 2;
                    }
                    // Ball is not moving toward goal.
                    else
                    {
                        double dL = wm->ball.pos.loc.dist(Field::ourGoalPost_L);
                        double dR = wm->ball.pos.loc.dist(Field::ourGoalPost_R);

                        Vector2D dest = Field::ourGoalCenter;
                        dest.y += (dR - dL) / 2;
                        dest.x += 60;

                        rc.fin_pos.loc = dest;
                        rc.maxSpeed = 2;
                    }
                }
                // Ball is not moving.
                else
                {
                    Line2D robotLine = Line2D(Vector2D(wm->oppRobot[attackerID].pos.loc), Vector2D(wm->oppRobot[attackerID].pos.loc +
                                              Vector2D::polar2vector(100, wm->oppRobot[attackerID].pos.dir * AngleDeg::RAD2DEG)));
                    Vector2D s = Field::leftLine.intersection(robotLine);
                    if(s.y > 350)
                        s.y = 350 - ROBOT_RADIUS;
                    else if(s.y < 350)
                        s.y = -350 + ROBOT_RADIUS;

                    rc.fin_pos.loc = s;
                    rc.maxSpeed = 2;
                }
            }
        }
        // Ball is inside golie area.
        else
        {
            // Ball is moving.
            if(wm->ball.vel.loc.length()>0.86)
            {
                Line2D ballLine = Line2D(Vector2D(wm->ball.pos.loc), Vector2D(wm->ball.pos.loc + wm->ball.vel.loc * 1000));

                Vector2D s = Field::leftLine.intersection(ballLine);

                // Ball is moving toward goal.
                if(wm->ball.vel.loc.x < 0 && (s.y <= Field::ourGoalPost_L.y + 100 && s.y >= Field::ourGoalPost_R.y - 100))
                {
                    qDebug()<<"ball is moving inside goal. " << wm->ourRobot[0].pos.loc.x;
                    rc.fin_pos.loc = s;
                    rc.fin_pos.loc.x += 60;
                    rc.maxSpeed = 2;
                }
                // Ball is not moving toward goal.
                else
                {
                    double dL = wm->ball.pos.loc.dist(Field::ourGoalPost_L);
                    double dR = wm->ball.pos.loc.dist(Field::ourGoalPost_R);

                    Vector2D dest = Field::ourGoalCenter;
                    dest.y += (dR - dL)/2;
                    dest.x += 60;

                    rc.fin_pos.loc = dest;
                    rc.maxSpeed = 2;
                }
            }
            // Ball is not moving.
            else
            {
                rc.fin_pos.loc = wm->ball.pos.loc;
                rc.fin_pos.loc.x -= ROBOT_RADIUS;
                if(rc.fin_pos.loc.x < Field::ourGoalCenter.x + 60)

                rc.fin_pos.dir = 0;//(wm->ball.pos.loc - wm->ourRobot[id].pos.loc).dir().radian();
                rc.maxSpeed = 2;
      //          double werr1 = (wm->ball.pos.loc - wm->ourRobot[id].pos.loc).dir().radian()
        //                              - wm->ourRobot[id].pos.dir;
    //            if (werr1 > M_PI) werr1 -= M_2PI;
    //            if (werr1 < -M_PI) werr1 += M_2PI;

                if((wm->ourRobot[id].pos.loc-wm->ball.pos.loc).length() < (ROBOT_RADIUS + BALL_RADIUS))
                {
                    if(fabs((wm->ball.pos.loc - wm->ourRobot[id].pos.loc).dir().degree() * AngleDeg::DEG2RAD - wm->ourRobot[id].pos.dir) < AngleDeg::deg2rad(7))
                    {

                        qDebug()<<"goalie:KIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIICK";
                        rc.kickspeedx=5;
                    }
                }
            }
        }
    }
    // There is no ball;
    else
    {
        rc.fin_pos.loc = Field::ourGoalCenter;
        rc.fin_pos.loc.x += 60;
        rc.maxSpeed = 2;
    }


    rc.useNav = false;
    rc.isBallObs = false;
    rc.isKickObs = false;
    return rc;
}

