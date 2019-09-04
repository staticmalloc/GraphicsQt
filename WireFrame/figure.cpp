#include "figure.h"

double Figure::getShiftx() const
{
    return shiftx;
}

void Figure::setShiftx(double value)
{
    shiftx = value;
}

double Figure::getShifty() const
{
    return shifty;
}

void Figure::setShifty(double value)
{
    shifty = value;
}

double Figure::getShiftz() const
{
    return shiftz;
}

void Figure::setShiftz(double value)
{
    shiftz = value;
}

double Figure::getAngx() const
{
    return angx;
}

void Figure::setAngx(double value)
{
    angx = value;
}

double Figure::getAngy() const
{
    return angy;
}

void Figure::setAngy(double value)
{
    angy = value;
}

double Figure::getAngz() const
{
    return angz;
}

void Figure::setAngz(double value)
{
    angz = value;
}

Figure::Figure()
{
    
}

Figure::Figure(std::vector<std::pair<double, double> > line, std::vector<std::pair<double,double>> bSpline, QColor c)
{
    linePoints = std::move(line);
    bSplinePoints = std::move(bSpline);
    color = c;
}
