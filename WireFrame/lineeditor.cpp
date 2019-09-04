#include "lineeditor.h"
#include "ui_lineeditor.h"
#include "control.h"



Figure *LineEditor::getEditing() const
{
    return editing;
}

QColor LineEditor::getColor()
{
    return QColor(rc,gc,bc);
}

void LineEditor::setEditing(Figure *value)
{
    editing = value;
}

LineEditor::LineEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LineEditor)
{
    ui->setupUi(this);
    connect(ui->widget, SIGNAL(mouseMoveHappened(QMouseEvent*)), this, SLOT(mouseMoved(QMouseEvent*)));
    connect(ui->widget,SIGNAL(mouseReleased(QMouseEvent*)),this,SLOT(mouseReleased(QMouseEvent*)));
    connect(ui->widget,SIGNAL(mousePressed(QMouseEvent*)),this,SLOT(mousePressed(QMouseEvent*)));
    fillSin(7);
}

void LineEditor::fillSin(int points)
{
    double step = (b-a)/points;
    for(int i = 0; i < points; i++)
    {
        double x = a + step*i;
        bSplinePoints.push_back(std::pair<double,double>(x,2*sin(x)));
    }
}

void LineEditor::load(Figure *f)
{
    editing = f;
    setColors(f->color);
    edit();
}

void LineEditor::edit()
{
    frame = QImage(ui->widget->width(),ui->widget->height(),QImage::Format_ARGB32);
    frame.fill(Qt::white);
    bSplinePoints.clear();
    for (auto i = editing->bSplinePoints.begin(); i!= editing->bSplinePoints.end(); ++i)
    {
        bSplinePoints.push_back(*i);
    }
    drawPoints();
    makeLine();
    drawSpline();
}

void LineEditor::commit()
{
    editing->bSplinePoints.clear();
    editing->linePoints.clear();
    for(auto i = linePoints.begin(); i != linePoints.end(); ++i)
    {
        editing->linePoints.push_back(*i);
    }
    for(auto i = bSplinePoints.begin(); i!= bSplinePoints.end(); ++i)
    {
        editing->bSplinePoints.push_back(*i);
    }
}

void LineEditor::setContrLine(std::vector<std::pair<double, double> > &line)
{
    controllerLine = line;
}


LineEditor::~LineEditor()
{
    delete ui;
}

void LineEditor::mouseMoved(QMouseEvent *event)
{
    int x = std::max(2,std::min(ui->widget->width()-2,event->x()));
    int y = std::max(2,std::min(ui->widget->height()-2,event->y()));
    if(pointFlag != -1 && !addflag && !delflag)
    {
        frame.fill(Qt::white);
        pixelBPoints[pointFlag].first = x;
        pixelBPoints[pointFlag].second = y;
        bSplinePoints[pointFlag].first = Control::xtoReal(x,a,b,frame.width());
        bSplinePoints[pointFlag].second = Control::ytoReal(y,c,d,frame.height());
        drawPoints();
        makeLine();
        drawSpline();
    }
}

void LineEditor::mouseReleased(QMouseEvent *event)
{
    if(delflag && pointFlag!=-1)
    {
        pixelBPoints.erase(pixelBPoints.begin() + pointFlag);
        bSplinePoints.erase(bSplinePoints.begin() + pointFlag);
        frame.fill(Qt::white);
        drawPoints();
        makeLine();
        drawSpline();
        delflag = false;
        ui->deletePoint->setChecked(false);
    }
    if(addflag)
    {
        int x = event->x();
        int y = event->y();
        int px;
        int cx;
        for(uint i = 0; i < pixelBPoints.size(); ++i)
        {
            if(i==0)
            {
                px = 2;
                cx = pixelBPoints[i].first;
            }
            else if(i == pixelBPoints.size()-1)
            {
                px = pixelBPoints[i].first;
                cx = ui->widget->width()-2;
                i++;
            }
            else
            {
                px = pixelBPoints[i-1].first;
                cx = pixelBPoints[i].first;
            }
            if(x>px && x < cx)
            {
                pixelBPoints.insert(pixelBPoints.begin() + i,std::pair<int,int>(x,y));
                double rx = Control::xtoReal(x,a,b,ui->widget->width());
                double ry = Control::ytoReal(y,c,d,ui->widget->height());
                bSplinePoints.insert(bSplinePoints.begin() + i,std::pair<double,double>(rx,ry));
                frame.fill(Qt::white);
                drawPoints();
                makeLine();
                drawSpline();
                addflag = false;
                ui->addPoint->setChecked(false);
                break;
            }
        }
    }
    pointFlag = -1;
}

void LineEditor::mousePressed(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();
    for(uint i = 0; i < pixelBPoints.size(); ++i)
    {
        auto current = pixelBPoints[i];
        if(x > current.first-4 && x < current.first+4
            && y > current.second-4 && y < current.second+4)
        {
            pointFlag = i;
            break;
        }
    }
}

void LineEditor::on_actionTestSIN_triggered()
{
    frame = QImage(ui->widget->width(),ui->widget->height(),QImage::Format_ARGB32);
    frame.fill(Qt::white);
    fillSin(7);
    drawPoints();
    makeLine();
    drawSpline();
}

void LineEditor::makeLine()
{
    QMatrix4x4 m(-1.0,  3.0, -3.0, 1.0,
                  3.0, -6.0,  3.0, 0.0,
                 -3.0,  0.0,  3.0, 0.0,
                  1.0,  4.0,  1.0, 0.0);
    m = m*(1.0/6);
    linePoints.clear();
    for(uint i = 1; i < bSplinePoints.size()-2; i++)
    {
        for(uint j = 0; j <= 10; j++)
        {
            QVector4D t = countT(j*0.1);
            QVector4D tXm = t*m;
            QVector4D gx(bSplinePoints[i-1].first,
                         bSplinePoints[i].first,
                         bSplinePoints[i+1].first,
                         bSplinePoints[i+2].first);
            QVector4D gy(bSplinePoints[i-1].second,
                         bSplinePoints[i].second,
                         bSplinePoints[i+1].second,
                         bSplinePoints[i+2].second);
            double x = tXm[0]*gx[0] +
                       tXm[1]*gx[1] +
                       tXm[2]*gx[2] +
                       tXm[3]*gx[3];
            double y = tXm[0]*gy[0] +
                       tXm[1]*gy[1] +
                       tXm[2]*gy[2] +
                       tXm[3]*gy[3];
            linePoints.push_back(std::pair<double,double>(x,y));
        }
    }
}

void LineEditor::drawSpline()
{
    for( uint i = 0; i < linePoints.size()-1; ++i)
    {
        auto cur = linePoints[i];
        auto next = linePoints[i+1];
        int x0 = Control::xtoPixel(cur.first,a,b,ui->widget->width());
        int y0 = Control::ytoPixel(cur.second,c,d,ui->widget->height());
        int x1 = Control::xtoPixel(next.first,a,b,ui->widget->width());
        int y1 = Control::ytoPixel(next.second,c,d,ui->widget->height());
        Control::drawLine(x0,y0,x1,y1,frame,QColor(rc,gc,bc));
    }
    Control::drawLine(ui->widget->width()/2, 0,
                      ui->widget->width()/2, ui->widget->height()-1,
                      frame, Qt::red);
    Control::drawLine(0, ui->widget->height()/2,
                      ui->widget->width()-1, ui->widget->height()/2,
                      frame, Qt::red);
    ui->widget->setImage(frame);
}

void LineEditor::drawPoints()
{
    QRgb* bits = (QRgb*)frame.bits();
    pixelBPoints.clear();
    for(auto i = bSplinePoints.begin(); i != bSplinePoints.end(); ++i)
    {
        auto cur = *i;
        int x0 = Control::xtoPixel(cur.first,a,b,ui->widget->width());
        int y0 = Control::ytoPixel(cur.second,c,d,ui->widget->height());
        uint offset = y0*frame.width() + x0;
        pixelBPoints.push_back(std::pair<int,int>(x0,y0));
        auto c = QColor(0,0,255).rgb();
        bits[offset-2*frame.width()] = c;
        bits[offset-frame.width()] = c;
        bits[offset-2] = c;
        bits[offset-1] = c;
        bits[offset] = c;
        bits[offset+1] = c;
        bits[offset+2] = c;
        bits[offset+frame.width()] = c;
        bits[offset+2*frame.width()] = c;
    }
}

QVector4D LineEditor::countT(float t)
{
    return QVector4D(t*t*t,t*t,t,1);
}

void LineEditor::setColors(QColor c)
{
    rc = c.red();
    gc = c.green();
    bc = c.blue();
}

void LineEditor::on_addPoint_clicked()
{
    if(!ui->addPoint->isChecked())
    {
        ui->addPoint->setChecked(false);
        ui->deletePoint->setChecked(false);
        addflag = false;
        delflag = false;
    }
    else
    {
        ui->addPoint->setChecked(true);
        ui->deletePoint->setChecked(false);
        addflag = true;
        delflag = false;
    }
}

void LineEditor::showEvent(QShowEvent* event)
{
    QMainWindow::showEvent(event);
    frame = QImage(ui->widget->width(),ui->widget->height(),QImage::Format_ARGB32);
    frame.fill(Qt::white);
    drawPoints();
    makeLine();
    drawSpline();
}

void LineEditor::on_deletePoint_clicked()
{
    if(!ui->deletePoint->isChecked())
    {
        ui->addPoint->setChecked(false);
        ui->deletePoint->setChecked(false);
        addflag = false;
        delflag = false;
    }
    else
    {
        ui->addPoint->setChecked(false);
        ui->deletePoint->setChecked(true);
        addflag = false;
        delflag = true;
    }
}

void LineEditor::on_buttonBox_accepted()
{
    if(editing == NULL)
        emit accepted();
    else
    {
        emit edited();
    }
}

void LineEditor::on_buttonBox_rejected()
{
    editing = NULL;
    this->hide();
}

void LineEditor::on_R_Spin_valueChanged(double arg1)
{
    rc =arg1;
    drawSpline();
}

void LineEditor::on_G_Spin_valueChanged(double arg1)
{
    gc = arg1;
    drawSpline();
}

void LineEditor::on_B_Spin_valueChanged(double arg1)
{
    bc = arg1;
    drawSpline();
}

