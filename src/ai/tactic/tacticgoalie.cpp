#include "tacticgoalie.h"

static bool IsInsideGolieArea(Vector2D pos)
{
    double x = (pos.x - Field::ourGoalCenter.x);

    return (pos.dist(Field::ourGoalCC_L)<=Field::goalCircle_R ||
       pos.dist(Field::ourGoalCC_R)<=Field::goalCircle_R ||
       (x >= 0 && x <= Field::goalCircle_R && fabs(pos.y) <=
        Field::ourGoalCC_L.dist(Field::ourGoalCC_R) / 2));
}

static void LimitToGoalArea(Vector2D *pos)
{
    if(pos->x < Field::ourGoalCenter.x)
        pos->x = Field::ourGoalCenter.x;

    if(pos->y > Field::ourGoalPost_L.y - ROBOT_RADIUS)
        pos->y = Field::ourGoalPost_L.y - ROBOT_RADIUS;
    else if(pos->y < Field::ourGoalPost_R.y + ROBOT_RADIUS)
        pos->y = Field::ourGoalPost_R.y + ROBOT_RADIUS;
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
                    if((dist = wm->oppRobot[i].pos.loc.dist(wm->ball.pos.loc)) < attackerDist && dist < 700)
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
                    Ray2D ballRay = Ray2D(wm->ball.pos.loc, wm->ball.vel.loc.dir());
                    Vector2D s = ballRay.intersection(Field::leftLine);

                    // Ball is moving toward goal.
                    if(s != Vector2D::INVALIDATED && (s.y <= Field::ourGoalPost_L.y + 100 && s.y >= Field::ourGoalPost_R.y - 100))
                    {
                        rc.fin_pos.loc = s;
                        LimitToGoalArea(&rc.fin_pos.loc);
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

                        rc.fin_pos.loc = dest;
                        LimitToGoalArea(&rc.fin_pos.loc);
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

                    rc.fin_pos.loc = dest;
                    LimitToGoalArea(&rc.fin_pos.loc);
                    rc.fin_pos.loc.x += 60;
                    rc.maxSpeed = 2;
                }
            }
            // An opp robot may kick.
            else
            {                                
                // Ball is moving.
                if(wm->ball.vel.loc.length() > 0.7)
                {
                    Ray2D ballRay = Ray2D(wm->ball.pos.loc, wm->ball.vel.loc.dir());
                    Vector2D s = ballRay.intersection(Field::leftLine);

                    // Ball is moving toward goal.
                    if(s != Vector2D::INVALIDATED && (s.y <= Field::ourGoalPost_L.y + 100 && s.y >= Field::ourGoalPost_R.y - 100))
                    {
                        rc.fin_pos.loc = s;
                        LimitToGoalArea(&rc.fin_pos.loc);
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

                        rc.fin_pos.loc = dest;
                        LimitToGoalArea(&rc.fin_pos.loc);
                        rc.fin_pos.loc.x += 60;
                        rc.maxSpeed = 2;
                    }
                }
                // Ball is not moving.
                else
                {
                    Ray2D robotRay = Ray2D(wm->oppRobot[attackerID].pos.loc,
                                            AngleDeg(wm->oppRobot[attackerID].pos.dir * AngleDeg::RAD2DEG));
                    Vector2D s = robotRay.intersection(Field::leftLine);

                    // Attacker robot oriented toward left line.
                    if(s != Vector2D::INVALIDATED)
                    {
                        rc.fin_pos.loc = s;
                        LimitToGoalArea(&rc.fin_pos.loc);
                        rc.fin_pos.loc.x += 60;
                        rc.maxSpeed = 2;
                    }
                    // Attacker robot is not oriented toward our goal.
                    else
                    {
                        Ray2D r = Ray2D(wm->oppRobot[attackerID].pos.loc, wm->ball.pos.loc);
                        Vector2D s = r.intersection(Field::leftLine);
                        // Position blocking.
                        if(s != Vector2D::INVALIDATED)
                        {
                            rc.fin_pos.loc = s;
                            LimitToGoalArea(&rc.fin_pos.loc);
                            rc.fin_pos.loc.x += 60;
                            rc.maxSpeed = 2;
                        }
                        else
                        {
                            double dL = wm->ball.pos.loc.dist(Field::ourGoalPost_L);
                            double dR = wm->ball.pos.loc.dist(Field::ourGoalPost_R);

                            Vector2D dest = Field::ourGoalCenter;
                            dest.y += (dR - dL) / 2;

                            rc.fin_pos.loc = dest;
                            LimitToGoalArea(&rc.fin_pos.loc);
                            rc.fin_pos.loc.x += 60;
                            rc.maxSpeed = 2;
                        }
                    }
                }
            }
        }
        // Ball is inside golie area.
        else
        {
            // Ball is moving.
            if(wm->ball.vel.loc.length() > 0.85)
            {
                Ray2D ballRay = Ray2D(wm->ball.pos.loc, wm->ball.vel.loc.dir());
                Vector2D s = ballRay.intersection(Field::leftLine);

                // Ball is moving toward goal.
                if(s != Vector2D::INVALIDATED && (s.y <= Field::ourGoalPost_L.y + 100 && s.y >= Field::ourGoalPost_R.y - 100))
                {
                    rc.fin_pos.loc = s;
                    LimitToGoalArea(&rc.fin_pos.loc);
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

                    rc.fin_pos.loc = dest;
                    LimitToGoalArea(&rc.fin_pos.loc);
                    rc.fin_pos.loc.x += 60;
                    rc.maxSpeed = 2;
                }
            }
            // Ball is not moving.
            else
            {
                rc.fin_pos.loc = wm->ball.pos.loc;
                rc.fin_pos.loc.x -= ROBOT_RADIUS;
                if(rc.fin_pos.loc.x < Field::ourGoalCenter.x + 60)

                rc.fin_pos.dir = 0;
                rc.maxSpeed = 2;

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

