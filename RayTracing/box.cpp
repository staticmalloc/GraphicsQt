#include "box.h"

QVector3D Box::getMin() const
{
    return min;
}

void Box::setMin(const QVector3D &value)
{
    min = value;
}

QVector3D Box::getMax() const
{
    return max;
}

void Box::setMax(const QVector3D &value)
{
    max = value;
}

void Box::countPoints()
{
    if(!points.empty())
        return;
    countPoints(min,max);
}

void Box::countPoints(QVector3D min, QVector3D max)
{
    points.clear();
    points.push_back(min);
    points.push_back(QVector3D(min[0],min[1],max[2]));
    points.push_back(QVector3D(min[0],max[1],min[2]));
    points.push_back(QVector3D(min[0],max[1],max[2]));
    points.push_back(QVector3D(max[0],min[1],min[2]));
    points.push_back(QVector3D(max[0],min[1],max[2]));
    points.push_back(QVector3D(max[0],max[1],min[2]));
    points.push_back(max);
}

std::vector<QVector3D> Box::getPoints() const
{
    return points;
}

bool Box::checkCrossing(const QVector3D &eye,const QVector3D &way, double &x, double &y, double &z) const
{

}

QVector3D Box::getNorm(const QVector3D &P) const
{

}




Box::Box(QVector3D mini, QVector3D maxi, QObject *parent)
    :Primitive(parent)
{
    min = mini;
    max = maxi;
    countPoints(mini,maxi);
}

double Box::getDx()
{
    return (max[0]-min[0]);
}

double Box::getDy()
{
    return (max[1]-min[1]);
}

double Box::getDz()
{
    return (max[2]-min[2]);
}
