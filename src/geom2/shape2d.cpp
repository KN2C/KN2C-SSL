#include "shape2d.h"

Shape2D::Shape2D()
{

}

void Shape2D::Clear()
{
    _ncircles.clear();
    _nrects.clear();

    _pcircles.clear();
    _prects.clear();
}

bool Shape2D::IsInside(Vector2D p)
{
    QList<Circle2D>::iterator itCircle;
    QList<rect>::iterator itRect;

    for (itCircle = _ncircles.begin(); itCircle != _ncircles.end(); ++itCircle)
    {
        if((*itCircle).center().dist(p) < (*itCircle).radius())
        {
            return false;
        }
    }

    for (itRect = _nrects.begin(); itRect != _nrects.end(); ++itRect)
    {
        if((*itRect).IsInside(p))
        {
            return false;
        }
    }

    for (itCircle = _pcircles.begin(); itCircle != _pcircles.end(); ++itCircle)
    {
        if((*itCircle).center().dist(p) < (*itCircle).radius())
        {
            return true;
        }
    }

    for (itRect = _nrects.begin(); itRect != _nrects.end(); ++itRect)
    {
        if((*itRect).IsInside(p))
        {
            return true;
        }
    }

    return false;
}
