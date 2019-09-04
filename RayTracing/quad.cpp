#include "quad.h"

QVector3D Quad::getP1() const
{
    return p1;
}

void Quad::setP1(const QVector3D &value)
{
    p1 = value;
}

QVector3D Quad::getP2() const
{
    return p2;
}

void Quad::setP2(const QVector3D &value)
{
    p2 = value;
}

QVector3D Quad::getP3() const
{
    return p3;
}

void Quad::setP3(const QVector3D &value)
{
    p3 = value;
}

QVector3D Quad::getP4() const
{
    return p4;
}

void Quad::setP4(const QVector3D &value)
{
    p4 = value;
}

bool Quad::checkCrossing(const QVector3D &begin,const QVector3D &way, double &x, double &y, double &z) const
{
    if (!triangle1.checkCrossing(begin, way, x, y, z))
            return triangle2.checkCrossing(begin, way, x, y, z);
        return true;
}

QVector3D Quad::getNorm(const QVector3D &vect) const
{
    if(triangle1.isVectorCrossing(vect))
        return triangle1.getNorm(vect);
    else
        return triangle2.getNorm(vect);
}


Quad::Quad(QVector3D pp1, QVector3D pp2, QVector3D pp3, QVector3D pp4, QObject *parent)
    :Primitive(parent),
      triangle1(pp1,pp2,pp3),
      triangle2(pp3,pp4,pp1)
{
    p1 = pp1;
    p2 = pp2;
    p3 = pp3;
    p4 = pp4;
}
