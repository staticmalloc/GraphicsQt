#include "control.h"
int amount = 16;
void* abrakadabra(void *_arg)
{
    std::pair<Control*, int> *arg = (std::pair<Control*, int>*) _arg;
    void* res =  arg->first->beginPtracing((void*)&arg->second);
    delete arg;
    return res;
}

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

QColor Control::getBackground() const
{
    return background;
}

void Control::setBackground(const QColor &value)
{
    background = value;
}


volatile float Control::getProc() const
{
    return proc;
}

void Control::setProc(volatile float value)
{
    proc = value;
}

void Control::changeSettings(int width, int height, int depth, int qual, QColor background, double gamma, int threads)
{
    wdth = width;
    hght = height;
    tracingDepth = depth;
    switch (qual)
    {
    case 2:
        quality = Normal;
        break;
    case 3:
        quality = Fine;
        break;
    default:
        quality = Rough;
        break;
    }
    this->background = background;
    this->gamma = gamma;
    amount = threads;
    makeScene();
}

QVector3D Control::getPRef() const
{
    return pRef;
}

void Control::setPRef(const QVector3D &value)
{
    pRef = value;
}

QVector3D Control::getPView() const
{
    return pView;
}

void Control::setPView(const QVector3D &value)
{
    pView = value;
}

QVector3D Control::getVUp() const
{
    return vUp;
}

void Control::setVUp(const QVector3D &value)
{
    vUp = value;
}

Control::Control(Field *f, QObject *parent) : QObject(parent)
{
    frame = f;
}

QVector3D Control::vectMult(QVector3D v, QVector3D w)
{
    return QVector3D(v[1]*w[2]-v[2]*w[1],
                     v[2]*w[0]-v[0]*w[2],
                     v[0]*w[1]-v[1]*w[0]);
}

double Control::xtoReal(int x)
{
    double wh = scene.width() / 2;
    return double(x - wh) / wh * (Sw / 2);;
}

double Control::ytoReal(int y)
{
    double hh = scene.height() / 2;
    return -(double)(y - hh) / hh * (Sh / 2);
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

QVector4D Control::to4D(const QVector3D in)
{
    return QVector4D(in,1);
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

void Control::makeScene()
{
    int w = wdth;//frame->width();
    int h = hght;//frame->height();
    double coef = (double)w/h;
    Sw = Sh*coef;
    scene = QImage(w, h,QImage::Format_ARGB32);
    scene.fill(Qt::white);
    drawBoxes();
    drawTriangles();
    drawQuads();
    drawSpheres();
    drawMainCoord(scene);
    frame->setImage(scene);
}

void Control::render()
{
    QtConcurrent::run(this, &Control::runPthreads);
}

void Control::toInit()
{
    angx = 0;
    angy = 0;
    angz = 0;
    pRef = defpRef;
    pView = defpView;
    vUp = defvUp;
    Zb = defZb;
    Zf = defZf;
    Sw = defSw;
    Sh = defSh;
    makeScene();
}

void Control::turnScene(double dx, double dy)
{
    QMatrix4x4 turnx(  1,        0,          0,   0,
                       0,  cos(dx),   -sin(dx),   0,
                       0,  sin(dx),    cos(dx),   0,
                       0,        0,          0,   1);
    QMatrix4x4 turny(   cos(dy),0, sin(dy),   0,
                              0,1,       0,   0,
                       -sin(dy),0, cos(dy),   0,
                              0,0,       0,   1);
    auto forward = (pRef - pView);
    QVector3D w = -forward.normalized();
    QVector3D u = QVector3D::crossProduct(w,vUp).normalized();
    QVector3D v = QVector3D::crossProduct(w,u);
    QMatrix4x4 cam(u[0],u[1],u[2], 0,
                   v[0],v[1],v[2], 0,
                   w[0],w[1],w[2], 0,
                      0,   0,   0, 1);
    QVector4D rot = cam.transposed()*turnx*turny*cam*to4D(w);
    QVector3D res = QVector3D(rot.x(),rot.y(),rot.z());
    pRef = res + pView;
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
    QVector3D w = (pView-pRef).normalized();
    QVector3D u = QVector3D::crossProduct(w,vUp).normalized();
    QVector3D v = QVector3D::crossProduct(w,u);

    QMatrix4x4 cam(u[0],u[1],u[2], 0,
                   v[0],v[1],v[2], 0,
                   w[0],w[1],w[2], 0,
                      0,   0,   0, 1);
    QVector4D eye(pRef[0],pRef[1],pRef[2],1);
    QVector4D shift = cam*eye;
    QMatrix4x4 view(1, 0, 0, -shift[0],
                    0, 1, 0, -shift[1],
                    0, 0, 1, -shift[2],
                    0, 0, 0, 1);
//    QMatrix4x4 view(1, 0, 0, -pRef[0],
//                    0, 1, 0, -pRef[1],
//                    0, 0, 1, -pRef[2],
//                    0, 0, 0, 1);
//    cam *=view;
    QMatrix4x4 turnx(  1,        0,          0,   0,
                       0,  cos(angx),   -sin(angx),   0,
                       0,  sin(angx),    cos(angx),   0,
                       0,        0,          0,   1);
    QMatrix4x4 turny(   cos(angy),0, sin(angy),   0,
                              0,1,       0,   0,
                       -sin(angy),0, cos(angy),   0,
                              0,0,       0,   1);
    auto turn = turnx*turny;
    cam = view * turn * cam;
    QVector4D res = cam*vect;
    res[3] = 1;
    return res;
}

QVector4D Control::project(QVector4D v)
{
    QMatrix4x4 p = QMatrix4x4
            (2*Zf/Sw,       0,         0,           0,
                   0, 2*Zf/Sh,         0,           0,
                   0,       0, Zb/(Zb-Zf), -Zf*Zb/(Zb-Zf),
                   0,       0,         1,           0);

    QVector4D res = p*v;
    res/=res[3];
    return res;
}
void Control::drawMainCoord(QImage &image)
{
    std::vector<QVector4D> coord;
    for(int i=0; i<4; ++i)
    {
        auto cur = axis[i];
        //cur = turn(cur,angx,angy,angz);
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

void Control::drawAxis(QVector3D cur, QVector3D next, QImage &image, QColor c)
{
    int x0 = xtoPixel(cur[0],-1,1,scene.width());
    int x1 = xtoPixel(next[0],-1,1,scene.width());
    int y0 = ytoPixel(cur[1],-1,1,scene.height());
    int y1 = ytoPixel(next[1],-1,1,scene.height());
    ClippedLine(x0,y0,cur[2],x1,y1,next[2],image,c);
}

void Control::drawAxis(QVector4D cur, QVector4D next, QImage &image, QColor c)
{
    int x0 = xtoPixel(cur[0],-1,1,scene.width());
    int x1 = xtoPixel(next[0],-1,1,scene.width());
    int y0 = ytoPixel(cur[1],-1,1,scene.height());
    int y1 = ytoPixel(next[1],-1,1,scene.height());
    ClippedLine(x0,y0,cur[2],x1,y1,next[2],image,c);
}

void Control::openState(std::string filename)
{
    for(auto i:spheres)
    {
        delete i;
    }
    spheres.clear();
    for(auto i:boxes)
    {
        delete i;
    }
    boxes.clear();
    for(auto i:triangles)
    {
        delete i;
    }
    triangles.clear();
    for(auto i:quads)
    {
        delete i;
    }
    quads.clear();
    light.clear();
    int NL;
    uint r,g,b;
    double LX, LY, LZ;
    double x,y,z;
    double rad;
    QVector3D p1, p2, p3, p4;
    Triangle *tr;
    Box *box;
    Sphere *s;
    Quad *quad;
    double KDr,KDg,KDb,KSr,KSg,KSb,Power;
    std::ifstream in(filename);
    std::string line;
    std::string figureType;
    std::stringstream stream;
    int count = 0;
    int key = 0;
    bool flag = true;
    while (flag && std::getline(in, line))
    {
        count++;
        size_t pos = line.find("//");
        if (pos != std::string::npos)
        {
            line.erase(pos);
        }
        stream.clear();
        stream.str(line);
        switch(key)
        {
        case 0:
            stream >> r >> g >> b;
            argblight = QColor(r,g,b);
            ++key;
            break;
        case 1:
            stream >> NL;
            ++key;
            break;
        case 2:
            NL--;
            stream >> LX >> LY >> LZ >> r >> g >> b;
            light.push_back(std::pair<QVector3D,QColor>(QVector3D(LX,LY,LZ),QColor(r,g,b)));
            if(NL == 0)
                ++key;
            break;
        default:
            figureType = "";
            stream >> figureType;
            if(figureType == "SPHERE")
            {
                count++;
                std::getline(in, line);
                pos = line.find("//");
                if (pos != std::string::npos)
                    line.erase(pos);
                stream.clear();
                stream.str(line);
                stream >> x >> y >> z;
                p1 = QVector3D(x,y,z);

                std::getline(in, line);count++;
                pos = line.find("//");
                if (pos != std::string::npos)
                    line.erase(pos);
                stream.clear();
                stream.str(line);
                stream >> rad;

                std::getline(in, line);count++;
                pos = line.find("//");
                if (pos != std::string::npos)
                    line.erase(pos);
                stream.clear();
                stream.str(line);
                stream >> KDr >> KDg >> KDb >> KSr >> KSg >> KSb >> Power;
                s = new Sphere(p1,rad);
                s->setSettings(KDr,KDg,KDb,KSr,KSg,KSb,Power);
                spheres.push_back(s);
                break;
            }
            else if(figureType == "BOX")
            {
                std::getline(in, line);count++;
                pos = line.find("//");
                if (pos != std::string::npos)
                    line.erase(pos);
                stream.clear();
                stream.str(line);
                stream >> x >> y >> z;
                p1 = QVector3D(x,y,z);

                std::getline(in, line);count++;
                pos = line.find("//");
                if (pos != std::string::npos)
                    line.erase(pos);
                stream.clear();
                stream.str(line);
                stream >> x >> y >> z;
                p2 = QVector3D(x,y,z);

                std::getline(in, line);count++;
                pos = line.find("//");
                if (pos != std::string::npos)
                    line.erase(pos);
                stream.clear();
                stream.str(line);
                stream >> KDr >> KDg >> KDb >> KSr >> KSg >> KSb >> Power;
                box = new Box(p1,p2);
                box->setSettings(KDr,KDg,KDb,KSr,KSg,KSb,Power);
                boxes.push_back(box);
                break;
            }
            else if(figureType == "TRIANGLE")
            {
                std::getline(in, line);count++;
                pos = line.find("//");
                if (pos != std::string::npos)
                    line.erase(pos);
                stream.clear();
                stream.str(line);
                stream >> x >> y >> z;
                p1 = QVector3D(x,y,z);

                std::getline(in, line);count++;
                pos = line.find("//");
                if (pos != std::string::npos)
                    line.erase(pos);
                stream.clear();
                stream.str(line);
                stream >> x >> y >> z;
                p2 = QVector3D(x,y,z);

                std::getline(in, line);count++;
                pos = line.find("//");
                if (pos != std::string::npos)
                    line.erase(pos);
                stream.clear();
                stream.str(line);
                stream >> x >> y >> z;
                p3 = QVector3D(x,y,z);

                std::getline(in, line);count++;
                pos = line.find("//");
                if (pos != std::string::npos)
                    line.erase(pos);
                stream.clear();
                stream.str(line);
                stream >> KDr >> KDg >> KDb >> KSr >> KSg >> KSb >> Power;
                tr = new Triangle(p1,p2,p3);
                tr->setSettings(KDr,KDg,KDb,KSr,KSg,KSb,Power);
                triangles.push_back(tr);
                break;
            }
            else if(figureType == "QUADRANGLE")
            {
                std::getline(in, line);count++;
                pos = line.find("//");
                if (pos != std::string::npos)
                    line.erase(pos);
                stream.clear();
                stream.str(line);
                stream >> x >> y >> z;
                p1 = QVector3D(x,y,z);

                std::getline(in, line);count++;
                pos = line.find("//");
                if (pos != std::string::npos)
                    line.erase(pos);
                stream.clear();
                stream.str(line);
                stream >> x >> y >> z;
                p2 = QVector3D(x,y,z);

                std::getline(in, line);count++;
                pos = line.find("//");
                if (pos != std::string::npos)
                    line.erase(pos);
                stream.clear();
                stream.str(line);
                stream >> x >> y >> z;
                p3 = QVector3D(x,y,z);

                std::getline(in, line);count++;
                pos = line.find("//");
                if (pos != std::string::npos)
                    line.erase(pos);
                stream.clear();
                stream.str(line);
                stream >> x >> y >> z;
                p4 = QVector3D(x,y,z);

                std::getline(in, line);count++;
                pos = line.find("//");
                if (pos != std::string::npos)
                    line.erase(pos);
                stream.clear();
                stream.str(line);
                stream >> KDr >> KDg >> KDb >> KSr >> KSg >> KSb >> Power;
                quad = new Quad(p1,p2,p3,p4);
                quad->setSettings(KDr,KDg,KDb,KSr,KSg,KSb,Power);
                quads.push_back(quad);
                break;
            }
            else
            {
                std::cerr << "UNKNOWN PRIMITIVE TYPE" << std::endl;
                break;
            }
        }

   }
    makeScene();
}

void Control::loadRSettings(std::string filename)
{
    uint r, g, b;
    double p1,p2,p3;
    std::ifstream in(filename);
    std::string line;
    std::string str;
    QString _str;
    std::stringstream stream;
    int key = 0;
    bool flag = true;
    while (flag && std::getline(in, line))
    {
        size_t pos = line.find("//");
        if (pos != std::string::npos)
        {
            line.erase(pos);
        }
        stream.clear();
        stream.str(line);
        switch(key)
        {
        case 0:
            stream >> r >> g >> b;
            background = QColor(r,g,b);
            break;
        case 1:
            stream >> gamma;
            break;
        case 2:
            stream >> tracingDepth;
            break;
        case 3:
            stream >> str;
            _str = QString::fromStdString(str);
            _str.toUpper();
            if(_str == "FINE")
                quality = Fine;
            if(_str == "NORMAL")
                quality = Normal;
            if(_str == "ROUGH")
                quality = Rough;
            break;
        case 4:
            stream>>p1>>p2>>p3;
            defpRef = QVector3D(p1,p2,p3);
            break;
        case 5:
            stream>>p1>>p2>>p3;
            defpView = QVector3D(p1,p2,p3);
            break;
        case 6:
            stream>>p1>>p2>>p3;
            defvUp = QVector3D(p1,p2,p3);
            break;
        case 7:
            stream>>p1>>p2;
            defZf = p1;
            defZb = p2;
            break;
        case 8:
            stream>>p1>>p2;
            defSw = p1;
            defSh = p2;
            break;
        default:
            if(key!=9)
                std::cerr << "RENDER SETTINGS FILE UNEXPETEDLY ENDED" << std::endl;
                break;
        }
        key++;
    }
    pRef = defpRef;
    pView = defpView;
    vUp = defvUp;
    Zb = defZb;
    Zf = defZf;
    Sw = defSw;
    Sh = defSh;
}


void Control::drawBoxes()
{
    for(auto i = boxes.begin(); i != boxes.end(); ++i)
    {
        Box *cur = *i;
        cur->countPoints();
        auto box = cur->getPoints();
        for(uint j = 0; j<box.size(); ++j)
        {
            auto temp = to4D(box[j]);
            //temp = scale(temp,coefx,coefy,coefz);
            //temp = turn(temp,angx,angy,angz);
            temp = camera(temp);
            temp = project(temp);
            box[j] = temp.toVector3D();
        }
        drawAxis(box[0],box[1],scene,Qt::black);
        drawAxis(box[0],box[2],scene,Qt::black);
        drawAxis(box[0],box[4],scene,Qt::black);
        drawAxis(box[1],box[3],scene,Qt::black);
        drawAxis(box[1],box[5],scene,Qt::black);
        drawAxis(box[4],box[6],scene,Qt::black);
        drawAxis(box[4],box[5],scene,Qt::black);
        drawAxis(box[3],box[2],scene,Qt::black);
        drawAxis(box[6],box[2],scene,Qt::black);
        drawAxis(box[7],box[3],scene,Qt::black);
        drawAxis(box[7],box[5],scene,Qt::black);
        drawAxis(box[7],box[6],scene,Qt::black);
    }
}

void Control::drawTriangles()
{
    for(auto i = triangles.begin(); i != triangles.end(); ++i)
    {
        Triangle *t = *i;
        std::vector<QVector3D> vec;
        vec.push_back(t->getP1());
        vec.push_back(t->getP2());
        vec.push_back(t->getP3());
        for(uint j = 0; j<vec.size(); ++j)
        {
            auto temp = to4D(vec[j]);
            //temp = turn(temp,angx,angy,angz);
            temp = camera(temp);
            temp = project(temp);
            vec[j] = temp.toVector3D();
        }
        drawAxis(vec[0],vec[1],scene,Qt::black);
        drawAxis(vec[1],vec[2],scene,Qt::black);
        drawAxis(vec[0],vec[2],scene,Qt::black);
    }
}

void Control::drawQuads()
{
    for(auto i = quads.begin(); i != quads.end(); ++i)
    {
        Quad *t = *i;
        std::vector<QVector3D> vec;
        vec.push_back(t->getP1());
        vec.push_back(t->getP2());
        vec.push_back(t->getP3());
        vec.push_back(t->getP4());
        for(uint j = 0; j<vec.size(); ++j)
        {
            auto temp = to4D(vec[j]);
            //temp = turn(temp,angx,angy,angz);
            temp = camera(temp);
            temp = project(temp);
            vec[j] = temp.toVector3D();
        }
        drawAxis(vec[0],vec[1],scene,Qt::black);
        drawAxis(vec[1],vec[2],scene,Qt::black);
        drawAxis(vec[2],vec[3],scene,Qt::black);
        drawAxis(vec[3],vec[0],scene,Qt::black);
    }
}

void Control::drawSpheres()
{
    for(auto i = spheres.begin(); i != spheres.end(); ++i)
    {
        Sphere *s = *i;
        auto vec = mkSphere(s);
        for(uint j = 0; j<vec.size(); ++j)
        {
            auto temp = vec[j];
            //temp = turn(temp,angx,angy,angz);
            temp = camera(temp);
            temp = project(temp);
            vec[j] = temp;
        }
        for(uint i = 0; i < vec.size()-1; ++i)
        {
            if ((i+1)%(int)(s->getR()*16) == 0)
                continue;
            drawAxis(vec[i],vec[i+1],scene,Qt::black);
        }
    }
}

std::vector<QVector4D> Control::mkSphere(Sphere *s)
{
    std::vector<QVector4D> vec;
    double r = s->getR();
    double ystep = r/16;
    for(int i = 0; i < r*16; ++i)
    {
        double y = r - i*ystep;
        double x = sqrt(r*r - y*y);
        vec.push_back(QVector4D(x,y,0,1));
    }
    for(int i = vec.size()-1; i >= 0; i--)
    {
        auto temp = vec[i];
        temp[1]*=-1;
        vec.push_back(temp);
    }
    std::vector<QVector4D> points;
    int angles = 16*r + 8;
    double anglestep = 2*M_PI/angles;
    for(int i = 0; i < angles; ++i)
    {
        double angle = anglestep*i;
        QMatrix4x4 turn(   cos(angle),0, sin(angle),   0,
                                 0,1,       0,   0,
                          -sin(angle),0, cos(angle),   0,
                                 0,0,       0,   1);
        for(auto it = vec.begin(); it != vec.end(); ++it)
        {
            auto cur = *it;
            QVector4D res = cur*turn;
            points.push_back(res);
        }
    }
    for(uint i = 0; i < points.size(); i++)
    {
        auto temp = points[i];
        auto mid = s->getMid();
        temp = shift(temp,mid[0],mid[1],mid[2]);
        points[i] = temp;
    }
    return points;
}

void Control::runPthreads()
{
    proc = 0.0;
    int width = scene.width();
    int height = scene.height();
    int size = (width * height * 3)/amount;
    double *data = new double[width * height * 3];
    double **returnValues = (double**)malloc(sizeof(double*)*amount);
    pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t)*amount);
    for (int i = 0; i < amount; ++i)
    {
        std::pair<Control*, int> *arg = new std::pair<Control*, int>(this, i);
        pthread_create(threads+i,NULL,abrakadabra,(void*)arg);
    }
    for (int i = 0; i < amount; ++i)
    {
        pthread_join(threads[i],(void**)&returnValues[i]);
    }
    for (int i = 0; i < amount; ++i)
    {
        for(int j = 0; j < size; j++)
        {
            data[i*size + j] = returnValues[i][j];
        }
    }
    double max = 0;
    auto *it = data;
    for (int i = 0; i < width * height * 3; ++i, ++it)
        max = std::max(max, *it);
    if (std::abs(max) < 10e-9)
        max = 1;
    it = data;
    for (int i = 0; i < width * height * 3; ++i, ++it)
    {
        if(gamma == 1)
            gamma+=0.001;
        it[0] = std::pow(it[0] / max, gamma);
    }
    it = data;
    auto *it2 = scene.bits();
    for (int i = 0; i < width * height; ++i, it += 3, it2 += 4)
    {
        it2[0] = std::min(255,int(it[0] * 255 + 0.5));
        it2[1] = std::min(255,int(it[1] * 255 + 0.5));
        it2[2] = std::min(255,int(it[2] * 255 + 0.5));
        it2[3] = 255;
    }
    emit done();
    frame->setImage(scene);
    for (int i = 0; i < amount; ++i)
    {
        delete[] returnValues[i];
    }
    delete[] data;
    free(threads);
    free(returnValues);
}

void* Control::beginPtracing(void *arg)
{
    int Id = *(int*)arg;
    int width = scene.width();
    int height = scene.height()/amount;
    double *data = new double[width * height * 3];
    switch (quality) {
    case Rough:
        {
            double prog = 0, prog_step = 400.0 / (width * height);
            int iterStep = width*3;
            auto *it = data, *it2 = data + iterStep;
            for (int y = Id*height; y < height*(Id+1); y += 2, it += iterStep, it2 += iterStep)
                for (int x = 0; x < width; x += 2, it += 6, it2 += 6)
                {
                    auto rgb = rayFromScreen(x + 1, y + 1, prog, prog_step);
                    it[0] = rgb[2];
                    it[1] = rgb[1];
                    it[2] = rgb[0];
                    it[3] = rgb[2];
                    it[4] = rgb[1];
                    it[5] = rgb[0];
                    it2[0] = rgb[2];
                    it2[1] = rgb[1];
                    it2[2] = rgb[0];
                    it2[3] = rgb[2];
                    it2[4] = rgb[1];
                    it2[5] = rgb[0];
                }
        }
        break;

    case Normal:
        {
            double prog = 0, prog_step = 100.0 / (width * height);
            auto *it = data;
            for (int y = Id*height; y < height*(Id+1); ++y)
                for (int x = 0; x < width; ++x, it += 3)
                {
                    auto rgb = rayFromScreen(x + 0.5f, y + 0.5f, prog, prog_step);
                    it[0] = rgb[2];
                    it[1] = rgb[1];
                    it[2] = rgb[0];
                }
        }
        break;

    case Fine:
        {
            double prog = 0, prog_step = 25.0 / (width * height);
            auto *it = data;
            for (int y = Id*height; y < height*(Id+1); ++y)
                for (int x = 0; x < width; ++x, it += 3)
                {
                    auto rgb1 = rayFromScreen(x + 0.25, y + 0.25, prog, prog_step);
                    auto rgb2 = rayFromScreen(x + 0.75, y + 0.25, prog, prog_step);
                    auto rgb3 = rayFromScreen(x + 0.25, y + 0.75, prog, prog_step);
                    auto rgb4 = rayFromScreen(x + 0.75, y + 0.75, prog, prog_step);
                    it[0] = (rgb1[2] + rgb2[2] + rgb3[2] + rgb4[2]) / 4;
                    it[1] = (rgb1[1] + rgb2[1] + rgb3[1] + rgb4[1]) / 4;
                    it[2] = (rgb1[0] + rgb2[0] + rgb3[0] + rgb4[0]) / 4;
                }
        }
        break;
    }
    return (void*)data;
}

void Control::beginTracing()
{
    int width = scene.width();
    int height = scene.height();
    double *data = new double[width * height * 3];

    switch (quality) {
    case Rough:
        {
            double prog = 0, prog_step = 400.0 / (width * height);
            int iterStep = width*3;
            auto *it = data, *it2 = data + iterStep;
            for (int y = 0; y < height; y += 2, it += iterStep, it2 += iterStep)
                for (int x = 0; x < width; x += 2, it += 6, it2 += 6)
                {
                    auto rgb = rayFromScreen(x + 1.0, y + 1.0, prog, prog_step);
                    it[0] = rgb[2];
                    it[1] = rgb[1];
                    it[2] = rgb[0];
                    it[3] = rgb[2];
                    it[4] = rgb[1];
                    it[5] = rgb[0];
                    it2[0] = rgb[2];
                    it2[1] = rgb[1];
                    it2[2] = rgb[0];
                    it2[3] = rgb[2];
                    it2[4] = rgb[1];
                    it2[5] = rgb[0];
                }
        }
        break;

    case Normal:
        {
            double prog = 0, prog_step = 100.0 / (width * height);
            auto *it = data;
            for (int y = 0; y < height; ++y)
                for (int x = 0; x < width; ++x, it += 3)
                {
                    auto rgb = rayFromScreen(x + 0.5f, y + 0.5f, prog, prog_step);
                    it[0] = rgb[2];
                    it[1] = rgb[1];
                    it[2] = rgb[0];
                }
        }
        break;

    case Fine:
        {
            double prog = 0, prog_step = 25.0 / (width * height);
            auto *it = data;
            for (int y = 0; y < height; ++y)
                for (int x = 0; x < width; ++x, it += 3)
                {
                    auto rgb1 = rayFromScreen(x + 0.25, y + 0.25, prog, prog_step);
                    auto rgb2 = rayFromScreen(x + 0.75, y + 0.25, prog, prog_step);
                    auto rgb3 = rayFromScreen(x + 0.25, y + 0.75, prog, prog_step);
                    auto rgb4 = rayFromScreen(x + 0.75, y + 0.75, prog, prog_step);
                    it[0] = (rgb1[2] + rgb2[2] + rgb3[2] + rgb4[2]) / 4;
                    it[1] = (rgb1[1] + rgb2[1] + rgb3[1] + rgb4[1]) / 4;
                    it[2] = (rgb1[0] + rgb2[0] + rgb3[0] + rgb4[0]) / 4;
                }
        }
        break;
    }


    double max = 0;
    auto *it = data;
    for (int i = 0; i < width * height * 3; ++i, ++it)
        max = std::max(max, *it);

    if (std::abs(max) < 10e-9)
        max = 1;

    it = data;
    for (int i = 0; i < width * height * 3; ++i, ++it)
        it[0] = std::pow(it[0] / max, gamma);

    it = data;
    auto *it2 = scene.bits();
    for (int i = 0; i < width * height; ++i, it += 3, it2 += 4)
    {
        it2[0] = std::min(255,int(it[0] * 255 + 0.5));
        it2[1] = std::min(255,int(it[1] * 255 + 0.5));
        it2[2] = std::min(255,int(it[2] * 255 + 0.5));
        it2[3] = 255;
    }


    frame->setImage(scene);
    emit done();
    delete[] data;
}

QVector3D Control::rayFromScreen(double x, double y, double &progress, double progress_step)
{
    QVector3D k = (pView - pRef).normalized();
    QVector3D i = QVector3D::crossProduct(k,vUp).normalized();
    QVector3D j = QVector3D::crossProduct(i, k);
    QVector3D screen = Zf * k;
    double r = 0, g = 0, b = 0;
    x = xtoReal(x);
    y = ytoReal(y);
    QVector3D point = (screen + x * i + y * j).normalized();
    traceRay(tracingDepth,pRef, point, r, g, b);
    proc += progress_step;
    progress += progress_step;
    emit rendering((int) progress);
    return QVector3D(r,g,b);
}

void Control::traceRay(int depth,const QVector3D &begin,const QVector3D &way, double &r, double &g, double &b)
{
    if (depth <= 0)
    {
            r = 0;
            g = 0;
            b = 0;
            return;
        }

        double x, y, z;
        Primitive *primit;

        if (!checkPrimitives(begin, way, x, y, z, &primit))
        {
            r = (double)background.red()/255;
            g = (double)background.green()/255;
            b = (double)background.blue()/255;
            return;
        }

        auto &primitive = *primit;
        QVector3D start(x, y, z);


        double Rr = 0, Gr = 0, Br = 0;
        traceRay(depth - 1, start, primitive.reflect(way, start), Rr, Gr, Br);

        auto norm = primitive.getNorm(start);

        for (auto l : light)
        {
            QVector3D l_way = start - l.first;
            float fatt = 1 / (1 + l_way.length());
            l_way.normalize();

            double x, y, z;
            Primitive *crossLight;
            checkPrimitives(l.first, l_way, x, y, z, &crossLight);

            if (crossLight != primit)
                continue;


            double k1 = QVector3D::dotProduct(norm, -l_way);
            QVector3D H = l_way + way;
            H.normalize();
            double k2 = std::pow(QVector3D::dotProduct(norm, -H), primitive.getPower());


            r += fatt * l.second.red() * (primitive.getKDr() * k1 + primitive.getKSr() * k2);
            g += fatt * l.second.green() * (primitive.getKDg() * k1 + primitive.getKSg() * k2);
            b += fatt * l.second.blue() * (primitive.getKDb() * k1 + primitive.getKSb() * k2);
        }


        r += primitive.getKDr() * argblight.red() + primitive.getKSr() * Rr;
        g += primitive.getKDg() * argblight.green() + primitive.getKSg() * Gr;
        b += primitive.getKDb() * argblight.blue() + primitive.getKSb() * Br;
}

bool Control::checkPrimitives(const QVector3D &begin,const QVector3D &way, double &x, double &y, double &z, Primitive **primitive)
{
    bool crossed = false;
    double min_len;
    std::vector<Primitive*> primitives;
//    for(auto i : boxes)
//        primitives.push_back(i);
    for(auto i : triangles)
        primitives.push_back(i);
    for(auto i : quads)
        primitives.push_back(i);
    for(auto i : spheres)
        primitives.push_back(i);

    for (auto &p : primitives)
    {
        double xF, yF, zF;
        if (p->checkCrossing(begin, way, xF, yF, zF))
        {
            if (crossed)
            {
                float len = (QVector3D(xF, yF, zF) - begin).length();

                if (len < min_len)
                {
                    min_len = len;
                    x = xF;
                    y = yF;
                    z = zF;
                    (*primitive) = p;
                }
            }
            else
            {
                crossed = true;
                min_len = (QVector3D(xF, yF, zF) - begin).length();
                x = xF;
                y = yF;
                z = zF;
                (*primitive) = p;
            }
        }
    }

    return crossed;
}

