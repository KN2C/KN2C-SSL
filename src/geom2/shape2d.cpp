#include "shape2d.h"

Shape2D::Shape2D()
{

}

void Shape2D::AddRect(Vector2D topLeft, Vector2D bottomRight)
{
    _prects.append(rect(topLeft, bottomRight));
}

void Shape2D::AddCircle(Vector2D center, double r)
{
    _pcircles.append(Circle2D(center, r));
}

void Shape2D::SubtractRect(Vector2D topLeft, Vector2D bottomRight)
{
    _nrects.append(rect(topLeft, bottomRight));
}

void Shape2D::SubtractCircle(Vector2D center, double r)
{
    _ncircles.append(Circle2D(center, r));
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

    for (itRect = _prects.begin(); itRect != _prects.end(); ++itRect)
    {
        if((*itRect).IsInside(p))
        {
            return true;
        }
    }

    return false;
}
