#ifndef MAPSEARCHNODE_H
#define MAPSEARCHNODE_H

#include "geom.h"
#include "3rdparty/stlastar.h"

class MapSearchNode
{
public:
    Vector2D vec;

    MapSearchNode() {}
    MapSearchNode(Vector2D v) {vec = v;}

    float GoalDistanceEstimate( MapSearchNode &nodeGoal );
    bool IsGoal( MapSearchNode &nodeGoal );
    bool GetSuccessors( AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node );
    float GetCost( MapSearchNode &successor );
    bool IsSameState( MapSearchNode &rhs );
};

#endif // MAPSEARCHNODE_H
