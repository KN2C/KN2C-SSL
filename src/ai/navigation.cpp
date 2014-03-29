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
    Position mypos = wm->ourRobot[id].pos;
    Position myvel = wm->ourRobot[id].vel;

    ControllerInput ci;
    ci.cur_pos = mypos;
    ci.cur_vel = myvel;
    ci.fin_pos  = rc.fin_pos;
    ci.fin_vel  = rc.fin_vel;

    //rc.useNav = false; //DISABLE A-STAR
    if(rc.useNav == false || (rc.fin_pos.loc - mypos.loc).length() < ROBOT_RADIUS)
    {
        ci.mid_pos = rc.fin_pos;
        ci.mid_vel = rc.fin_vel;
        ci.fin_dist = (ci.cur_pos.loc - ci.fin_pos.loc).length();
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
    Position mypos = wm->ourRobot[id].pos;
    double pathL = 0;
    //return (mypos.loc - rc.fin_pos.loc).length();

    AStarSearch<MapSearchNode> astarsearch;
    MapSearchNode::wm = wm;
    MapSearchNode::isBallObs = rc.isBallObs;
    MapSearchNode::selfRobot = id;

    MapSearchNode nodeStart;
    MapSearchNode nodeEnd;

    nodeStart.vec = mypos.loc;
    nodeEnd.vec = rc.fin_pos.loc;

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
        qDebug() << "AStarSearch FAILED";
    }
    astarsearch.EnsureMemoryFreed();

    //qDebug()<< "----- NAV END -----";
    return pathL; //path length
}
