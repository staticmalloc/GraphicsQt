#include "primitive.h"

double Primitive::getKDr() const
{
    return KDr;
}

void Primitive::setKDr(double value)
{
    KDr = value;
}

double Primitive::getKDg() const
{
    return KDg;
}

void Primitive::setKDg(double value)
{
    KDg = value;
}

double Primitive::getKDb() const
{
    return KDb;
}

void Primitive::setKDb(double value)
{
    KDb = value;
}

double Primitive::getKSr() const
{
    return KSr;
}

void Primitive::setKSr(double value)
{
    KSr = value;
}

double Primitive::getKSg() const
{
    return KSg;
}

void Primitive::setKSg(double value)
{
    KSg = value;
}

double Primitive::getKSb() const
{
    return KSb;
}

void Primitive::setKSb(double value)
{
    KSb = value;
}

double Primitive::getPower() const
{
    return Power;
}

void Primitive::setPower(double value)
{
    Power = value;
}

void Primitive::setSettings(double KDr, double KDg, double KDb, double KSr, double KSg, double KSb, double Power)
{
    this->KDr = KDr;
    this->KDg = KDg;
    this->KDb = KDb;
    this->KSr = KSr;
    this->KSg = KSg;
    this->KSb = KSb;
    this->Power = Power;
}

QVector3D Primitive::reflect(const QVector3D &way,const QVector3D &P)
{
    QVector3D n = getNorm(P);
    return 2 * QVector3D::dotProduct(n, -way) * n + way;
}

Primitive::Primitive(QObject *parent)
    : QObject(parent)
{

}
