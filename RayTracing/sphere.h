#ifndef SPHERE_H
#define SPHERE_H

#include <QObject>
#include "primitive.h"
#include <QVector3D>
#include <math.h>

class Sphere : public Primitive
{
    Q_OBJECT
    double r;
    QVector3D mid;
public:
    explicit Sphere(QVector3D m, double radius, QObject *parent = nullptr);
    double getR() const;
    void setR(double value);
    QVector3D getMid() const;
    void setMid(const QVector3D &value);
    bool checkCrossing(const QVector3D &begin,const QVector3D &way, double &x, double &y, double &z)const override;
    QVector3D getNorm(const QVector3D &P)const override;
};

#endif // SPHERE_H
