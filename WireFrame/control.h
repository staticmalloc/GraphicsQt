#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include <field.h>
#include <math.h>
#include <QVector4D>
#include <QVector3D>
#include <QMatrix4x4>
#include <float.h>
#include "figure.h"
#include "const.h"

class Control : public QObject
{
    Q_OBJECT
    Field *mainframe;
    QImage scene;
    int n;
    int m = 10;
    int angles = 16;
    double worldX = 0;
    double worldY = 0;
    double worldZ = 0;
    double angz = 0;
    double angx = 0;
    double angy = 0;
    double Sw = 16, Sh = 9, Zf = 5, Zb = 15;
    QVector3D pRef  = { 10,0,0};
    QVector3D pView = {-10,0,0};
    QVector3D vUp   = {  0,1,0};
    int selected = -1;
public:
    std::vector<QVector4D> axis = {QVector4D(0,0,0,1),
                                   QVector4D(1,0,0,1),
                                   QVector4D(0,1,0,1),
                                   QVector4D(0,0,1,1)};
    std::vector<Figure*> figures;
    Control();
    Control(Field *m);
    static QVector3D vectMult(QVector3D v, QVector3D w);
    static double xtoReal(int x, double a, double b, int width);
    static double ytoReal(int y, double c, double d, int height);
    static int xtoPixel(double x, double a, double b, int width);
    static int ytoPixel(double y, double c, double d, int height);
    static void drawLine(int x1, int y1, int x2, int y2, QImage &image, QColor color);
    static void ClippedLine(int x1, int y1, double z1, int x2, int y2, double z2, QImage &image, QColor color);
    void addF(Figure *f);
    void mkscene();
    double getAngz() const;
    void setAngz(double value);
    double getAngx() const;
    void setAngx(double value);
    double getAngy() const;
    void setAngy(double value);
    double getZf() const;
    void setZf(double value);
    void rotateLine(Figure *f);
    QVector4D shift(QVector4D v, double dx, double dy, double dz);
    QVector4D turn(QVector4D v, double ax, double ay, double az);
    QVector4D scale(QVector4D v, double coefx, double coefy, double coefz);
    QVector4D camera(QVector4D v);
    QVector4D project(QVector4D v);
    void showFigure(QImage &image, std::vector<QVector4D> vect, int size, QColor c);
    void drawMainCoord(QImage &image);
    void drawFigureCoord(Figure *f, QImage &image);
    void drawAxis(QVector4D cur, QVector4D next, QImage &image, QColor c);
    void selectFigure(int index);
    double getZb() const;
    void setZb(double value);
    int getSelected() const;
    void setSelected(int value);
    double getFigureAngleX(int index);
    double getFigureAngleY(int index);
    double getFigureAngleZ(int index);
    double getFigureShiftX(int index);
    double getFigureShiftY(int index);
    double getFigureShiftZ(int index);
    void changeFigureAngleX(int index, double value);
    void changeFigureAngleY(int index, double value);
    void changeFigureAngleZ(int index, double value);
    void changeFigureShiftX(int index, double value);
    void changeFigureShiftY(int index, double value);
    void changeFigureShiftZ(int index, double value);
    void deleteFigure(int index);

    double getSw() const;
    void setSw(double value);

    double getSh() const;
    void setSh(double value);

signals:

public slots:
};

#endif // CONTROL_H
