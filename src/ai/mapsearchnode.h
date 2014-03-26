#ifndef MAPSEARCHNODE_H
#define MAPSEARCHNODE_H

#include "geom.h"
#include "3rdparty/stlastar.h"

class WorldModel;

class MapSearchNode
{
public:
    Vector2D vec;
    static WorldModel *wm;
    static bool isBallObs; //WTF

    MapSearchNode() {isBallObs=false;}
    MapSearchNode(Vector2D v) {vec = v;}

    float GoalDistanceEstimate( MapSearchNode &nodeGoal );
    bool IsGoal( MapSearchNode &nodeGoal );
    bool GetSuccessors( AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node );
    float GetCost( MapSearchNode &successor );
    bool IsSameState( MapSearchNode &rhs );
};

#endif // MAPSEARCHNODE_H
