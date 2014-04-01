#include "tacticdefender.h"

static bool IsInsideGolieArea(Vector2D pos)
{
    double x = (pos.x - Field::ourGoalCenter.x);

    return (pos.dist(Field::ourGoalCC_L)<=Field::goalCircle_R ||
       pos.dist(Field::ourGoalCC_R)<=Field::goalCircle_R ||
       (x >= 0 && x <= Field::goalCircle_R && fabs(pos.y) <=
        Field::ourGoalCC_L.dist(Field::ourGoalCC_R) / 2));
}

static bool ExtractDefendPoint(Vector2D origin, Vector2D target, Vector2D *vOut)
{
    int sCount;
    Vector2D s1, s2;

    Ray2D ray = Ray2D(origin, target);
    Circle2D cl = Circle2D(Field::ourGoalCC_L, Field::goalCircleEX_R);
    Circle2D cr = Circle2D(Field::ourGoalCC_R, Field::goalCircleEX_R);
    Line2D line = Line2D(Vector2D(Field::ourGoalCenter.x + Field::goalCircleEX_R, 0), AngleDeg(90));

    // Check for left arc.
    sCount = cl.intersection(ray, &s1, &s2);
    if(sCount == 2)
    {
        if(s1.x > Field::ourGoalCenter.x && s1.y > Field::ourGoalCC_L.y)
        {
            *vOut = s1;
            return true;
        }

        if(s2.x > Field::ourGoalCenter.x && s2.y > Field::ourGoalCC_L.y)
        {
            *vOut = s2;
            return true;
        }
    }
    else if(sCount == 1)
    {
        if(s1.x > Field::ourGoalCenter.x && s1.y > Field::ourGoalCC_L.y)
        {
            *vOut = s1;
            return true;
        }
    }

    // Check for right arc.
    sCount = cr.intersection(ray, &s1, &s2);
    if(sCount == 2)
    {
        if(s1.x > Field::ourGoalCenter.x && s1.y < Field::ourGoalCC_R.y)
        {
            *vOut = s1;
            return true;
        }

        if(s2.x > Field::ourGoalCenter.x && s2.y < Field::ourGoalCC_R.y)
        {
            *vOut = s2;
            return true;
        }
    }
    else if(sCount == 1)
    {
        if(s1.x > Field::ourGoalCenter.x && s1.y < Field::ourGoalCC_R.y)
        {
            *vOut = s1;
            return true;
        }
    }

    // Check for between arcs.
    s1 = ray.intersection(line);
    if(s1 != Vector2D::INVALIDATED)
    {
        if(s1.y <= Field::ourGoalCC_L.y && s1.y >= Field::ourGoalCC_R.y)
        {
            *vOut = s1;

            return true;
        }
    }

    return false;
}

static Vector2D GotoDefaultLoc(int def_t, int def_c)
{
    Vector2D out;

    switch (def_t) {
    case 1: // One defender.
    {
        out = Field::ourGoalCenter;
        out.x += Field::goalCircleEX_R;
    }
        break;
    case 2: // Two defenders.
    {
        switch (def_c) {
        case 0: // Left defender.
        {
            out = Field::ourGoalCC_L;
            out.x += Field::goalCircleEX_R;
        }
            break;
        case 1: // Right defender.
        {
            out = Field::ourGoalCC_R;
            out.x += Field::goalCircleEX_R;
        }
            break;
        }
    }
        break;
    case 3: // Three defenders.
    {
        switch (def_c) {
        case 0: // Left defender.
        {
            out = Field::ourGoalCC_L;
            out.x += Field::goalCircleEX_R;
        }
            break;
        case 1: // Right defender.
        {
            out = Field::ourGoalCC_R;
            out.x += Field::goalCircleEX_R;
        }
            break;
        case 2: // Mid defender.
        {
            out = Field::ourGoalCenter;
            out.x += Field::goalCircleEX_R;
        }
            break;
        }
    }
        break;
    }

    return out;
}

static Vector2D BlockAttacker(Vector2D ballPos, Vector2D robotPos, double robotDirection, int def_t, int def_c)
{
    Vector2D p;
    AngleDeg robotDir(robotDirection * AngleDeg::RAD2DEG);

    Ray2D robotRay = Ray2D(robotPos, robotDir);
    Vector2D s = robotRay.intersection(Field::leftLine);

    // Orientation blocking.
    if(ExtractDefendPoint(robotPos, s, &p))
    {
        return p;
    }
    // Position blocking.
    else
    {
        robotRay = Ray2D(robotPos, ballPos);
        s = robotRay.intersection(Field::leftLine);
        if(ExtractDefendPoint(robotPos, s, &p))
        {
            return p;
        }
        else
        {
            return GotoDefaultLoc(def_t, def_c);
        }
    }
}


TacticDefender::TacticDefender(WorldModel *worldmodel, QObject *parent) :
    Tactic(worldmodel, parent)
{
}

void TacticDefender::setDefenderID(int total, int current)
{
    def_t = total;
    def_c = current;
}

RobotCommand TacticDefender::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;

    // There is a ball in the field.
    if(wm->ball.isValid)
    {
        // Ball is not inside golie area.
        if(!IsInsideGolieArea(wm->ball.pos.loc))
        {
            int attackerID = -1, warnerID[def_t];
            for(int i = 0; i < def_t; ++i)
            {
                warnerID[i] = -1;
            }

            double attackerDist = 10000000, warnerDist[def_t];
            for(int i = 0; i < def_t; ++i)
            {
                warnerDist[i] = 10000000;
            }

            int idc = 0;
            for(int i = 0; i < PLAYERS_MAX_NUM; ++i)
            {
                if(wm->oppRobot[i].isValid)
                {
                    double dist = wm->oppRobot[i].pos.loc.dist(wm->ball.pos.loc);
                    if(dist < 2000 && idc < def_t)
                    {
                        warnerID[idc] = i;
                        warnerDist[idc] = i;
                        ++idc;
                    }

                    if(dist < attackerDist && dist < 700)
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
                    Ray2D ballRayOL = Ray2D(wm->ball.pos.loc, wm->ball.vel.loc.dir() + 3);
                    Ray2D ballRayOR = Ray2D(wm->ball.pos.loc, wm->ball.vel.loc.dir() - 3);

                    Vector2D s = ballRay.intersection(Field::leftLine);
                    Vector2D p;

                    // Ball moving toward left line.
                    if(ExtractDefendPoint(wm->ball.pos.loc, s, &p))
                    {
                        switch (def_t) {
                        // One defender.
                        case 1:
                        {
                                rc.fin_pos.loc = p;
                                rc.fin_pos.dir = 0;
                                rc.maxSpeed = 2;
                        }
                            break;
                        // Two defenders.
                        case 2:
                        {
                            switch (def_c) {
                            // Left defender.
                            case 0:
                            {
                                s = ballRayOR.intersection(Field::leftLine);
                                ExtractDefendPoint(wm->ball.pos.loc, s, &p);
                                rc.fin_pos.loc = p;
                                rc.fin_pos.dir = 0;
                                rc.maxSpeed = 2;
                            }
                                break;
                            // Right defender.
                            case 1:
                            {
                                s = ballRayOL.intersection(Field::leftLine);
                                ExtractDefendPoint(wm->ball.pos.loc, s, &p);
                                rc.fin_pos.loc = p;
                                rc.fin_pos.dir = 0;
                                rc.maxSpeed = 2;
                            }
                                break;
                            }
                        }
                            break;
                        // Three defenders.
                        case 3:
                        {
                            switch (def_c) {
                            // Left defender.
                            case 0:
                            {
                                s = ballRayOR.intersection(Field::leftLine);
                                ExtractDefendPoint(wm->ball.pos.loc, s, &p);
                                rc.fin_pos.loc = p;
                                rc.fin_pos.dir = 0;
                                rc.maxSpeed = 2;
                            }
                                break;
                            // Right defender.
                            case 1:
                            {
                                s = ballRayOL.intersection(Field::leftLine);
                                ExtractDefendPoint(wm->ball.pos.loc, s, &p);
                                rc.fin_pos.loc = p;
                                rc.fin_pos.dir = 0;
                                rc.maxSpeed = 2;
                            }
                                break;
                            // Mid defender.
                            case 2:
                            {
                                rc.fin_pos.loc = p;
                                rc.fin_pos.dir = 0;
                                rc.maxSpeed = 2;
                            }
                                break;
                            }
                        }
                            break;
                        }
                    }
                    else
                    {
                        rc.fin_pos.loc = GotoDefaultLoc(def_t, def_c);
                        rc.fin_pos.dir = 0;
                        rc.maxSpeed = 2;
                    }
                }
                // Ball is not moving.
                else
                {
                    rc.fin_pos.loc = GotoDefaultLoc(def_t, def_c);
                    rc.fin_pos.dir = 0;
                    rc.maxSpeed = 2;
                }
            }
            // An opp robot may kick.
            else
            {
//                // Ball is moving.
//                if(wm->ball.vel.loc.length() > 0.7)
//                {
//                    Ray2D ballRay = Ray2D(wm->ball.pos.loc, wm->ball.vel.loc.dir());
//                    Vector2D s = ballRay.intersection(Field::leftLine);

//                    // Ball is moving toward goal.
//                    if(s != Vector2D::INVALIDATED && (s.y <= Field::ourGoalPost_L.y + 100 && s.y >= Field::ourGoalPost_R.y - 100))
//                    {
//                        rc.fin_pos.loc = s;
//                        LimitToGoalArea(&rc.fin_pos.loc);
//                        rc.fin_pos.loc.x += 60;
//                        rc.maxSpeed = 2;
//                    }
//                    // Ball is not moving toward goal.
//                    else
//                    {
//                        double dL = wm->ball.pos.loc.dist(Field::ourGoalPost_L);
//                        double dR = wm->ball.pos.loc.dist(Field::ourGoalPost_R);

//                        Vector2D dest = Field::ourGoalCenter;
//                        dest.y += (dR - dL) / 2;

//                        rc.fin_pos.loc = dest;
//                        LimitToGoalArea(&rc.fin_pos.loc);
//                        rc.fin_pos.loc.x += 60;
//                        rc.maxSpeed = 2;
//                    }
//                }
//                // Ball is not moving.
//                else
                {
                    Ray2D robotRay = Ray2D(wm->oppRobot[attackerID].pos.loc,
                                            AngleDeg(wm->oppRobot[attackerID].pos.dir * AngleDeg::RAD2DEG));
                    Ray2D robotRayOL = Ray2D(wm->oppRobot[attackerID].pos.loc,
                                             AngleDeg(wm->oppRobot[attackerID].pos.dir * AngleDeg::RAD2DEG + 4));
                    Ray2D robotRayOR = Ray2D(wm->oppRobot[attackerID].pos.loc,
                                             AngleDeg(wm->oppRobot[attackerID].pos.dir * AngleDeg::RAD2DEG - 4));

                    Vector2D s = robotRay.intersection(Field::leftLine);
                    Vector2D p;

                    // Attacker robot oriented toward left line.
                    if(ExtractDefendPoint(wm->oppRobot[attackerID].pos.loc, s, &p))
                    {
                        switch (def_t) {
                        // One defender.
                        case 1:
                        {
                                rc.fin_pos.loc = p;
                                rc.fin_pos.dir = 0;
                                rc.maxSpeed = 2;
                        }
                            break;
                        // Two defenders.
                        case 2:
                        {
                            switch (def_c) {
                            // Left defender.
                            case 0:
                            {
                                s = robotRayOR.intersection(Field::leftLine);
                                ExtractDefendPoint(wm->oppRobot[attackerID].pos.loc, s, &p);
                                rc.fin_pos.loc = p;
                                rc.fin_pos.dir = 0;
                                rc.maxSpeed = 2;
                            }
                                break;
                            // Right defender.
                            case 1:
                            {
                                s = robotRayOL.intersection(Field::leftLine);
                                ExtractDefendPoint(wm->oppRobot[attackerID].pos.loc, s, &p);
                                rc.fin_pos.loc = p;
                                rc.fin_pos.dir = 0;
                                rc.maxSpeed = 2;
                            }
                                break;
                            }
                        }
                            break;
                        // Three defenders.
                        case 3:
                        {
                            switch (def_c) {
                            // Left defender.
                            case 0:
                            {
                                s = robotRayOR.intersection(Field::leftLine);
                                ExtractDefendPoint(wm->oppRobot[attackerID].pos.loc, s, &p);
                                rc.fin_pos.loc = p;
                                rc.fin_pos.dir = 0;
                                rc.maxSpeed = 2;
                            }
                                break;
                            // Right defender.
                            case 1:
                            {
                                s = robotRayOL.intersection(Field::leftLine);
                                ExtractDefendPoint(wm->oppRobot[attackerID].pos.loc, s, &p);
                                rc.fin_pos.loc = p;
                                rc.fin_pos.dir = 0;
                                rc.maxSpeed = 2;
                            }
                                break;
                            // Mid defender.
                            case 2:
                            {
                                rc.fin_pos.loc = p;
                                rc.fin_pos.dir = 0;
                                rc.maxSpeed = 2;
                            }
                                break;
                            }
                        }
                            break;
                        }
                    }
                    // Attacker robot may pass to other robots.
                    else
                    {
                        Vector2D d[def_t];
                        for(int i = 0; i < def_t; ++i)
                        {
                             d[i] = BlockAttacker(wm->ball.pos.loc, wm->oppRobot[warnerID[i]].pos.loc,
                                          wm->oppRobot[warnerID[i]].pos.dir, def_t, def_c);
                        }

                        switch (def_t) {
                        // One defender.
                        case 1:
                        {
                            rc.fin_pos.loc = d[0];
                            rc.fin_pos.dir = 0;
                            rc.maxSpeed = 2;
                        }
                            break;
                        // Two defenders.
                        case 2:
                        {
                            switch (def_c) {
                            // Left defender.
                            case 0:
                            {
                                if(d[0].y > 0)
                                {
                                    rc.fin_pos.loc = d[0];
                                }
                                else
                                {
                                    rc.fin_pos.loc = d[1];
                                }

                                rc.fin_pos.dir = 0;
                                rc.maxSpeed = 2;
                            }
                                break;
                            // Right defender.
                            case 1:
                            {
                                if(d[0].y < 0)
                                {
                                    rc.fin_pos.loc = d[0];
                                }
                                else
                                {
                                    rc.fin_pos.loc = d[1];
                                }

                                rc.fin_pos.dir = 0;
                                rc.maxSpeed = 2;
                            }
                                break;
                            }
                        }
                            break;
                        // Three defenders.
                        case 3:
                        {
                            switch (def_c) {
                            // Left defender.
                            case 0:
                            {
                                if(d[0].y > Field::ourGoalCC_L.y)
                                {
                                    rc.fin_pos.loc = d[0];
                                }
                                else if(d[1].y > Field::ourGoalCC_L.y)
                                {
                                    rc.fin_pos.loc = d[1];
                                }
                                else
                                {
                                    rc.fin_pos.loc = d[2];
                                }

                                rc.fin_pos.dir = 0;
                                rc.maxSpeed = 2;
                            }
                                break;
                            // Right defender.
                            case 1:
                            {
                                if(d[0].y < Field::ourGoalCC_R.y)
                                {
                                    rc.fin_pos.loc = d[0];
                                }
                                else if(d[1].y < Field::ourGoalCC_R.y)
                                {
                                    rc.fin_pos.loc = d[1];
                                }
                                else
                                {
                                    rc.fin_pos.loc = d[2];
                                }

                                rc.fin_pos.dir = 0;
                                rc.maxSpeed = 2;
                            }
                                break;
                            // Mid defender.
                            case 2:
                            {
                                if(d[0].y < Field::ourGoalCC_L.y && d[0].y > Field::ourGoalCC_R.y)
                                {
                                    rc.fin_pos.loc = d[0];
                                }
                                else if(d[1].y < Field::ourGoalCC_L.y && d[1].y > Field::ourGoalCC_R.y)
                                {
                                    rc.fin_pos.loc = d[1];
                                }
                                else
                                {
                                    rc.fin_pos.loc = GotoDefaultLoc(def_t, def_c);
                                }

                                rc.fin_pos.dir = 0;
                                rc.maxSpeed = 2;
                            }
                                break;
                            }
                        }
                            break;
                        }
                    }
                }
            }
        }
        // Ball is inside golie area.
        else
        {
            rc.fin_pos.loc = GotoDefaultLoc(def_t, def_c);
            rc.fin_pos.dir = 0;
            rc.maxSpeed = 2;
        }
    }
    // There is no ball;
    else
    {
        rc.fin_pos.loc = GotoDefaultLoc(def_t, def_c);
        rc.fin_pos.dir = 0;
        rc.maxSpeed = 2;
    }

    rc.useNav = false;
    rc.isBallObs = false;
    rc.isKickObs = false;

    return rc;
}
