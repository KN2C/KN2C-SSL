#include "tacticgoalie.h"

TacticGoalie::TacticGoalie(WorldModel *worldmodel, QObject *parent) :
    Tactic("TacticGoalie", worldmodel, parent)
{
}

void TacticGoalie::LimitToGoalArea(Vector2D *pos)
{
    if(pos->x < Field::ourGoalCenter.x)
        pos->x = Field::ourGoalCenter.x;

    if(pos->y > Field::ourGoalPost_L.y - ROBOT_RADIUS)
        pos->y = Field::ourGoalPost_L.y - ROBOT_RADIUS;
    else if(pos->y < Field::ourGoalPost_R.y + ROBOT_RADIUS)
        pos->y = Field::ourGoalPost_R.y + ROBOT_RADIUS;
}

RobotCommand TacticGoalie::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;
    
    int attackerID = -1;
    double attackerDist = 1000;
    for(int i = 0; i < PLAYERS_MAX_NUM; ++i)
    {
        if(wm->oppRobot[i].isValid)
        {
            double dist;
            if((dist = wm->oppRobot[i].pos.loc.dist(wm->ball.pos.loc)) < attackerDist)
            {
                attackerID = i;
                attackerDist = dist;
            }
        }
    }

    // There is a ball in the field.
    if(wm->ball.isValid && wm->kn->IsInsideField(wm->ball.pos.loc))
    {
        // Ball is not inside golie area.
        if(!wm->kn->IsInsideGolieArea(wm->ball.pos.loc))
        {                        
            // No attacker found.
            if(attackerID == -1)
            {
                // Ball is moving.
                if(wm->ball.vel.loc.length() > 0.05)
                {
                    Ray2D ballRay = Ray2D(wm->ball.pos.loc, wm->ball.vel.loc.dir());
                    Vector2D s = ballRay.intersection(Field::leftLine);
                    
                    // Ball is moving toward goal.
                    if(s != Vector2D::INVALIDATED && (s.y <= Field::ourGoalPost_L.y + 100 && s.y >= Field::ourGoalPost_R.y - 100))
                    {                        
                        rc.fin_pos.loc = s + (wm->ball.pos.loc - s).normalizedVector().scale(ROBOT_RADIUS);
                        LimitToGoalArea(&rc.fin_pos.loc);
                    }
                    // Ball is not moving toward goal.
                    else
                    {
                        double dL = wm->ball.pos.loc.dist(Field::ourGoalPost_L);
                        double dR = wm->ball.pos.loc.dist(Field::ourGoalPost_R);
                        
                        Vector2D dest = Field::ourGoalCenter;
                        dest.y += (dR - dL) / 2;
                        
                        rc.fin_pos.loc = dest + (wm->ball.pos.loc - dest).normalizedVector().scale(ROBOT_RADIUS);
                        LimitToGoalArea(&rc.fin_pos.loc);
                    }
                }
                // Ball is not moving.
                else
                {
                    double dL = wm->ball.pos.loc.dist(Field::ourGoalPost_L);
                    double dR = wm->ball.pos.loc.dist(Field::ourGoalPost_R);
                    
                    Vector2D dest = Field::ourGoalCenter;
                    dest.y += (dR - dL) / 2;
                    
                    rc.fin_pos.loc = dest + (wm->ball.pos.loc - dest).normalizedVector().scale(ROBOT_RADIUS);
                    LimitToGoalArea(&rc.fin_pos.loc);
                }
            }
            // An opp robot may kick.
            else
            {
                // Ball is moving.
                if(wm->ball.vel.loc.length() > 0.8)
                {
                    Ray2D ballRay = Ray2D(wm->ball.pos.loc, wm->ball.vel.loc.dir());
                    Vector2D s = ballRay.intersection(Field::leftLine);
                    
                    // Ball is moving toward goal.
                    if(s != Vector2D::INVALIDATED && (s.y <= Field::ourGoalPost_L.y + 100 && s.y >= Field::ourGoalPost_R.y - 100))
                    {
                        rc.fin_pos.loc = s + (wm->ball.pos.loc - s).normalizedVector().scale(ROBOT_RADIUS);
                        LimitToGoalArea(&rc.fin_pos.loc);
                    }
                    // Ball is not moving toward goal.
                    else
                    {
                        double dL = wm->ball.pos.loc.dist(Field::ourGoalPost_L);
                        double dR = wm->ball.pos.loc.dist(Field::ourGoalPost_R);
                        
                        Vector2D dest = Field::ourGoalCenter;
                        dest.y += (dR - dL) / 2;

                        rc.fin_pos.loc = dest + (wm->ball.pos.loc - dest).normalizedVector().scale(ROBOT_RADIUS);
                        LimitToGoalArea(&rc.fin_pos.loc);
                    }
                }
                // Ball is not moving.
                else
                {
                    Vector2D r = Vector2D::INVALIDATED;
                    Vector2D p = wm->ball.pos.loc;
                    Ray2D ray1 = Ray2D(p, wm->oppRobot[attackerID].pos.dir * AngleDeg::RAD2DEG);
                    Ray2D ray2 = Ray2D(p, p + (p - wm->oppRobot[attackerID].pos.loc));
                    Vector2D q1 = ray1.intersection(Field::leftLine);
                    Vector2D q2 = ray2.intersection(Field::leftLine);
                    
                    if(q1 != Vector2D::INVALIDATED && q2 == Vector2D::INVALIDATED)
                    {
                        r = q1;
                    }
                    else if(q1 == Vector2D::INVALIDATED && q2 != Vector2D::INVALIDATED)
                    {
                        r = q2;
                    }
                    else if(q1 != Vector2D::INVALIDATED && q2 != Vector2D::INVALIDATED)
                    {
                        Circle2D circle(p, 100);
                        circle.intersection(ray1, &q1, nullptr);
                        circle.intersection(ray2, &q2, nullptr);
                        
                        r = Ray2D(p, (q1 + q2) * 0.5).intersection(Field::leftLine);
                    }
                    
                    // Attacker robot oriented toward left line.
                    if(r != Vector2D::INVALIDATED)
                    {
                        rc.fin_pos.loc = r + (wm->ball.pos.loc - r).normalizedVector().scale(ROBOT_RADIUS);
                        LimitToGoalArea(&rc.fin_pos.loc);
                    }
                    // Attacker robot is not oriented toward our goal.
                    else
                    {
                        double dL = wm->ball.pos.loc.dist(Field::ourGoalPost_L);
                        double dR = wm->ball.pos.loc.dist(Field::ourGoalPost_R);
                        
                        Vector2D dest = Field::ourGoalCenter;
                        dest.y += (dR - dL) / 2;

                        rc.fin_pos.loc = dest + (wm->ball.pos.loc - dest).normalizedVector().scale(ROBOT_RADIUS);
                        LimitToGoalArea(&rc.fin_pos.loc);
                    }
                }
            }
        }
        // Ball is inside golie area.
        else
        {
            // Ball is kicked.
            if(wm->ball.vel.loc.length() > 0.85)
            {
                Ray2D ballRay = Ray2D(wm->ball.pos.loc, wm->ball.vel.loc.dir());
                Vector2D s = ballRay.intersection(Field::leftLine);
                
                // Ball is moving toward goal.
                if(s != Vector2D::INVALIDATED && (s.y <= Field::ourGoalPost_L.y + 100 && s.y >= Field::ourGoalPost_R.y - 100))
                {
                    rc.fin_pos.loc = s + (wm->ball.pos.loc - s).normalizedVector().scale(ROBOT_RADIUS);
                    LimitToGoalArea(&rc.fin_pos.loc);
                }
                // Ball is not moving toward goal.
                else
                {
                    double dL = wm->ball.pos.loc.dist(Field::ourGoalPost_L);
                    double dR = wm->ball.pos.loc.dist(Field::ourGoalPost_R);
                    
                    Vector2D dest = Field::ourGoalCenter;
                    dest.y += (dR - dL) / 2;

                    rc.fin_pos.loc = dest + (wm->ball.pos.loc - dest).normalizedVector().scale(ROBOT_RADIUS);
                    LimitToGoalArea(&rc.fin_pos.loc);
                }
            }
            // Ball is not kicked.
            else
            {
                rc.fin_pos = wm->kn->AdjustKickPoint(wm->ball.pos.loc, Field::oppGoalCenter);

                // Kick if you can.
                if(wm->kn->CanKick(wm->ourRobot[id].pos, wm->ball.pos.loc))
                {
                    rc.kickspeedz = 2;
                    rc.kickspeedx = 4;
                }
            }
        }
    }
    // There is no ball;
    else
    {
        rc.fin_pos.loc = Field::ourGoalCenter;
        rc.fin_pos.loc.x += 60;
    }        

    rc.useNav = false;
    rc.maxSpeed = 1.5;

    return rc;
}

