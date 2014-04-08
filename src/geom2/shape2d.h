#ifndef SHAPE2D_H
#define SHAPE2D_H

#include <QtCore>
#include "geom.h"

class Shape2D
{
    struct rect
    {
        rect(Vector2D topLeft, Vector2D bottomRight): tl(topLeft), br(bottomRight) { }

        bool IsInside(Vector2D p)
        {
             return (p.x < br.x) && (p.x > tl.x) && (p.y > br.y) && (p.y < tl.y);
        }

        Vector2D tl;
        Vector2D br;
    };

public:
    Shape2D();

    void AddRect(Vector2D topLeft, Vector2D bottomRight);
    void AddCircle(Vector2D center, double r);

    void SubtractRect(Vector2D topLeft, Vector2D bottomRight);
    void SubtractCircle(Vector2D center, double r);

    void Clear();

    bool IsInside(Vector2D p);

private:
     QList<rect> _prects;
     QList<Circle2D> _pcircles;

     QList<rect> _nrects;
     QList<Circle2D> _ncircles;

};

#endif // SHAPE2D_H
