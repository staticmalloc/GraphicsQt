#ifndef QUAD_H
#define QUAD_H

#include <QObject>
#include "primitive.h"
#include "triangle.h"
#include <QVector3D>

class Quad : public Primitive
{
    Q_OBJECT
    QVector3D p1;
    QVector3D p2;
    QVector3D p3;
    QVector3D p4;
    Triangle triangle1;
    Triangle triangle2;
public:
    explicit Quad(QVector3D pp1,QVector3D pp2,QVector3D pp3, QVector3D pp4, QObject *parent = nullptr);
    QVector3D getP1() const;
    void setP1(const QVector3D &value);
    QVector3D getP2() const;
    void setP2(const QVector3D &value);
    QVector3D getP3() const;
    void setP3(const QVector3D &value);
    QVector3D getP4() const;
    void setP4(const QVector3D &value);
    bool checkCrossing(const QVector3D &begin,const QVector3D &way, double &x, double &y, double &z) const override;
    QVector3D getNorm(const QVector3D &vect) const override;
};

#endif // QUAD_H
