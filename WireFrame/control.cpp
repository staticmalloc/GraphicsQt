#include "control.h"

double Control::getAngz() const
{
    return angz;
}

void Control::setAngz(double value)
{
    angz = value;
}

double Control::getAngx() const
{
    return angx;
}

void Control::setAngx(double value)
{
    angx = value;
}

double Control::getAngy() const
{
    return angy;
}

void Control::setAngy(double value)
{
    angy = value;
}

double Control::getZf() const
{
    return Zf;
}

void Control::setZf(double value)
{
    Zf = value;
}

double Control::getZb() const
{
    return Zb;
}

void Control::setZb(double value)
{
    Zb = value;
}

int Control::getSelected() const
{
    return selected;
}

void Control::setSelected(int value)
{
    selected = value;
}

double Control::getFigureAngleX(int index)
{
    return figures[index]->getAngx();
}

double Control::getFigureAngleY(int index)
{
    return figures[index]->getAngy();
}

double Control::getFigureAngleZ(int index)
{
    return figures[index]->getAngz();
}

double Control::getFigureShiftX(int index)
{
    return figures[index]->getShiftx();
}

double Control::getFigureShiftY(int index)
{
    return figures[index]->getShifty();
}

double Control::getFigureShiftZ(int index)
{
    return figures[index]->getShiftz();
}

void Control::changeFigureAngleX(int index, double value)
{
    figures[index]->setAngx(value);
}

void Control::changeFigureAngleY(int index, double value)
{
    figures[index]->setAngy(value);
}

void Control::changeFigureAngleZ(int index, double value)
{
    figures[index]->setAngz(value);
}

void Control::changeFigureShiftX(int index, double value)
{
    figures[index]->setShiftx(value);
}

void Control::changeFigureShiftY(int index, double value)
{
    figures[index]->setShifty(value);
}

void Control::changeFigureShiftZ(int index, double value)
{
    figures[index]->setShiftz(value);
}

void Control::deleteFigure(int index)
{
    auto f = figures[index];
    figures.erase(figures.begin() + index);
    delete f;
}

double Control::getSw() const
{
    return Sw;
}

void Control::setSw(double value)
{
    Sw = value;
}

double Control::getSh() const
{
    return Sh;
}

void Control::setSh(double value)
{
    Sh = value;
}

Control::Control() : QObject(nullptr)
{

}

Control::Control(Field *m) : QObject(nullptr)
{
    mainframe = m;
}

QVector3D Control::vectMult(QVector3D v, QVector3D w)
{
    return QVector3D(v[1]*w[2]-v[2]*w[1],
                     v[2]*w[0]-v[0]*w[2],
                     v[0]*w[1]-v[1]*w[0]);
}

double Control::xtoReal(int x, double a, double b, int width)
{
    return (a + x*((b-a)/width));
}

double Control::ytoReal(int y, double c, double d, int height)
{
    return (c + y*((d-c)/height));
}

int Control::xtoPixel(double x, double a, double b, int width)
{
    int out = round((x-a)*width/(b-a));
    out = std::min(width-1,out);
    return out;
}

int Control::ytoPixel(double y, double c, double d, int height)
{
    int out = round((y-c)*height/(d-c));
    out = std::min(height-1,out);
    return out;
}

void Control::drawLine(int x1, int y1, int x2, int y2, QImage &image, QColor color)
{
    if( x1<0||x1>image.width()
            ||x2<0||x2>image.width()
            ||y1<0||y1>image.height()
            ||y2<0||y2>image.height())
    {
        return;
    }
    const unsigned int width = image.width();
    auto mass = (QRgb *)image.bits();
    int xsign = x1==x2?0:(x1<x2?1:-1);
    int ysign = y1==y2?0:(y1<y2?1:-1);
    int xsign1 = xsign, ysign1 = ysign;
    const int dX = abs(x2 - x1);
    const int dY = abs(y2 - y1);
    int max,min;
    int err = 0;
    if(dX>dY)
    {
        max = dX;
        min = dY;
        ysign1 = 0;
        xsign = 0;
    }
    else
    {
        max = dY;
        min = dX;
        xsign1 = 0;
        ysign = 0;
    }
    for(int i = 0; i < max; i++)
    {
        mass[y1 * width + x1] = color.rgb();
        x1 += xsign1;
        y1 += ysign1;
        err += 2*min;
        if(err > max)
        {
            x1 += xsign;
            y1 += ysign;
            err -= 2*max;
        }
    }
    mass[y2 * width + x2] = color.rgb();
}

void Control::ClippedLine(int x1, int y1, double z1, int x2, int y2, double z2, QImage &image, QColor color)
{
    const unsigned int width = image.width();
    auto mass = (QRgb *)image.bits();
    int xsign = x1==x2?0:(x1<x2?1:-1);
    int ysign = y1==y2?0:(y1<y2?1:-1);
    int xsign1 = xsign, ysign1 = ysign;
    const int dX = abs(x2 - x1);
    const int dY = abs(y2 - y1);
    int max,min;
    int err = 0;
    if(dX>dY)
    {
        max = dX;
        min = dY;
        ysign1 = 0;
        xsign = 0;
    }
    else
    {
        max = dY;
        min = dX;
        xsign1 = 0;
        ysign = 0;
    }
    double zstep = (z2 - z1)/max;
    for(int i = 0; i < max; i++)
    {

        if( !(x1<0 || x1 >= image.width()-1 ||
              y1<0 || y1 >= image.height()-1 ||
              z1<0 || z1>1))
            mass[y1 * width + x1] = color.rgb();
        x1 += xsign1;
        y1 += ysign1;
        err += 2*min;
        if(err > max)
        {
            x1 += xsign;
            y1 += ysign;
            err -= 2*max;
        }
        z1+=zstep;
    }
    if( !(x1<0 ||x1>image.width()-2 ||
          y1<0||y1>image.height()-2 ))
        mass[y2 * width + x2] = color.rgb();
}


void Control::addF(Figure *f)
{
    rotateLine(f);
    figures.push_back(f);
}

void Control::mkscene()
{
    int w = mainframe->width();
    double coef = Sw/Sh;
    int h = w/coef;
    scene = QImage(mainframe->width(),mainframe->height(),QImage::Format_ARGB32);
    scene.fill(Qt::white);
    for (auto i = figures.begin(); i!=figures.end(); ++i)
    {
        auto f = *i;
        auto vec = f->model;
        for(uint j = 0; j<vec.size(); ++j)
        {
            auto temp = vec[j];
            double coefx = 2.0/(f->xmax-f->xmin);
            double coefy = 2.0/(f->ymax-f->ymin);
            double coefz = 2.0/(f->zmax-f->zmin);
            temp = scale(temp,coefx,coefy,coefz);
            temp = turn(temp,f->getAngx(),f->getAngy(),f->getAngz());
            temp = shift(temp,f->getShiftx(),f->getShifty(),f->getShiftz());
            temp = turn(temp,angx,angy,angz);
            temp = camera(temp);
            temp = project(temp);
            vec[j] = temp;
        }
        showFigure(scene,vec,f->linePoints.size(),f->color);
        drawFigureCoord(f,scene);
    }
    drawMainCoord(scene);
    if(selected>=0)
        selectFigure(selected);
    mainframe->setImage(scene);
}



void Control::rotateLine(Figure *f)
{

    f->model.clear();
    double anglestep = 2*M_PI/angles;
    for(int i = 0; i < angles; ++i)
    {
        double angle = anglestep*i;
        QMatrix4x4 turn(  1,          0,            0,   0,
                          0, cos(angle),  -sin(angle),   0,
                          0, sin(angle),   cos(angle),   0,
                          0,          0,            0,   1);
        for(auto it = f->linePoints.begin(); it != f->linePoints.end(); ++it)
        {
            auto current = *it;
            QVector4D cur(current.first, current.second, 0, 1);
            QVector4D res = cur*turn;
            if(res[0]<f->xmin)
                f->xmin = res[0];
            if(res[1]<f->ymin)
                f->ymin = res[1];
            if(res[2]<f->zmin)
                f->zmin = res[2];
            if(res[0]>f->xmax)
                f->xmax = res[0];
            if(res[1]>f->ymax)
                f->ymax = res[1];
            if(res[2]>f->zmax)
                f->zmax = res[2];
            f->model.push_back(res);
        }
    }
}

QVector4D Control::shift(QVector4D v, double dx, double dy, double dz)
{
    QMatrix4x4 shift( 1, 0, 0, dx,
                      0, 1, 0, dy,
                      0, 0, 1, dz,
                      0, 0, 0, 1 );
    return shift*v;
}

QVector4D Control::turn(QVector4D v, double ax, double ay, double az)
{
    QMatrix4x4 turnz( cos(az), -sin(az),   0,   0,
                      sin(az),  cos(az),   0,   0,
                            0,        0,   1,   0,
                            0,        0,   0,   1);
    QMatrix4x4 turnx(  1,        0,          0,   0,
                       0,  cos(ax),   -sin(ax),   0,
                       0,  sin(ax),    cos(ax),   0,
                       0,        0,          0,   1);
    QMatrix4x4 turny(   cos(ay),0, sin(ay),   0,
                              0,1,       0,   0,
                       -sin(ay),0, cos(ay),   0,
                              0,0,       0,   1);
    v = v*turnx;
    v = v*turny;
    v = v*turnz;
    return v;
}

QVector4D Control::scale(QVector4D v, double coefx, double coefy, double coefz)
{
    QMatrix4x4 scale( coefx,     0,     0,     0,
                          0, coefy,     0,     0,
                          0,     0, coefz,     0,
                          0,     0,     0,     1 );
    return v*scale;
}

QVector4D Control::camera(QVector4D vect)
{
    QVector3D w = pView-pRef;
    w /= w.length();
    QVector3D u = vectMult(vUp,w);
    u /= u.length();
    QVector3D v = vectMult(w, u);
    QMatrix4x4 cam(u[0],u[1],u[2], 0,
                   v[0],v[1],v[2], 0,
                   w[0],w[1],w[2], 0,
                      0,   0,   0, 1);
    QMatrix4x4 view(1, 0, 0, -pView[0],
                    0, 1, 0, -pView[1],
                    0, 0, 1, -pView[2],
                    0, 0, 0, 1);
    cam *=view;
    QVector4D res = cam*vect;
    res[3] = 1;
    return res;
}

QVector4D Control::project(QVector4D v)
{
    QMatrix4x4 p = QMatrix4x4
            (2*Zf/Sw,       0,         0,           0,
                   0, 2*Zf/Sh,         0,           0,
                   0,       0, 1/(Zb-Zf), -Zf/(Zb-Zf),
                   0,       0,         1,           0);

    QVector4D res = p*v;
    res/=res[3];
    return res;
}


void Control::showFigure(QImage &image, std::vector<QVector4D> vect, int size, QColor c)
{
    for(uint i = 0; i < vect.size()-1; ++i)
    {
        if ((i+1)%(size) == 0)
            continue;
        auto cur = vect[i];
        auto next = vect[i+1];
        int x0 = xtoPixel(cur[0],-1,1,scene.width());
        int x1 = xtoPixel(next[0],-1,1,scene.width());
        int y0 = ytoPixel(cur[1],-1,1,scene.height());
        int y1 = ytoPixel(next[1],-1,1,scene.height());
        ClippedLine(x0,y0,cur[2],x1,y1,next[2],image,c);
    }
    double step = (size-1)/m;
    for(int i = 0; i <= m; i++)
    {
        for(int a = 0; a<angles; a++)
        {
            auto cur = vect[i*step +a*size];
            auto next = vect[i*step + ((a+1)%angles)*size];
            int x0 = xtoPixel(cur[0],-1,1,scene.width());
            int x1 = xtoPixel(next[0],-1,1,scene.width());
            int y0 = ytoPixel(cur[1],-1,1,scene.height());
            int y1 = ytoPixel(next[1],-1,1,scene.height());
            ClippedLine(x0,y0,cur[2],x1,y1,next[2],image,c);
        }
    }
    for(int a = 0; a<angles; a++)
    {
        auto cur = vect[size-1 + a*size];
        auto next = vect[size-1 + ((a+1)%angles)*size];
        int x0 = xtoPixel(cur[0],-1,1,scene.width());
        int x1 = xtoPixel(next[0],-1,1,scene.width());
        int y0 = ytoPixel(cur[1],-1,1,scene.height());
        int y1 = ytoPixel(next[1],-1,1,scene.height());
        ClippedLine(x0,y0,cur[2],x1,y1,next[2],image,c);
    }
}

void Control::drawMainCoord(QImage &image)
{
    std::vector<QVector4D> coord;
    for(int i=0; i<4; ++i)
    {
        auto cur = axis[i];
        cur = turn(cur,angx,angy,angz);
        cur = camera(cur);
        cur = project(cur);
        coord.push_back(cur);
    }
    auto zero = coord[0];
    auto x = coord[1];
    auto y = coord[2];
    auto z = coord[3];
    drawAxis(zero,x,image,Qt::red);
    drawAxis(zero,y,image,Qt::green);
    drawAxis(zero,z,image,Qt::blue);
}

void Control::drawFigureCoord(Figure *f, QImage &image)
{
    std::vector<QVector4D> coord;
    for(int i=0; i<4; ++i)
    {
        auto cur = axis[i];
        cur = turn(cur,f->getAngx(),f->getAngy(),f->getAngz());
        cur = shift(cur,f->getShiftx(),f->getShifty(),f->getShiftz());
        cur = turn(cur,angx,angy,angz);
        cur = camera(cur);
        cur = project(cur);
        coord.push_back(cur);
    }
    auto zero = coord[0];
    auto x = coord[1];
    auto y = coord[2];
    auto z = coord[3];
    drawAxis(zero,x,image,Qt::red);
    drawAxis(zero,y,image,Qt::green);
    drawAxis(zero,z,image,Qt::blue);
}

void Control::drawAxis(QVector4D cur, QVector4D next, QImage &image, QColor c)
{
    int x0 = xtoPixel(cur[0],-1,1,scene.width());
    int x1 = xtoPixel(next[0],-1,1,scene.width());
    int y0 = ytoPixel(cur[1],-1,1,scene.height());
    int y1 = ytoPixel(next[1],-1,1,scene.height());
    ClippedLine(x0,y0,cur[2],x1,y1,next[2],image,c);
}

void Control::selectFigure(int index)
{
    auto f = figures[index];
    std::vector<QVector4D> box = {QVector4D(f->xmax,f->ymax,f->zmax,1), //0
                                  QVector4D(f->xmax,f->ymax,f->zmin,1), //1
                                  QVector4D(f->xmax,f->ymin,f->zmax,1), //2
                                  QVector4D(f->xmax,f->ymin,f->zmin,1), //3
                                  QVector4D(f->xmin,f->ymax,f->zmax,1), //4
                                  QVector4D(f->xmin,f->ymax,f->zmin,1), //5
                                  QVector4D(f->xmin,f->ymin,f->zmax,1), //6
                                  QVector4D(f->xmin,f->ymin,f->zmin,1)}; //7
                                    /* {QVector4D(1,1,1,1), //0
                                      QVector4D(1,1,-1,1), //1
                                      QVector4D(1,-1,1,1), //2
                                      QVector4D(1,-1,-1,1), //3
                                      QVector4D(-1,1,1,1), //4
                                      QVector4D(-1,1,-1,1), //5
                                      QVector4D(-1,-1,1,1), //6
                                      QVector4D(-1,-1,-1,1)};*/
    for(int i = 0; i < 8; ++i)
    {
        auto cur = box[i];
        cur = turn(cur,f->getAngx(),f->getAngy(),f->getAngz());
        cur = shift(cur,f->getShiftx(),f->getShifty(),f->getShiftz());
        cur = turn(cur,angx,angy,angz);
        cur = camera(cur);
        cur = project(cur);
        box[i] = cur;
    }
    drawAxis(box[0],box[1],scene,Qt::magenta);
    drawAxis(box[0],box[2],scene,Qt::magenta);
    drawAxis(box[0],box[4],scene,Qt::magenta);
    drawAxis(box[1],box[3],scene,Qt::magenta);
    drawAxis(box[1],box[5],scene,Qt::magenta);
    drawAxis(box[4],box[6],scene,Qt::magenta);
    drawAxis(box[4],box[5],scene,Qt::magenta);
    drawAxis(box[3],box[2],scene,Qt::magenta);
    drawAxis(box[6],box[2],scene,Qt::magenta);
    drawAxis(box[7],box[3],scene,Qt::magenta);
    drawAxis(box[7],box[5],scene,Qt::magenta);
    drawAxis(box[7],box[6],scene,Qt::magenta);
}

