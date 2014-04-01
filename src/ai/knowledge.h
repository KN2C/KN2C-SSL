#ifndef KNOWLEDGE_H
#define KNOWLEDGE_H

#include <QtCore>
#include "geom.h"

class WorldModel;

class Knowledge : public QObject
{
    Q_OBJECT
public:
    explicit Knowledge(WorldModel* wm, QObject *parent = 0);
    int CountActiveAgents();
    QList<int> ActiveAgents();
    QList<int> findNearestTo(Vector2D loc);
    int findOppAttacker();

private:
    WorldModel* _wm;

signals:

public slots:

};

#endif // KNOWLEDGE_H
