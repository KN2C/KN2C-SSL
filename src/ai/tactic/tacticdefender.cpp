#include "tacticdefender.h"

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

static int ResolvePosition(Vector2D pos, double range, Vector2D *a, Vector2D *b)
{
    int s1, s2, s3;
    Circle2D c(pos, range);
    Circle2D cr(Field::ourGoalCC_R, Field::goalCircleEX_R);
    Circle2D cl(Field::ourGoalCC_L, Field::goalCircleEX_R);
    Line2D l = Line2D(Vector2D(Field::ourGoalCenter.x + Field::goalCircleEX_R, 0),
                      Vector2D(Field::ourGoalCenter.x + Field::goalCircleEX_R, 10));

    Vector2D p1, p2, p3, p4, p5, p6;

    s1 = c.intersection(cr, &p1, &p2);
    s2 = c.intersection(cl, &p3, &p4);
    s3 = c.intersection(l, &p5, &p6);

    // No contact.
    if(s1 + s2 + s3 == 0)
        return 0;

    int t = 0;
    int x = Field::ourGoalCenter.x;
    int r = Field::goalCircleEX_R - 20;
    int offset = Field::ourGoalCC_L.dist(Field::ourGoalCC_R);

    if(t < 2 && s1 > 0 && !IsInsideGoalShape(p1, x, r, offset))
    {
        if(t == 0)
        {
            *a = p1;
        }
        else
        {
            *b = p1;
        }
        t++;
    }

    if(t < 2 && s1 > 1 && !IsInsideGoalShape(p2, x, r, offset))
    {
        if(t == 0)
        {
            *a = p2;
        }
        else
        {
            *b = p2;
        }
        t++;

    }

    if(t < 2 && s2 > 0 && !IsInsideGoalShape(p3, x, r, offset))
    {
        if(t == 0)
        {
            *a = p3;
        }
        else
        {
            *b = p3;
        }
        t++;
    }

    if(t < 2 && s2 > 1 && !IsInsideGoalShape(p4, x, r, offset))
    {
        if(t == 0)
        {
            *a = p4;
        }
        else
        {
            *b = p4;
        }
        t++;
    }

    if(t < 2 && s3 > 0 && !IsInsideGoalShape(p5, x, r, offset))
    {
        if(t == 0)
        {
            *a = p5;
        }
        else
        {
            *b = p5;
        }
        t++;
    }

    if(t < 2 && s3 > 1 && !IsInsideGoalShape(p6, x, r, offset))
    {
        if(t == 0)
        {
            *a = p5;
        }
        else
        {
            *b = p5;
        }
        t++;
    }

    return t;
}

static bool IsInsideGolieArea(Vector2D pos)
{
    return IsInsideGoalShape(pos, Field::ourGoalCenter.x, Field::goalCircle_R,
                             Field::ourGoalCC_L.dist(Field::ourGoalCC_R));
}

static bool IsInsideDefendArea(Vector2D pos)
{
    bool a = IsInsideGoalShape(pos, Field::ourGoalCenter.x, Field::goalCircleEX_R,
                                 Field::ourGoalCC_L.dist(Field::ourGoalCC_R));

    bool b = IsInsideGoalShape(pos, Field::ourGoalCenter.x, Field::goalCircleDEF_R,
                                 Field::ourGoalCC_L.dist(Field::ourGoalCC_R));

    return !a && b;
}

static bool ExtractDefendPoint(Vector2D origin, Vector2D target, Vector2D *vOut)
{
    if(origin == Vector2D::INVALIDATED || target == Vector2D::INVALIDATED)
    {
        return false;
    }

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

    double ballMoveThreshold = 0.05, ballKickThreshold = 1;
    double attackerRangeThreshold = 500, warnerRangeThreshold = 2600;

    // There is a ball in the field.
    if(wm->ball.isValid && IsInsideField(wm->ball.pos.loc))
    {
        // Ball is in defensive area.
        if(IsInsideDefendArea(wm->ball.pos.loc))
        {
            // TODO: robots should kick the ball.

            int w;
            double dist, minDist = 10000000;

            for(int i = 0; i < PLAYERS_MAX_NUM; ++i)
            {
                if(wm->ourRobot[i].isValid && (dist = wm->ball.pos.loc.dist(wm->ourRobot[i].pos.loc)) < minDist)
                {
                    w = i;
                    minDist = dist;
                }
            }

            // we are the nearest.
            if(w == id)
            {
                rc.fin_pos.loc = wm->ball.pos.loc;
                rc.fin_pos.loc.x -= ROBOT_RADIUS;
                if(!IsInsideDefendArea(rc.fin_pos.loc))
                {
                    ExtractDefendPoint(wm->ball.pos.loc, rc.fin_pos.loc, &rc.fin_pos.loc);
                }
            }
            // TODO: Cover.
            else
            {
                rc.fin_pos.loc = GotoDefaultLoc(def_t, def_c);
            }

            // Kick if you can.
            if(CanKick(wm->ourRobot[id].pos, wm->ball.pos.loc, ROBOT_RADIUS + BALL_RADIUS*2.5, 7))
            {
                qDebug()<<"Defender " << def_c << ":KIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIICK";
                rc.kickspeedx = 5;
            }
        }
        // Ball is out of defensive and golie area.
        else if(!IsInsideGolieArea(wm->ball.pos.loc))
        {
            int warnerCount = 0;
            int attackerID = -1, warnerID[def_t];
            double attackerDist = 10000000, warnerDist[def_t];

            // Init id and dist.
            for(int i = 0; i < def_t; ++i)
            {
                warnerID[i] = -1;
                warnerDist[i] = 10000000;
            }

            // Find attacker robot.
            for(int i = 0; i < PLAYERS_MAX_NUM; ++i)
            {
                if(wm->oppRobot[i].isValid)
                {
                    double dist = wm->oppRobot[i].pos.loc.dist(wm->ball.pos.loc);
                    if(dist < attackerDist && dist < attackerRangeThreshold)
                    {
                        attackerID = i;
                        attackerDist = dist;
                    }
                }
            }

            // Find warner robot.
            for(int i = 0; i < PLAYERS_MAX_NUM; ++i)
            {
                if(wm->oppRobot[i].isValid)
                {
                    double dist = wm->oppRobot[i].pos.loc.dist(wm->ball.pos.loc);
                    if(warnerCount < def_t && dist < warnerRangeThreshold && i != attackerID)
                    {
                        warnerID[warnerCount] = i;
                        warnerDist[warnerCount] = i;
                        ++warnerCount;
                    }
                }
            }

            // No warner found.
            if(warnerCount == 0)
            {
                // There is no robot.
                if(attackerID == -1)
                {
                    // Ball is moving.
                    if(wm->ball.vel.loc.length() > ballMoveThreshold)
                    {
                        Ray2D ballRay = Ray2D(wm->ball.pos.loc, wm->ball.vel.loc.dir());
                        Ray2D ballRayOL = Ray2D(wm->ball.pos.loc, wm->ball.vel.loc.dir() + 3);
                        Ray2D ballRayOR = Ray2D(wm->ball.pos.loc, wm->ball.vel.loc.dir() - 3);

                        Vector2D s = ballRay.intersection(Field::leftLine);
                        Vector2D p;

                        // Ball moving toward goal. [IV.I]
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
                                qDebug() << "Defender " << def_c << ": defending from kick. " << wm->ball.pos.loc.x;
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
                        // Ball is not moving toward goal. [IV.II]
                        else
                        {
                            rc.fin_pos.loc = GotoDefaultLoc(def_t, def_c);
                            rc.fin_pos.dir = 0;
                            rc.maxSpeed = 2;
                        }
                    }
                    // Ball is not moving. [I]
                    else
                    {
                        rc.fin_pos.loc = GotoDefaultLoc(def_t, def_c);
                        rc.fin_pos.dir = 0;
                        rc.maxSpeed = 2;
                    }
                }
                // Only one attacker and no warner.
                else
                {
                    Vector2D r, r1, r2;
                    Vector2D p = wm->ball.pos.loc;
                    Ray2D ray1 = Ray2D(p, wm->oppRobot[attackerID].pos.dir * AngleDeg::RAD2DEG);
                    Ray2D ray2 = Ray2D(p, (p - wm->oppRobot[attackerID].pos.loc).dir());
                    Vector2D q1 = ray1.intersection(Field::leftLine);
                    Vector2D q2 = ray2.intersection(Field::leftLine);

                    bool ret1 = ExtractDefendPoint(p, q1, &r1);
                    bool ret2 = ExtractDefendPoint(p, q1, &r2);

                    if(ret1 && !ret2)
                    {
                        r = r1;
                    }
                    else if(ret2 && !ret1)
                    {
                        r = r2;
                    }
                    else if(ret1 && ret2)
                    {
                        Circle2D circle(p, 100);
                        circle.intersection(ray1, &q1, nullptr);
                        circle.intersection(ray2, &q2, nullptr);

                        r = Ray2D(p, (q1 + q2) * 0.5).intersection(Field::leftLine);

                        if(!ExtractDefendPoint(p, (q1 + q2) * 0.5, &r))
                        {
                            ExtractDefendPoint(p, Field::ourGoalCenter, &r);
                        }
                    }
                    else
                    {
                        ExtractDefendPoint(p, Field::ourGoalCenter, &r);
                    }

                    switch (def_t) {
                    case 1:
                    {
                        rc.fin_pos.loc = r;
                    }
                        break;
                    case 2:
                    {
                        Vector2D a, b;
                        ResolvePosition(r, 2 * ROBOT_RADIUS, &a, &b);

                        if(def_c == 0)
                        {
                            rc.fin_pos.loc = (a.y > b.y)? a: b;
                        }
                        else
                        {
                            rc.fin_pos.loc = (a.y > b.y)? b: a;
                        }
                    }
                        break;
                    case 3:
                    {
                        if(def_c == 2)
                        {
                            rc.fin_pos.loc = r;
                        }
                        else
                        {
                            Vector2D a, b;
                            ResolvePosition(r, 2 * ROBOT_RADIUS, &a, &b);

                            if(def_c == 0)
                            {
                                rc.fin_pos.loc = (a.y > b.y)? a: b;
                            }
                            else
                            {
                                rc.fin_pos.loc = (a.y > b.y)? b: a;
                            }
                        }
                    }
                        break;
                    }

                    rc.fin_pos.dir = 0;
                    rc.maxSpeed = 2;
                }
            }
            // Warner robot found.
            else
            {
                // Ball is not moving. [II]&[IV]
                if(wm->ball.vel.loc.length() < ballMoveThreshold)
                {
                    Vector2D pa[3], pb[3];

                    // There is no attacker.
                    if(attackerID == -1)
                    {
                        if(warnerCount == 1)
                        {
                            pa[0] = wm->ball.pos.loc;
                            pb[0] = Field::ourGoalCenter;

                            pa[1] = wm->oppRobot[warnerID[0]].pos.loc;
                            pb[1] = Field::ourGoalCenter;

                            pa[2] = wm->oppRobot[warnerID[0]].pos.loc;
                            pb[2] = Field::ourGoalCenter;
                        }
                        else
                        {
                            pa[0] = wm->ball.pos.loc;
                            pb[0] = Field::ourGoalCenter;

                            // Select nearest warner first.
                            if(wm->ball.pos.loc.dist(wm->oppRobot[warnerID[0]].pos.loc) <
                               wm->ball.pos.loc.dist(wm->oppRobot[warnerID[1]].pos.loc))
                            {
                                pa[1] = wm->oppRobot[warnerID[0]].pos.loc;
                                pb[1] = Field::ourGoalCenter;

                                pa[2] = wm->oppRobot[warnerID[1]].pos.loc;
                                pb[2] = Field::ourGoalCenter;
                            }
                            else
                            {
                                pa[1] = wm->oppRobot[warnerID[1]].pos.loc;
                                pb[1] = Field::ourGoalCenter;

                                pa[2] = wm->oppRobot[warnerID[0]].pos.loc;
                                pb[2] = Field::ourGoalCenter;
                            }
                        }
                    }
                    // There is an attacker.
                    else
                    {
                        Vector2D r, r1, r2;
                        Vector2D p = wm->ball.pos.loc;
                        Ray2D ray1 = Ray2D(p, wm->oppRobot[attackerID].pos.dir * AngleDeg::RAD2DEG);
                        Ray2D ray2 = Ray2D(p, (p - wm->oppRobot[attackerID].pos.loc).dir());
                        Vector2D q1 = ray1.intersection(Field::leftLine);
                        Vector2D q2 = ray2.intersection(Field::leftLine);

                        bool ret1 = ExtractDefendPoint(p, q1, &r1);
                        bool ret2 = ExtractDefendPoint(p, q1, &r2);

                        if(ret1 && !ret2)
                        {
                            r = r1;
                        }
                        else if(ret2 && !ret1)
                        {
                            r = r2;
                        }
                        else if(ret1 && ret2)
                        {
                            Circle2D circle(p, 100);
                            circle.intersection(ray1, &q1, nullptr);
                            circle.intersection(ray2, &q2, nullptr);

                            r = Ray2D(p, (q1 + q2) * 0.5).intersection(Field::leftLine);

                            if(!ExtractDefendPoint(p, (q1 + q2) * 0.5, &r))
                            {
                                ExtractDefendPoint(p, Field::ourGoalCenter, &r);
                            }
                        }
                        else
                        {
                            r = Field::ourGoalCenter;
                        }

                        pa[0] = wm->ball.pos.loc;
                        pb[0] = r;

                        pa[1] = wm->oppRobot[warnerID[0]].pos.loc;
                        pb[1] = Field::ourGoalCenter;

                        if(warnerCount == 1)
                        {
                            pa[2] = wm->oppRobot[warnerID[0]].pos.loc;
                            pb[2] = Field::ourGoalCenter;
                        }
                        else
                        {
                            pa[2] = wm->oppRobot[warnerID[1]].pos.loc;
                            pb[2] = Field::ourGoalCenter;
                        }
                    }

                    switch (def_t) {
                    // One defender.
                    case 1:
                    {
                        Vector2D temp;
                        if(ExtractDefendPoint(pa[0], pb[0], &temp))
                        {
                            rc.fin_pos.loc = temp;
                            rc.fin_pos.dir = 0;
                            rc.maxSpeed = 2;
                        }
                        else
                        {
                            rc.fin_pos.loc = GotoDefaultLoc(def_t, def_c);
                            rc.fin_pos.dir = 0;
                            rc.maxSpeed = 2;
                        }
                    }
                        break;
                    // Two defenders.
                    case 2:
                    {
                        Vector2D temp1, temp2;
                        bool a = ExtractDefendPoint(pa[0], pb[0], &temp1);
                        bool b = ExtractDefendPoint(pa[1], pb[1], &temp2);

                        if(!a && !b)
                        {
                            rc.fin_pos.loc = GotoDefaultLoc(def_t, def_c);
                        }
                        else if(a && !b)
                        {
                            if((def_c == 0 && temp1.y < 0) || (def_c == 1 && temp1.y > 0))
                            {
                                rc.fin_pos.loc = GotoDefaultLoc(def_t, def_c);
                            }
                            else
                            {
                                rc.fin_pos.loc = temp1;
                            }
                        }
                        else if(!a && b)
                        {
                            if((def_c == 0 && temp2.y < 0) || (def_c == 1 && temp2.y > 0))
                            {
                                rc.fin_pos.loc = GotoDefaultLoc(def_t, def_c);
                            }
                            else
                            {
                                rc.fin_pos.loc = temp2;
                            }
                        }
                        else
                        {
                            if((def_c == 0 && temp1.y < 0) || (def_c == 1 && temp1.y > 0))
                            {
                                rc.fin_pos.loc = temp2;
                            }
                            else
                            {
                                rc.fin_pos.loc = temp1;
                            }
                        }

                        rc.fin_pos.dir = 0;
                        rc.maxSpeed = 2;
                    }
                        break;
                    // Three defenders.
                    case 3:
                    {
                        Vector2D temp1, temp2, temp3;
                        bool a = ExtractDefendPoint(pa[0], pb[0], &temp1);
                        bool b = ExtractDefendPoint(pa[1], pb[1], &temp2);
                        bool c = ExtractDefendPoint(pa[2], pb[2], &temp3);

                        if(def_c != 2)
                        {
                            if(!a && !b)
                            {
                                rc.fin_pos.loc = GotoDefaultLoc(def_t, def_c);
                            }
                            else if(a && !b)
                            {
                                if((def_c == 0 && temp1.y < 0) || (def_c == 1 && temp1.y > 0))
                                {
                                    rc.fin_pos.loc = GotoDefaultLoc(def_t, def_c);
                                }
                                else
                                {
                                    rc.fin_pos.loc = temp1;
                                }
                            }
                            else if(!a && b)
                            {
                                if((def_c == 0 && temp2.y < 0) || (def_c == 1 && temp2.y > 0))
                                {
                                    rc.fin_pos.loc = GotoDefaultLoc(def_t, def_c);
                                }
                                else
                                {
                                    rc.fin_pos.loc = temp2;
                                }
                            }
                            else
                            {
                                if((def_c == 0 && temp1.y < 0) || (def_c == 1 && temp1.y > 0))
                                {
                                    rc.fin_pos.loc = temp2;
                                }
                                else
                                {
                                    rc.fin_pos.loc = temp1;
                                }
                            }
                        }
                        else
                        {
                            if(c)
                            {
                                rc.fin_pos.loc = temp3;
                            }
                            else
                            {
                                rc.fin_pos.loc = GotoDefaultLoc(def_t, def_c);
                            }
                        }

                        rc.fin_pos.dir = 0;
                        rc.maxSpeed = 2;
                    }
                        break;
                    }
                }
                // Ball is kicked.
                else
                {
                    Ray2D ballRay = Ray2D(wm->ball.pos.loc, wm->ball.vel.loc.dir());
                    Vector2D s = ballRay.intersection(Field::leftLine);
                    Vector2D p;

                    // Ball moving toward goal. [IV.I]
                    if(ExtractDefendPoint(wm->ball.pos.loc, s, &p))
                    {
                        rc.fin_pos.loc = p;
                    }
                    else
                    {
                        rc.fin_pos.loc = GotoDefaultLoc(def_t, def_c);
                    }

                    rc.fin_pos.dir = 0;
                    rc.maxSpeed = 2;
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

    if(rc.fin_pos.loc.x == 0)
    {
        qDebug()<<"Defender BUG found!";
    }


    rc.fin_pos.dir = 0;
    rc.maxSpeed = 2;

    rc.useNav = false;
    rc.isBallObs = false;
    rc.isKickObs = false;

    return rc;
}
