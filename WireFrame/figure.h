#ifndef FIGURE_H
#define FIGURE_H

#include <QVector4D>
#include <float.h>
#include <math.h>
#include <QColor>

class Figure
{
    double shiftx = 0;
    double shifty = 0;
    double shiftz = 0;
    double angx = 0;
    double angy = 0;
    double angz = 0;
public:
    double xmin = DBL_MAX;
    double ymin = DBL_MAX;
    double zmin = DBL_MAX;
    double xmax = -DBL_MAX;
    double ymax = -DBL_MAX;
    double zmax = -DBL_MAX;
    QColor color = Qt::black;
    std::vector<QVector4D> model;
    std::vector<std::pair<double,double>> bSplinePoints;
    std::vector<std::pair<double,double>> linePoints;
    Figure();
    Figure(std::vector<std::pair<double,double>> line,std::vector<std::pair<double,double>> bSpline, QColor c);
    double getShiftx() const;
    void setShiftx(double value);
    double getShifty() const;
    void setShifty(double value);
    double getShiftz() const;
    void setShiftz(double value);
    double getAngx() const;
    void setAngx(double value);
    double getAngy() const;
    void setAngy(double value);
    double getAngz() const;
    void setAngz(double value);
};

#endif // FIGURE_H
