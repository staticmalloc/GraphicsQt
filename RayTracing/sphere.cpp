#include "sphere.h"

double Sphere::getR() const
{
    return r;
}

void Sphere::setR(double value)
{
    r = value;
}

QVector3D Sphere::getMid() const
{
    return mid;
}

void Sphere::setMid(const QVector3D &value)
{
    mid = value;
}

bool Sphere::checkCrossing(const QVector3D &begin, const QVector3D &way, double &x, double &y, double &z) const
{
    /*
    double B = QVector3D::dotProduct(way,(begin-mid));
    double C = QVector3D::dotProduct(begin-mid,begin-mid)- r*r;
    double D = B*B - C;
    if(D<0)
        return false;
    else
    {
        double t;
        double t0 = -B - sqrt(D);
        if(t0<0)
        {
            double t1 = -B + sqrt(D);
            if(t1<0)
                return false;
            else
                t = t1;
        }
        else
            t = t0;
        QVector3D res = begin + way*t;
        x = res.x();
        y = res.y();
        z = res.z();
        return true;
    }
    return false;
    */
    QVector3D OC = mid - begin;
    double OC2 = QVector3D::dotProduct(OC, OC);

    if (OC2 < r*r - 10e-9f)
        return false;

    double tsv = QVector3D::dotProduct(OC, way);
    if (tsv < 0)
        return false;

    double thc2 = r*r - OC2 + tsv*tsv;
    if (thc2 < 0)
        return false;

    double t = tsv - sqrt(thc2);


    QVector3D P = begin + t * way;
    x = P.x();
    y = P.y();
    z = P.z();

    return true;
}

QVector3D Sphere::getNorm(const QVector3D &P) const
{
    return (P-mid).normalized();
}


Sphere::Sphere(QVector3D m, double radius, QObject *parent)
    : Primitive(parent)
{
    mid = m;
    r = radius;
}
