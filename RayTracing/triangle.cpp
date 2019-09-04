#include "triangle.h"

QVector3D Triangle::getP1() const
{
    return p1;
}

void Triangle::setP1(const QVector3D &value)
{
    p1 = value;
}

QVector3D Triangle::getP2() const
{
    return p2;
}

void Triangle::setP2(const QVector3D &value)
{
    p2 = value;
}

QVector3D Triangle::getP3() const
{
    return p3;
}

void Triangle::setP3(const QVector3D &value)
{
    p3 = value;
}

bool Triangle::checkCrossing(const QVector3D &eye,const QVector3D &way, double &x, double &y, double &z) const
{
    double D = QVector3D::dotProduct(norm,p1);
    double vd = QVector3D::dotProduct(norm, way);
    if (vd >= 0)
        return false;

    double v0 = -QVector3D::dotProduct(norm, eye) + D;
    double t = v0 / vd;

    if (t < 0)
        return false;

    QVector3D l1 = p2-p1;
    QVector3D l2 = p3-p2;
    QVector3D l3 = p1-p3;

    QVector3D P = eye + t * way;


    if (QVector3D::dotProduct(QVector3D::crossProduct(l1, P - p1), norm) < 0)
        return false;
    if (QVector3D::dotProduct(QVector3D::crossProduct(l2, P - p2), norm) < 0)
        return false;
    if (QVector3D::dotProduct(QVector3D::crossProduct(l3, P - p3), norm) < 0)
        return false;
    x = P.x();
    y = P.y();
    z = P.z();
    return true;
}

QVector3D Triangle::getNorm(const QVector3D &P) const
{
    return norm;
}

bool Triangle::isVectorCrossing(const QVector3D &vect) const
{
    double D = QVector3D::dotProduct(norm,p1);
    if(std::abs(QVector3D::dotProduct(norm, vect) - D) < 10e-6f)
        return true;
    else
        return false;
}


Triangle::Triangle(QVector3D pp1, QVector3D pp2, QVector3D pp3, QObject *parent)
    :Primitive(parent)
{
    p1 = pp1;
    p2 = pp2;
    p3 = pp3;
    norm = QVector3D::crossProduct(p2-p1,p3-p1).normalized();
}
