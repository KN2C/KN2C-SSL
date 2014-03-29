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
    Q_UNUSED(astarsearch)
    Q_UNUSED(parent_node)
    return true;
}

// given this node, what does it cost to move to successor. In the case
// of our map the answer is the map terrain value at this node since that is
// conceptually where we're moving

float MapSearchNode::GetCost(MapSearchNode &successor)
{
    double BIG_NUMBER = 1000000;
    Segment2D seg(vec, successor.vec);
    double d = seg.length();

    if(isBallObs && wm->ball.isValid)
    {
        Circle2D bc(wm->ball.pos.loc, BALL_RADIUS + ROBOT_RADIUS);
        int hasInt = bc.intersection(seg, NULL, NULL);
        if(hasInt > 0) d += BIG_NUMBER;
    }

    for(int i=0; i<PLAYERS_MAX_NUM; i++)
    {
        if(i == selfRobot) continue;
        if(!wm->ourRobot[i].isValid) continue;
        Circle2D rc(wm->ourRobot[i].pos.loc, ROBOT_RADIUS*2);
        int hasInt = bc.intersection(seg, NULL, NULL);
        if(hasInt > 0) d += BIG_NUMBER;
    }

    for(int i=0; i<PLAYERS_MAX_NUM; i++)
    {
        if(!wm->oppRobot[i].isValid) continue;
        Circle2D rc(wm->oppRobot[i].pos.loc, ROBOT_RADIUS*2);
        int hasInt = bc.intersection(seg, NULL, NULL);
        if(hasInt > 0) d += BIG_NUMBER;
    }

    return d;
}
