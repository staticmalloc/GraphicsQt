#ifndef CONTROL_H
#define CONTROL_H


#include <QObject>
#include "primitive.h"
#include "box.h"
#include "triangle.h"
#include "quad.h"
#include "sphere.h"
#include <vector>
#include <math.h>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include "field.h"
#include <QDataStream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <QtConcurrent/QtConcurrent>
#include <pthread.h>


class Control: public QObject
{
    Q_OBJECT
    Field *frame;
    QImage scene;
    QColor argblight;
    QColor background = Qt::black;
    std::vector<std::pair<QVector3D,QColor>> light;
    std::vector<Box*> boxes;
    std::vector<Quad*> quads;
    std::vector<Sphere*> spheres;
    std::vector<Triangle*> triangles;
    int wdth = 800, hght = 800;
    double defSw = 8.88,
           defSh = 5,
           defZf = 4.747500000000005,
           defZb = 15;
    double Sw = 8.88,
           Sh = 5,
           Zf = 4.747500000000005,
           Zb = 15;
    QVector3D pRef  = { -1.068364182650456, -5.713436742344853, 9.602876577591509};
    QVector3D pView = { 5.962909074683145, 9.398918066708948, -1.4505359500390993};
    QVector3D vUp   = { -3.202640205902993, -7.094666932505519, 6.356791394119225};
    QVector3D defpRef  = { -1.068364182650456, -5.713436742344853, 9.602876577591509};
    QVector3D defpView = { 5.962909074683145, 9.398918066708948, -1.4505359500390993};
    QVector3D defvUp   = { -3.202640205902993, -7.094666932505519, 6.356791394119225};
    double angz = 0;
    double angx = 0;
    double angy = 0;
    std::vector<QVector4D> axis = {QVector4D(0,0,0,1),
                                   QVector4D(1,0,0,1),
                                   QVector4D(0,1,0,1),
                                   QVector4D(0,0,1,1)};
    enum Quality {
            Rough, Normal, Fine
        };
    Quality quality = Rough;
    double gamma = 1.0;
    uint tracingDepth = 3;
    volatile float proc = 0.0;
signals:
    void rendering(int progress);
    void done();
public:
    explicit Control(Field *f,QObject *parent = nullptr);
    static QVector3D vectMult(QVector3D v, QVector3D w);
    double xtoReal(int x);
    double ytoReal(int y);
    static int xtoPixel(double x, double a, double b, int width);
    static int ytoPixel(double y, double c, double d, int height);
    static QVector4D to4D(const QVector3D in);
    static void drawLine(int x1, int y1, int x2, int y2, QImage &image, QColor color);
    static void ClippedLine(int x1, int y1, double z1, int x2, int y2, double z2, QImage &image, QColor color);
    void makeScene();
    void render();
    void toInit();
    void turnScene(double dx, double dy);
    QVector4D shift(QVector4D v, double dx, double dy, double dz);
    QVector4D turn(QVector4D v, double ax, double ay, double az);
    QVector4D scale(QVector4D v, double coefx, double coefy, double coefz);
    QVector4D camera(QVector4D v);
    QVector4D project(QVector4D v);
    void drawMainCoord(QImage &image);
    void drawAxis(QVector3D cur, QVector3D next, QImage &image, QColor c);
    void drawAxis(QVector4D cur, QVector4D next, QImage &image, QColor c);
    void openState(std::string filename);
    void loadRSettings(std::string filename);
    double getAngz() const;
    void setAngz(double value);

    double getAngx() const;
    void setAngx(double value);

    double getAngy() const;
    void setAngy(double value);

    double getZf() const;
    void setZf(double value);

    QColor getBackground() const;
    void setBackground(const QColor &value);
    void* beginPtracing(void *arg);

    volatile float getProc() const;
    void setProc(volatile float value);
    void changeSettings(int width,int height,int depth,int qual,QColor background,double gamma,int threads);
    QVector3D getPRef() const;
    void setPRef(const QVector3D &value);

    QVector3D getPView() const;
    void setPView(const QVector3D &value);

    QVector3D getVUp() const;
    void setVUp(const QVector3D &value);

private:
    void drawBoxes();
    void drawTriangles();
    void drawQuads();
    void drawSpheres();
    std::vector<QVector4D> mkSphere(Sphere *s);
    void runPthreads();

    void beginTracing();
    QVector3D rayFromScreen(double x, double y, double &progress, double progress_step);
    void traceRay(int depth,const QVector3D &begin,const QVector3D &way, double &r, double &g, double &b);
    bool checkPrimitives(const QVector3D &begin,const QVector3D &way,double &x, double &y, double &z,
                                    Primitive **primitive);
};

#endif // CONTROL_H
