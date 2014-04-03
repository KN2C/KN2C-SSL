#ifndef MAPSEARCHNODE_H
#define MAPSEARCHNODE_H

#include <QList>
#include "geom.h"
#include "3rdparty/stlastar.h"

class WorldModel;

class MapSearchNode
{
public:
    Vector2D vec;
    static WorldModel *wm;
    static bool isBallObs; //WTF
    static bool isKickObs; //WTF
    static int  selfRobot; //WTF

    MapSearchNode() {}
    MapSearchNode(Vector2D v) {vec = v;}

    float GoalDistanceEstimate( MapSearchNode &nodeGoal );
    bool IsGoal( MapSearchNode &nodeGoal );
    bool GetSuccessors( AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node );
    float GetCost( MapSearchNode &successor );
    bool IsSameState( MapSearchNode &rhs );

    static QList<Circle2D> getObsCircle();

};

#endif // MAPSEARCHNODE_H
