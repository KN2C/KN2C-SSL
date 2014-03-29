#include "mapsearchnode.h"
#include "worldmodel.h"

WorldModel *MapSearchNode::wm;
bool MapSearchNode::isBallObs;
int  MapSearchNode::selfRobot;

bool MapSearchNode::IsSameState(MapSearchNode &rhs)
{
    // same state in a maze search is simply when (x,y) are the same
    if(vec == rhs.vec)
        return true;
    else
        return false;
}

// Here's the heuristic function that estimates the distance from a Node
// to the Goal.

float MapSearchNode::GoalDistanceEstimate(MapSearchNode &nodeGoal)
{
    return (nodeGoal.vec - vec).length();
}

bool MapSearchNode::IsGoal(MapSearchNode &nodeGoal)
{
    return IsSameState(nodeGoal);
}

// This generates the successors to the given Node. It uses a helper function called
// AddSuccessor to give the successors to the AStar class. The A* specific initialisation
// is done for each node internally, so here you just set the state information that
// is specific to the application
bool MapSearchNode::GetSuccessors(AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node)
{
    //Q_UNUSED(astarsearch)
    //Q_UNUSED(parent_node)
    Vector2D parent = Vector2D(100000,100000);
    if(parent_node) parent = parent_node->vec;

    qDebug() << "GetSuccessors";
    qDebug() << "point" << vec.x << vec.y;
    qDebug() << "parent" << parent.x << parent.y;

    if(isBallObs && wm->ball.isValid)
    {
        MapSearchNode node1;
        MapSearchNode node2;
        MapSearchNode node3;
        MapSearchNode node4;

        node1.vec = wm->ball.pos.loc + Vector2D(+(BALL_RADIUS+ROBOT_RADIUS), +(BALL_RADIUS+ROBOT_RADIUS));
        node2.vec = wm->ball.pos.loc + Vector2D(+(BALL_RADIUS+ROBOT_RADIUS), -(BALL_RADIUS+ROBOT_RADIUS));
        node3.vec = wm->ball.pos.loc + Vector2D(-(BALL_RADIUS+ROBOT_RADIUS), +(BALL_RADIUS+ROBOT_RADIUS));
        node4.vec = wm->ball.pos.loc + Vector2D(-(BALL_RADIUS+ROBOT_RADIUS), -(BALL_RADIUS+ROBOT_RADIUS));

        if(node1.vec != parent)
            astarsearch->AddSuccessor(node1);
        if(node2.vec != parent)
            astarsearch->AddSuccessor(node2);
        if(node3.vec != parent)
            astarsearch->AddSuccessor(node3);
        if(node4.vec != parent)
            astarsearch->AddSuccessor(node4);
    }

    for(int i=0; i<PLAYERS_MAX_NUM; i++)
    {
        if(i == selfRobot) continue;
        if(!wm->ourRobot[i].isValid) continue;

        MapSearchNode node1;
        MapSearchNode node2;
        MapSearchNode node3;
        MapSearchNode node4;

        node1.vec = wm->ourRobot[i].pos.loc + Vector2D(+(ROBOT_RADIUS*4), +(ROBOT_RADIUS*4));
        node2.vec = wm->ourRobot[i].pos.loc + Vector2D(+(ROBOT_RADIUS*4), -(ROBOT_RADIUS*4));
        node3.vec = wm->ourRobot[i].pos.loc + Vector2D(-(ROBOT_RADIUS*4), +(ROBOT_RADIUS*4));
        node4.vec = wm->ourRobot[i].pos.loc + Vector2D(-(ROBOT_RADIUS*4), -(ROBOT_RADIUS*4));

        if(node1.vec != parent)
            astarsearch->AddSuccessor(node1);
        if(node2.vec != parent)
            astarsearch->AddSuccessor(node2);
        if(node3.vec != parent)
            astarsearch->AddSuccessor(node3);
        if(node4.vec != parent)
            astarsearch->AddSuccessor(node4);
    }

    for(int i=0; i<PLAYERS_MAX_NUM; i++)
    {
        if(!wm->oppRobot[i].isValid) continue;

        MapSearchNode node1;
        MapSearchNode node2;
        MapSearchNode node3;
        MapSearchNode node4;

        node1.vec = wm->oppRobot[i].pos.loc + Vector2D(+(ROBOT_RADIUS*4), +(ROBOT_RADIUS*4));
        node2.vec = wm->oppRobot[i].pos.loc + Vector2D(+(ROBOT_RADIUS*4), -(ROBOT_RADIUS*4));
        node3.vec = wm->oppRobot[i].pos.loc + Vector2D(-(ROBOT_RADIUS*4), +(ROBOT_RADIUS*4));
        node4.vec = wm->oppRobot[i].pos.loc + Vector2D(-(ROBOT_RADIUS*4), -(ROBOT_RADIUS*4));

        if(node1.vec != parent)
            astarsearch->AddSuccessor(node1);
        if(node2.vec != parent)
            astarsearch->AddSuccessor(node2);
        if(node3.vec != parent)
            astarsearch->AddSuccessor(node3);
        if(node4.vec != parent)
            astarsearch->AddSuccessor(node4);
    }

    MapSearchNode goal;
    goal.vec = astarsearch->GetSolutionEnd()->vec;
    astarsearch->AddSuccessor(goal);

    return true;
}

// given this node, what does it cost to move to successor. In the case
// of our map the answer is the map terrain value at this node since that is
// conceptually where we're moving

float MapSearchNode::GetCost(MapSearchNode &successor)
{
    if(IsSameState(successor)) return 0;

    double BIG_NUMBER = 1000000;
    Segment2D seg(vec, successor.vec);
    double d = seg.length();

    if(isBallObs && wm->ball.isValid)
    {
        Circle2D bc(wm->ball.pos.loc, BALL_RADIUS + ROBOT_RADIUS);
        Vector2D sol1, sol2;
        int hasInt = bc.intersection(seg, &sol1, &sol2);
        if(hasInt > 0) d += BIG_NUMBER;
    }

    for(int i=0; i<PLAYERS_MAX_NUM; i++)
    {
        if(i == selfRobot) continue;
        if(!wm->ourRobot[i].isValid) continue;
        Circle2D rc(wm->ourRobot[i].pos.loc, ROBOT_RADIUS*2);
        Vector2D sol1, sol2;
        int hasInt = rc.intersection(seg, &sol1, &sol2);
        if(hasInt > 0) d += BIG_NUMBER;
    }

    for(int i=0; i<PLAYERS_MAX_NUM; i++)
    {
        if(!wm->oppRobot[i].isValid) continue;
        Circle2D rc(wm->oppRobot[i].pos.loc, ROBOT_RADIUS*2);
        Vector2D sol1, sol2;
        int hasInt = rc.intersection(seg, &sol1, &sol2);
        if(hasInt > 0) d += BIG_NUMBER;
    }

    qDebug() << "GetCost" << vec.x << vec.y << "TO" << successor.vec.x << successor.vec.y << "=" << d;
    return d;
}
