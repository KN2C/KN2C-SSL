#include "tacticdefender.h"

TacticDefender::TacticDefender(WorldModel *worldmodel, QObject *parent) :
    Tactic("TacticDefender", worldmodel, parent)
{
    def_t = 1;
    def_c = 0;
}

void TacticDefender::setDefenderID(int total, int current)
{
    def_t = total;
    def_c = current;
}

bool TacticDefender::IsInsideDefendArea(Vector2D pos)
{
    bool a = wm->kn->IsInsideGoalShape(pos, Field::ourGoalCenter.x, Field::goalCircleEX_R,
                                 Field::ourGoalCC_L.dist(Field::ourGoalCC_R));

    bool b = wm->kn->IsInsideGoalShape(pos, Field::ourGoalCenter.x, Field::goalCircleDEF_R,
                                 Field::ourGoalCC_L.dist(Field::ourGoalCC_R));

    return !a && b;
}

int TacticDefender:: ResolvePosition(Vector2D pos, double range, Vector2D *a, Vector2D *b)
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

    if(t < 2 && s1 > 0 && !wm->kn->IsInsideGoalShape(p1, x, r, offset))
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

    if(t < 2 && s1 > 1 && !wm->kn->IsInsideGoalShape(p2, x, r, offset))
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

    if(t < 2 && s2 > 0 && !wm->kn->IsInsideGoalShape(p3, x, r, offset))
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

    if(t < 2 && s2 > 1 && !wm->kn->IsInsideGoalShape(p4, x, r, offset))
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

    if(t < 2 && s3 > 0 && !wm->kn->IsInsideGoalShape(p5, x, r, offset))
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

    if(t < 2 && s3 > 1 && !wm->kn->IsInsideGoalShape(p6, x, r, offset))
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

bool TacticDefender::ExtractDefendPoint(Vector2D origin, Vector2D target, Vector2D *vOut)
{
    if(origin == Vector2D::INVALIDATED || target == Vector2D::INVALIDATED)
    {
        return false;
    }

    int sCount;
    Vector2D ret = Vector2D::INVALIDATED, s1, s2;

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
            ret = s1;
        }

        if(s2.x > Field::ourGoalCenter.x && s2.y > Field::ourGoalCC_L.y)
        {
            if(ret == Vector2D::INVALIDATED)
            {
                ret = s2;
            }
            else
            {
                if(origin.dist(s2) < origin.dist(ret))
                {
                    ret = s2;
                }
            }
        }
    }
    else if(sCount == 1)
    {
        if(s1.x > Field::ourGoalCenter.x && s1.y > Field::ourGoalCC_L.y)
        {
            ret = s1;
        }
    }

    // Check for right arc.
    sCount = cr.intersection(ray, &s1, &s2);
    if(sCount == 2)
    {
        if(s1.x > Field::ourGoalCenter.x && s1.y < Field::ourGoalCC_R.y)
        {
            if(ret == Vector2D::INVALIDATED)
            {
                ret = s1;
            }
            else
            {
                if(origin.dist(s1) < origin.dist(ret))
                {
                    ret = s1;
                }
            }
        }

        if(s2.x > Field::ourGoalCenter.x && s2.y < Field::ourGoalCC_R.y)
        {
            if(ret == Vector2D::INVALIDATED)
            {
                ret = s2;
            }
            else
            {
                if(origin.dist(s2) < origin.dist(ret))
                {
                    ret = s2;
                }
            }
        }
    }
    else if(sCount == 1)
    {
        if(s1.x > Field::ourGoalCenter.x && s1.y < Field::ourGoalCC_R.y)
        {
            if(ret == Vector2D::INVALIDATED)
            {
                ret = s1;
            }
            else
            {
                if(origin.dist(s1) < origin.dist(ret))
                {
                    ret = s1;
                }
            }
        }
    }

    // Check for between arcs.
    s1 = ray.intersection(line);
    if(s1 != Vector2D::INVALIDATED)
    {
        if(s1.y <= Field::ourGoalCC_L.y && s1.y >= Field::ourGoalCC_R.y)
        {
            if(ret == Vector2D::INVALIDATED)
            {
                ret = s1;
            }
            else
            {
                if(origin.dist(s2) < origin.dist(ret))
                {
                    ret = s2;
                }
            }
        }
    }

    if(vOut != nullptr)
    {
        *vOut = ret;
    }

    if(target.dist(Field::ourGoalCenter) > Field::goalCircle_R)
    {
        return false;
    }
    else
    {
        return true;
    }
}

Vector2D TacticDefender::GotoDefaultLoc(int total, int current)
{
    Vector2D out;

    switch (total) {
    case 1: // One defender.
    {
        out = Field::ourGoalCenter;
        out.x += Field::goalCircleEX_R;
    }
        break;
    case 2: // Two defenders.
    {
        switch (current) {
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
        switch (current) {
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

RobotCommand TacticDefender::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid || def_t < 1 || def_c >= def_t) return rc;

    double ballMoveThreshold = 0.1, ballKickThreshold = 1;
    double attackerRangeThreshold = 500, warnerRangeThreshold = 2700;

    // There is a ball in the field.
    if(wm->ball.isValid && wm->kn->IsInsideField(wm->ball.pos.loc))
    {
        // Ball is in defensive area.
        if(IsInsideDefendArea(wm->ball.pos.loc))
        {
            int w;
            double dist, minDist = 10000000;

            for(int i = 0; i < PLAYERS_MAX_NUM; ++i)
            {
                if(wm->ourRobot[i].isValid && (wm->ourRobot[i].Role == AgentRole::DefenderLeft ||
                                               wm->ourRobot[i].Role == AgentRole::DefenderRight ||
                                               wm->ourRobot[i].Role == AgentRole::DefenderMid) &&
                  (dist = wm->ball.pos.loc.dist(wm->ourRobot[i].pos.loc)) < minDist)
                {
                    w = i;
                    minDist = dist;
                }
            }

            // we are the nearest.
            if(w == id)
            {
                rc.fin_pos = wm->kn->AdjustKickPoint(wm->ball.pos.loc, Field::oppGoalCenter);
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
            if(wm->kn->CanKick(wm->ourRobot[id].pos, wm->ball.pos.loc))
            {
                rc.kickspeedx = 5;
            }
        }
        // Ball is out of defensive area.
        else if(!wm->kn->IsInsideGolieArea(wm->ball.pos.loc))
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
                        Ray2D ballRayOL = Ray2D(wm->ball.pos.loc, wm->ball.vel.loc.dir() + 2);
                        Ray2D ballRayOR = Ray2D(wm->ball.pos.loc, wm->ball.vel.loc.dir() - 2);

                        Vector2D s = ballRay.intersection(Field::leftLine);
                        Vector2D p;

                        // Ball moving toward goal. [IV.I]
                        bool exret = ExtractDefendPoint(wm->ball.pos.loc, s, &p);
                        if(exret && p != Vector2D::INVALIDATED)
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
                    Vector2D r;
                    Vector2D p = wm->ball.pos.loc;
                    Ray2D ray1 = Ray2D(p, wm->oppRobot[attackerID].pos.dir * AngleDeg::RAD2DEG);
                    Ray2D ray2 = Ray2D(p, (p - wm->oppRobot[attackerID].pos.loc).dir());
                    Vector2D q1 = ray1.intersection(Field::leftLine);
                    Vector2D q2 = ray2.intersection(Field::leftLine);

                    bool ret1 = ExtractDefendPoint(p, q1, nullptr);
                    bool ret2 = ExtractDefendPoint(p, q2, nullptr);

                    if(ret1 && !ret2)
                    {
                        ExtractDefendPoint(p, q1, &r);
                    }
                    else if(ret2 && !ret1)
                    {
                        ExtractDefendPoint(p, q2, &r);
                    }
                    else if(ret1 && ret2)
                    {
                        Vector2D q1, q2;
                        Circle2D circle(p, 100);
                        circle.intersection(ray1, &q1, nullptr);
                        circle.intersection(ray2, &q2, nullptr);

                        q1 = Ray2D(p, (q1 + q2) * 0.5).intersection(Field::leftLine);
                        ExtractDefendPoint(p, q1, &r);
                    }
                    else
                    {
                        ExtractDefendPoint(p, Field::ourGoalCenter, &r);
                    }

                    if(r != Vector2D::INVALIDATED)
                    {
                        switch (def_t) {
                        case 1:
                        {
                            rc.fin_pos.loc = r;
                        }
                            break;
                        case 2:
                        {
                            Vector2D a, b;
                            ResolvePosition(r, ROBOT_RADIUS, &a, &b);

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
                                ResolvePosition(r, ROBOT_RADIUS, &a, &b);

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
                    }
                    else
                    {
                        rc.fin_pos.loc = GotoDefaultLoc(def_t, def_c);
                    }

                    rc.fin_pos.dir = 0;
                    rc.maxSpeed = 2;
                }
            }
            // Warner robot found.
            else
            {
                Vector2D pa[3], pb[3];
                // Ball is not moving. [II]&[IV]
                if(wm->ball.vel.loc.length() < ballMoveThreshold)
                {
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
                    // There are attacker and warner.
                    else
                    {
                        Vector2D r;
                        Vector2D p = wm->ball.pos.loc;
                        Ray2D ray1 = Ray2D(p, wm->oppRobot[attackerID].pos.dir * AngleDeg::RAD2DEG);
                        Ray2D ray2 = Ray2D(p, (p - wm->oppRobot[attackerID].pos.loc).dir());
                        Vector2D q1 = ray1.intersection(Field::leftLine);
                        Vector2D q2 = ray2.intersection(Field::leftLine);

                        bool ret1 = ExtractDefendPoint(p, q1, nullptr);
                        bool ret2 = ExtractDefendPoint(p, q2, nullptr);

                        if(ret1 && !ret2)
                        {
                            r = q1;
                        }
                        else if(ret2 && !ret1)
                        {
                            r = q2;
                        }
                        else if(ret1 && ret2)
                        {
                            Circle2D circle(p, 100);
                            circle.intersection(ray1, &q1, nullptr);
                            circle.intersection(ray2, &q2, nullptr);

                            r = Ray2D(p, (q1 + q2) * 0.5).intersection(Field::leftLine);
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
                        bool ret = ExtractDefendPoint(pa[0], pb[0], &temp);
                        if(ret && temp != Vector2D::INVALIDATED)
                        {
                            rc.fin_pos.loc = temp;
                        }
                        else
                        {
                            rc.fin_pos.loc = GotoDefaultLoc(def_t, def_c);
                        }
                        rc.fin_pos.dir = 0;
                        rc.maxSpeed = 2;
                    }
                        break;
                    // Two defenders.
                    case 2:
                    {
                        Vector2D temp1, temp2;
                        bool ret = ExtractDefendPoint(pa[0], pb[0], &temp1);
                        if(!ret || temp1 == Vector2D::INVALIDATED)
                        {
                            temp1 = GotoDefaultLoc(def_t, def_c);
                        }

                        ret = ExtractDefendPoint(pa[1], pb[1], &temp2);
                        if(!ret || temp2 == Vector2D::INVALIDATED)
                        {
                            temp2 = GotoDefaultLoc(def_t, def_c);
                        }

                        if((def_c == 0 && temp1.y < temp2.y) || (def_c == 1 && temp1.y > temp2.y))
                        {
                            rc.fin_pos.loc = temp2;
                        }
                        else
                        {
                            rc.fin_pos.loc = temp1;
                        }

                        rc.fin_pos.dir = 0;
                        rc.maxSpeed = 2;
                    }
                        break;
                    // Three defenders.
                    case 3:
                    {
                        Vector2D temp1, temp2, temp3;
                        bool ret = ExtractDefendPoint(pa[0], pb[0], &temp1);
                        if(!ret || temp1 == Vector2D::INVALIDATED)
                        {
                            temp1 = GotoDefaultLoc(def_t, def_c);
                        }

                        ret = ExtractDefendPoint(pa[1], pb[1], &temp2);
                        if(!ret || temp2 == Vector2D::INVALIDATED)
                        {
                            temp2 = GotoDefaultLoc(def_t, def_c);
                        }

                        ret = ExtractDefendPoint(pa[2], pb[2], &temp3);
                        if(!ret || temp3 == Vector2D::INVALIDATED)
                        {
                            temp3 = GotoDefaultLoc(def_t, def_c);
                        }

                        if(def_c != 2)
                        {
                            if((def_c == 0 && temp1.y < temp2.y) || (def_c == 1 && temp1.y > temp2.y))
                            {
                                rc.fin_pos.loc = temp2;
                            }
                            else
                            {
                                rc.fin_pos.loc = temp1;
                            }
                        }
                        else
                        {
                            rc.fin_pos.loc = temp3;
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
                    bool exret = ExtractDefendPoint(wm->ball.pos.loc, s, &p);
                    if(exret && p != Vector2D::INVALIDATED)
                    {
                        rc.fin_pos.loc = p;
                    }
                    else
                    {
                        if(warnerCount == 1)
                        {
                            pa[0] = wm->oppRobot[warnerID[0]].pos.loc;
                            pb[0] = Field::ourGoalCenter;

                            pa[1] = wm->ball.pos.loc;
                            pb[1] = Field::ourGoalCenter;

                            pa[2] = wm->ball.pos.loc;
                            pb[2] = Field::ourGoalCenter;
                        }
                        else
                        {
                            pa[0] = wm->oppRobot[warnerID[0]].pos.loc;
                            pb[0] = Field::ourGoalCenter;

                            pa[1] = wm->oppRobot[warnerID[1]].pos.loc;
                            pb[1] = Field::ourGoalCenter;

                            pa[2] = wm->ball.pos.loc;
                            pb[2] = Field::ourGoalCenter;
                        }

                        switch (def_t) {
                        // One defender.
                        case 1:
                        {
                            Vector2D temp;
                            bool r = ExtractDefendPoint(pa[0], pb[0], &temp);
                            if(!r || temp == Vector2D::INVALIDATED)
                            {
                                temp = GotoDefaultLoc(def_t, def_c);
                            }

                            rc.fin_pos.loc = temp;
                            rc.fin_pos.dir = 0;
                            rc.maxSpeed = 2;
                        }
                            break;
                        // Two defenders.
                        case 2:
                        {
                            Vector2D temp1, temp2;
                            bool r = ExtractDefendPoint(pa[0], pb[0], &temp1);
                            if(!r || temp1 == Vector2D::INVALIDATED)
                            {
                                temp1 = GotoDefaultLoc(def_t, def_c);
                            }

                            r = ExtractDefendPoint(pa[1], pb[1], &temp2);
                            if(!r || temp2 == Vector2D::INVALIDATED)
                            {
                                temp2 = GotoDefaultLoc(def_t, def_c);
                            }

                            if((def_c == 0 && temp1.y < temp2.y) || (def_c == 1 && temp1.y > temp2.y))
                            {
                                rc.fin_pos.loc = temp2;
                            }
                            else
                            {
                                rc.fin_pos.loc = temp1;
                            }

                            rc.fin_pos.dir = 0;
                            rc.maxSpeed = 2;
                        }
                            break;
                        // Three defenders.
                        case 3:
                        {
                            Vector2D temp1, temp2, temp3;
                            bool r = ExtractDefendPoint(pa[0], pb[0], &temp1);
                            if(!r || temp1 == Vector2D::INVALIDATED)
                            {
                                temp1 = GotoDefaultLoc(def_t, def_c);
                            }

                            r = ExtractDefendPoint(pa[1], pb[1], &temp2);
                            if(!r || temp2 == Vector2D::INVALIDATED)
                            {
                                temp2 = GotoDefaultLoc(def_t, def_c);
                            }

                            r = ExtractDefendPoint(pa[2], pb[2], &temp3);
                            if(!r || temp3 == Vector2D::INVALIDATED)
                            {
                                temp1 = GotoDefaultLoc(def_t, def_c);
                            }

                            if(def_c != 2)
                            {
                                if((def_c == 0 && temp1.y < temp2.y) || (def_c == 1 && temp1.y > temp2.y))
                                {
                                    rc.fin_pos.loc = temp2;
                                }
                                else
                                {
                                    rc.fin_pos.loc = temp1;
                                }
                            }
                            else
                            {
                                rc.fin_pos.loc = temp3;
                            }

                            rc.fin_pos.dir = 0;
                            rc.maxSpeed = 2;
                        }
                            break;
                        }
                    }

                    rc.fin_pos.dir = 0;
                    rc.maxSpeed = 2;
                }
            }
        }
        // Ball is inside golie area or behind defend line.
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

    if(rc.fin_pos.loc.x == 0 || rc.fin_pos.loc == Vector2D::INVALIDATED)
    {
        qDebug()<<"Defender BUG found!";
    }

    if(!wm->cmgs.canKickBall())
    {
        rc.kickspeedx = 0;
        rc.kickspeedz = 0;
    }

    rc.fin_pos.dir = 0;
    rc.maxSpeed = 2;

    rc.useNav = true;
    rc.isBallObs = true;
    rc.isKickObs = false;

    return rc;
}
