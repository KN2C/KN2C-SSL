#include "tacticgoalie.h"

static bool CanKick(Position robotPos, Vector2D ballPos, double distLimit, double degLimit)
{
    if(robotPos.loc.dist(ballPos) < distLimit)
    {
        AngleDeg d1((ballPos - robotPos.loc).dir());
        AngleDeg d2(robotPos.dir * AngleDeg::RAD2DEG);
        if(fabs((d1 - d2).degree()) < degLimit || (360.0 - fabs((d1 - d2).degree())) < degLimit)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
    return false;
    }
}

static bool IsInsideField(Vector2D pos)
{
    return pos.x > Field::MinX && pos.x < Field::MaxX &&
           pos.y > Field::MinY && pos.y < Field::MaxY;
}


static bool IsInsideGoalShape(Vector2D pos, double goalLeftX, double goalRadius, double goalCcOffset)
{
    double x = pos.x - goalLeftX;
    Vector2D ccl(goalLeftX, goalCcOffset / 2), ccr(goalLeftX, -goalCcOffset / 2);

    return (pos.dist(ccl) <= goalRadius || pos.dist(ccr) <= goalRadius ||
       (x >= 0 && x <= goalRadius && fabs(pos.y) <= goalCcOffset / 2));
}


static bool IsInsideGolieArea(Vector2D pos)
{
    return IsInsideGoalShape(pos, Field::ourGoalCenter.x, Field::goalCircle_R,
                             Field::ourGoalCC_L.dist(Field::ourGoalCC_R));
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
    if(wm->ball.isValid && IsInsideField(wm->ball.pos.loc))
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
                    Vector2D r = Vector2D::INVALIDATED;
                    Vector2D p = wm->ball.pos.loc;
                    Ray2D ray1 = Ray2D(p, wm->oppRobot[attackerID].pos.dir * AngleDeg::RAD2DEG);
                    Ray2D ray2 = Ray2D(p, (p - wm->oppRobot[attackerID].pos.loc).dir());
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
                        rc.fin_pos.loc = r;
                        LimitToGoalArea(&rc.fin_pos.loc);
                        rc.fin_pos.loc.x += 60;
                        rc.maxSpeed = 2;
                    }
                    // Attacker robot is not oriented toward our goal.
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
                    rc.fin_pos.loc.x = Field::ourGoalCenter.x + 60;

                rc.fin_pos.dir = 0;
                rc.maxSpeed = 2;

                // Kick if you can.
                if(CanKick(wm->ourRobot[id].pos, wm->ball.pos.loc, ROBOT_RADIUS + BALL_RADIUS * 2.5, 7))
                {
                    qDebug()<<"Golie: KIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIICK";
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
        rc.maxSpeed = 2;
    }

    rc.useNav = false;
    rc.isBallObs = false;
    rc.isKickObs = false;

    return rc;
}

