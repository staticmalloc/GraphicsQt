#include "mainwindow.h"
#include "ui_mainwindow.h"

const double ZOOM_STEP = 1.1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    controller = new Control(ui->MainFrame,nullptr);
    connect(controller, SIGNAL(rendering(int)),this,SLOT(progress(int)));
    connect(controller, SIGNAL(done()),this,SLOT(rdone()));
    connect(ui->MainFrame,SIGNAL(mouseMoveHappened(QMouseEvent*)),this,SLOT(moved(QMouseEvent*)));
    connect(ui->MainFrame,SIGNAL(mousePressed(QMouseEvent*)),this,SLOT(pressed(QMouseEvent*)));
    connect(ui->MainFrame,SIGNAL(mouseReleased(QMouseEvent*)),this,SLOT(released(QMouseEvent*)));
}
void MainWindow::wheelEvent(QWheelEvent *event)
{
    if(rendering)return;
    if(!selecting)return;
    int ang = event->angleDelta().y();
    if (QApplication::keyboardModifiers() & Qt::SHIFT)
    {
        const static float step = 0.001f;
        auto v = controller->getPView();
        auto r = controller->getPRef();
        QVector3D k = (event->delta() * step) * (v - r).normalized();
        controller->setPRef(r+k);
        controller->setPView(v+k);
    }
    else
    {
        double zf = controller->getZf();
        if (ang < 0) {
            zf/=ZOOM_STEP;
        } else {
            zf*=ZOOM_STEP;
        }
        controller->setZf(zf);
    }
    controller->makeScene();
}

void MainWindow::pressed(QMouseEvent *event)
{
    if(rendering)return;
    if(!selecting)return;
    xstart = event->x();
    ystart = event->y();
}

void MainWindow::released(QMouseEvent *event)
{
    if(rendering)return;
    if(!selecting)return;
    xstart = -1;
    ystart = -1;
}

void MainWindow::moved(QMouseEvent *event)
{
    if(rendering)return;
    if(!selecting)return;
    if(xstart>=0)
    {
        double dx = event->x()-xstart;
        double dy = (event->y()-ystart);
        xstart = event->x();
        ystart = event->y();
        if(QApplication::keyboardModifiers() & Qt::SHIFT)
            controller->turnScene(dx/3000,dy/3000);
        else
        {
            dx/=200;
            dy/=200;
            auto view = controller->getPView();
            auto ref = controller->getPRef();
            auto vUp = controller->getVUp();
            QVector3D w = (view-ref).normalized();
            QVector3D u = QVector3D::crossProduct(w,vUp).normalized();
            QVector3D v = QVector3D::crossProduct(w,u);
            controller->setPRef(ref  -dx*u  -dy*v);
            controller->setPView(view  -dx*u  -dy*v);
        }
        controller->makeScene();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_triggered()
{
    if(rendering)return;
    openaction open;
    open.exec();
    if(!open.getAccept())
        return;
    else
    {
        std::string name = open.getFilename().toStdString();
        QString _name = QString::fromStdString(name);
        controller->openState(name);
        _name.chop(5);
        _name.append("render");
        QFile r(_name);
        if(r.exists())
            controller->loadRSettings(_name.toStdString());
        ui->actionSelect_View->setEnabled(true);
        ui->actionSettings->setEnabled(true);
        ui->actionRender->setEnabled(true);
        ui->actionInit->setEnabled(true);
    }
}

void MainWindow::progress(int prog)
{
    ui->status->setText("Rendering: " + QString::number((int)controller->getProc()/thread_num) + "%");
}

void MainWindow::rdone()
{
    rendering = false;
    ui->actionSelect_View->setEnabled(true);
    ui->actionSaveImage->setEnabled(true);
    ui->actionRender->setChecked(false);
    ui->status->setText("Done");
}

void MainWindow::on_actionRender_triggered()
{
    rendering = true;
    selecting = false;
    ui->actionSelect_View->setChecked(false);
    ui->actionSelect_View->setEnabled(false);
    ui->actionRender->setChecked(true);
    controller->render();
}

void MainWindow::on_actionLoad_Render_Settings_triggered()
{
    if(rendering)return;
    openaction open;
    open.exec();
    if(!open.getAccept())
        return;
    else
    {
        std::string name = open.getFilename().toStdString();
        controller->openState(name);
    }
}

void MainWindow::on_actionInit_triggered()
{
    controller->toInit();
}

void MainWindow::on_actionSettings_triggered()
{
    Settings s;
    s.exec();
    if(s.acceptflag)
    {
        controller->changeSettings(s.width,s.height,s.depth,s.quality,s.background,s.gamma,s.threads);
        thread_num = s.threads;
    }
    return;
}

void MainWindow::on_actionSelect_View_triggered()
{
    selecting = true;
    ui->actionSelect_View->setChecked(true);
}

void MainWindow::on_actionSaveImage_triggered()
{
    QImage saving = ui->MainFrame->getImage();
    SaveAction save;
    save.exec();
    if(!save.getAccept())
        return;
    else
        saving.save(save.getName());
}
