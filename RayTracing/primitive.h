#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <QObject>
#include <QVector3D>

class Primitive : public QObject
{
    Q_OBJECT
    double KDr;
    double KDg;
    double KDb;
    double KSr;
    double KSg;
    double KSb;
    double Power;
public:
    explicit Primitive(QObject *parent = nullptr);
    double getKDr() const;
    void setKDr(double value);
    double getKDg() const;
    void setKDg(double value);
    double getKDb() const;
    void setKDb(double value);
    double getKSr() const;
    void setKSr(double value);
    double getKSg() const;
    void setKSg(double value);
    double getKSb() const;
    void setKSb(double value);
    double getPower() const;
    void setPower(double value);
    void setSettings(double KDr,double KDg,double KDb,double KSr,double KSg,double KSb,double Power);
    virtual QVector3D getNorm(const QVector3D &P) const = 0;
    QVector3D reflect(const QVector3D &way,const QVector3D &P);
    virtual bool checkCrossing(const QVector3D &eye,const QVector3D &way, double &x, double &y, double &z) const = 0;
};

#endif // PRIMITIVE_H
