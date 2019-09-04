#ifndef BOX_H
#define BOX_H

#include <QObject>
#include "primitive.h"
#include <QVector3D>

class Box : public Primitive
{
    Q_OBJECT
    QVector3D min;
    QVector3D max;
    std::vector<QVector3D> points;
public:
    explicit Box(QVector3D mini, QVector3D maxi, QObject *parent = nullptr);
    double getDx();
    double getDy();
    double getDz();
    QVector3D getMin() const;
    void setMin(const QVector3D &value);
    QVector3D getMax() const;
    void setMax(const QVector3D &value);
    void countPoints();
    std::vector<QVector3D> getPoints() const;
    bool checkCrossing(const QVector3D &eye,const QVector3D &way, double &x, double &y, double &z)const override;
    QVector3D getNorm(const QVector3D &P)const override;

private:
    void countPoints(QVector3D min, QVector3D max);
};

#endif // BOX_H
