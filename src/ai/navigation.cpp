#include "navigation.h"
#include "worldmodel.h"
#include "3rdparty/stlastar.h"
#include "mapsearchnode.h"

Navigation::Navigation(QObject *parent) :
    QObject(parent)
{
    wm = 0;
    id = -1;
}

void Navigation::setID(int id)
{
    this->id = id;
}

void Navigation::setWorldModel(WorldModel *wm)
{
    this->wm = wm;
}

ControllerInput Navigation::calc(RobotCommand rc)
{
    ControllerInput ci;
    ci.cur_pos = wm->ourRobot[id].pos;
    ci.cur_vel = wm->ourRobot[id].vel;
    ci.fin_pos  = rc.fin_pos;
    ci.fin_vel  = rc.fin_vel;

    //rc.useNav = false; //TOTALLY DISABLE A-STAR
    double direct_dist = ci.cur_pos.loc.dist(ci.fin_pos.loc);
    if(rc.useNav == false || direct_dist < ROBOT_RADIUS)
    {
        ci.mid_pos = rc.fin_pos;
        ci.mid_vel = rc.fin_vel;
        ci.fin_dist = direct_dist;
    }
    else
    {
        QList<Vector2D> points;
        ci.fin_dist = getPath(rc, &points);
        //qDebug() << "points" << points.size();

        if(points.size() >= 2)
            ci.mid_pos.loc = points[1];
        else
            ci.mid_pos.loc = ci.fin_pos.loc;

        ci.mid_pos.dir = ci.fin_pos.dir;
        ci.mid_vel = rc.fin_vel;
    }

    ci.maxSpeed  = rc.maxSpeed;
    ci.angleMode = rc.angleMode;
    return ci;
}

double Navigation::getPath(RobotCommand rc, QList<Vector2D> *points)
{
    Q_UNUSED(points);
    if(rc.useNav == false)
        return wm->ourRobot[id].pos.loc.dist(rc.fin_pos.loc);

    AStarSearch<MapSearchNode> astarsearch;
    MapSearchNode::wm = wm;
    MapSearchNode::isBallObs = rc.isBallObs;
    MapSearchNode::isKickObs = rc.isKickObs;
    MapSearchNode::selfRobot = id;

    MapSearchNode nodeStart;
    MapSearchNode nodeEnd;

    nodeStart.vec = wm->ourRobot[id].pos.loc;

    auto obs = MapSearchNode::getObsCircle();
    bool is_fin_obs = false;
    for(int i=0; i<obs.size(); i++)
    {
        if(obs[i].contains(rc.fin_pos.loc))
        {
            Circle2D c(obs[i].center(), obs[i].radius() + BALL_RADIUS*3);
            if(rc.fin_pos.loc == obs[i].center()) rc.fin_pos.loc += Vector2D(-1,0);
            Line2D l(obs[i].center(), rc.fin_pos.loc);
            Vector2D ans1, ans2;
            int ans = c.intersection(l, &ans1, &ans2);
            switch (ans)
            {
            case 0:
                qDebug() << "NAV 0 !!!";
                //qDebug() << c.center().x << c.center().y << c.radius();
                //qDebug() << l.a() << l.b() << l.c();
                //qDebug() << wm->ball.pos.loc.x << wm->ball.pos.loc.y;
                //qDebug() << rc.fin_pos.loc.x << rc.fin_pos.loc.y;
                break;
            case 1:
                //qDebug() << "1";
                nodeEnd.vec = ans1;
                is_fin_obs = true;
                break;
            case 2:
                //qDebug() << "2";
                if(ans1.dist2(rc.fin_pos.loc) < ans2.dist2(rc.fin_pos.loc))
                    nodeEnd.vec = ans1;
                else
                    nodeEnd.vec = ans2;
                is_fin_obs = true;
                break;
            default:
                qDebug() << "DEF";
                break;
            }

            //qDebug() << "FIN_POS chaned" << rc.fin_pos.loc.x << rc.fin_pos.loc.y << nodeEnd.vec.x << nodeEnd.vec.y;
            break;
        }
    }
    if(!is_fin_obs) nodeEnd.vec = rc.fin_pos.loc;

    //qDebug()<< "----- NAV START -----";
    //qDebug()<< "start" << nodeStart.vec.x << nodeStart.vec.y;
    //qDebug()<< "end  " << nodeEnd.vec.x << nodeEnd.vec.y;

    astarsearch.SetStartAndGoalStates(nodeStart, nodeEnd);

    unsigned int SearchState;
    unsigned int SearchSteps = 0;

    do
    {
        //qDebug() << "SearchStep" << SearchSteps;
        SearchState = astarsearch.SearchStep();
        SearchSteps++;
    }
    while(SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SEARCHING);

    double pathL = 0;
    if(SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED)
    {
        //qDebug() << "AStarSearch SUCCEEDED";
        MapSearchNode *node = astarsearch.GetSolutionStart();
        //qDebug() << "SolutionStart" << node->vec.x << node->vec.y;
        int steps = 0;
        if(points) points->append(node->vec); //nodeStart
        Vector2D lastNode = node->vec;
        for(;;)
        {
            node = astarsearch.GetSolutionNext();
            if(!node) break;
            //qDebug() << "SolutionNext" << node->vec.x << node->vec.y;
            if(points) points->append(node->vec);
            steps++;
            pathL += (lastNode - node->vec).length();
            lastNode = node->vec;
        }
        astarsearch.FreeSolutionNodes();
    }
    else if(SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_FAILED)
    {
        qDebug() << "AStarSearch FAILED" << id;
    }
    astarsearch.EnsureMemoryFreed();

    //qDebug()<< "----- NAV END -----";
    return pathL; //path length
}
