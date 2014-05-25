#include "knowledge.h"
#include "worldmodel.h"

Knowledge::Knowledge(WorldModel *wm, QObject *parent) :
    QObject(parent),
    _wm(wm)
{
}

int Knowledge::CountActiveAgents()
{
    int counter=0;
    for(int i=0; i< PLAYERS_MAX_NUM; i++)
        if(_wm->ourRobot[i].isValid) counter++;
    return counter;
}

QList<int> Knowledge::ActiveAgents()
{
    QList<int> ans;
    for(int i=0; i< PLAYERS_MAX_NUM; i++)
        if(_wm->ourRobot[i].isValid)
            ans.append(i);
    return ans;
}

QList<int> Knowledge::findNearestTo(Vector2D loc)
{
    QMap<double, int> smap;
    for(int i=0; i< PLAYERS_MAX_NUM; i++)
    {
        if(_wm->ourRobot[i].isValid==false) continue;
        double dist=(_wm->ourRobot[i].pos.loc-loc).length();
        smap.insert(dist, i);
    }
    QList<int> ans;
    for(auto i=smap.begin(); i!=smap.end(); i++)
        ans.append(i.value());
    return ans;
}

int Knowledge::findOppAttacker()
{
    int ans=0;
    double mindist=1000000;
    for(int i=0; i< PLAYERS_MAX_NUM; i++)
    {
        if(_wm->oppRobot[i].isValid==false) continue;
        double dist=(_wm->oppRobot[i].pos.loc-_wm->ball.pos.loc).length();
        if(dist<mindist)
        {
            mindist=dist;
            ans=i;
        }
    }
    return ans;
}

bool Knowledge::IsInsideRect(Vector2D pos, Vector2D topLeft, Vector2D bottomRight)
{
    return (pos.x > topLeft.x && pos.x < bottomRight.x &&
            pos.y > bottomRight.y && pos.y < topLeft.y);
}

void Knowledge::ClampToRect(Vector2D *pos, Vector2D topLeft, Vector2D bottomRight)
{
    if(pos->x > bottomRight.x)
    {
        pos->x = bottomRight.x;
    }
    else if(pos->x < topLeft.x)
    {
        pos->x = topLeft.x;
    }

    if(pos->y > topLeft.y)
    {
        pos->y = topLeft.y;
    }
    else if(pos->y < bottomRight.y)
    {
        pos->y = bottomRight.y;
    }
}

bool Knowledge::IsInsideField(Vector2D pos)
{
    return IsInsideRect(pos, Field::upperLeftCorner, Field::bottomRightCorner);
}

bool Knowledge::IsInsideGoalShape(Vector2D pos, double goalLeftX, double goalRadius, double goalCcOffset)
{
    double x = pos.x - goalLeftX;
    Vector2D ccl(goalLeftX, goalCcOffset / 2), ccr(goalLeftX, -goalCcOffset / 2);

    return (pos.dist(ccl) <= goalRadius || pos.dist(ccr) <= goalRadius ||
            (x >= 0 && x <= goalRadius && fabs(pos.y) <= goalCcOffset / 2));
}

bool Knowledge::IsInsideGolieArea(Vector2D pos)
{
    return IsInsideGoalShape(pos, Field::ourGoalCenter.x, Field::goalCircle_R,
                             Field::ourGoalCC_L.dist(Field::ourGoalCC_R));
}

Vector2D Knowledge::PredictDestination(Vector2D sourcePos, Vector2D targetPos, double sourceSpeed, Vector2D targetSpeed)
{
    double factor = _wm->var[3] / 250.0;
    if(factor < 0)
    {
        factor = 0;
    }

    double Vm = sourceSpeed;
    double k = Vm / targetSpeed.length();
    double gamaT = targetSpeed.dir().radian();
    Vector2D delta;

    delta = targetPos - sourcePos;
    double landa = atan2(delta.y, delta.x);
    double theta = gamaT - landa;

    if (theta > AngleDeg::PI)
    {
        theta -= 2 * AngleDeg::PI;
    }

    if (theta < - AngleDeg::PI)
    {
        theta += 2 * AngleDeg::PI;
    }

    double dlta = 0;
    if(k != 0 && fabs(sin(theta) / k) < 1)
    {
        dlta = asin(sin(theta)/k);
    }
    // No solution.
    else
    {
        qDebug() << "Prediction: No solution.";
        return targetPos;//Vector2D::INVALIDATED;
    }

    double tf = factor * (delta.length() / 1000) / (Vm*cos(dlta) - targetSpeed.length() * cos(theta));

    // No solution.
    if(tf < 0)
    {
        qDebug() << "Prediction: No solution.";
        return targetPos;   //Vector2D(0,0); //INVALIDATED;
    }

    double catchDist = targetSpeed.length() * tf * 1000;
    Vector2D catchDiff(catchDist * cos(gamaT), catchDist * sin(gamaT));

    return targetPos + catchDiff;
}

bool Knowledge::CanKick(Position robotPos, Vector2D ballPos)
{
    double distThreshold = _wm->var[0], degThreshold = _wm->var[1] / 10;

    AngleDeg d1((ballPos - robotPos.loc).dir());
    AngleDeg d2(robotPos.dir * AngleDeg::RAD2DEG);
    if(fabs((d1 - d2).degree()) < degThreshold || (360.0 - fabs((d1 - d2).degree())) < degThreshold)
    {
        if(robotPos.loc.dist(ballPos) < distThreshold)
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

bool Knowledge::IsReadyForKick(Position current, Position desired, Vector2D ballPos)
{
    double degThreshold = _wm->var[2] / 10;

    if(fabs((current.dir - desired.dir) * AngleDeg::RAD2DEG) < degThreshold ||
            (360.0 - fabs((current.dir - desired.dir) * AngleDeg::RAD2DEG)) < degThreshold)
    {
        return CanKick(current, ballPos);
    }
    else
    {
        return false;
    }
}

bool Knowledge::ReachedToPos(Vector2D current, Vector2D desired, double distThreshold)
{
    if(current.dist(desired) < distThreshold)
    {

            return true;

    }
    else
    {
        return false;
    }
}

bool Knowledge::ReachedToPos(Position current, Position desired, double distThreshold, double degThreshold)
{
    if(current.loc.dist(desired.loc) < distThreshold)
    {
        if(fabs((current.dir - desired.dir) * AngleDeg::RAD2DEG) < degThreshold ||
                (360.0 - fabs((current.dir - desired.dir) * AngleDeg::RAD2DEG)) < degThreshold)
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

Position Knowledge::AdjustKickPoint(Vector2D ballPos, Vector2D target, int kickSpeed)
{
    Position p;
    Vector2D dir = (ballPos - target).normalizedVector();
    dir.scale(ROBOT_RADIUS - (35 - kickSpeed));

    p.loc = ballPos + dir;
    p.dir = (-dir).dir().radian();

    return p;
}
