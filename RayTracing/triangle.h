#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QObject>
#include "primitive.h"
#include <QVector3D>
#include <QVector3D>

class Triangle : public Primitive
{
    Q_OBJECT
    QVector3D p1;
    QVector3D p2;
    QVector3D p3;
    QVector3D norm;
public:
    explicit Triangle(QVector3D pp1,QVector3D pp2,QVector3D pp3, QObject *parent = nullptr);
    QVector3D getP1() const;
    void setP1(const QVector3D &value);
    QVector3D getP2() const;
    void setP2(const QVector3D &value);
    QVector3D getP3() const;
    void setP3(const QVector3D &value);
    bool checkCrossing(const QVector3D &eye,const QVector3D &way, double &x, double &y, double &z)const override;
    QVector3D getNorm(const QVector3D &P)const override;
    bool isVectorCrossing(const QVector3D &vect) const;
};

#endif // TRIANGLE_H
