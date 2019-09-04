#include "mainwindow.h"
#include "ui_mainwindow.h"

double ZOOM_STEP = 1.2;
double SHIFT_STEP = 0.5;
double ANGLE_STEP = M_PI/32;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    controller = new Control(ui->mainField);
    editor = new LineEditor(this);
    connect(editor, SIGNAL(accepted()), this, SLOT(line_accepted()));
    connect(editor, SIGNAL(edited()), this, SLOT(line_edited()));
    connect(ui->mainField,SIGNAL(mouseMoveHappened(QMouseEvent*)),this,SLOT(moved(QMouseEvent*)));
    connect(ui->mainField,SIGNAL(mousePressed(QMouseEvent*)),this,SLOT(pressed(QMouseEvent*)));
    connect(ui->mainField,SIGNAL(mouseReleased(QMouseEvent*)),this,SLOT(released(QMouseEvent*)));
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAdd_Figure_triggered()
{
    editor->show();
}

void MainWindow::line_accepted()
{
    editor->hide();
    std::vector<std::pair<double,double>> line;
    std::vector<std::pair<double,double>> bSpline;
    for(auto i = editor->linePoints.begin(); i != editor->linePoints.end(); ++i)
    {
        line.push_back(*i);
    }
    for(auto i = editor->bSplinePoints.begin(); i!= editor->bSplinePoints.end(); ++i)
    {
        bSpline.push_back(*i);
    }
    controller->addF(new Figure(line,bSpline,editor->getColor()));
    controller->mkscene();
}

void MainWindow::line_edited()
{
    editor->commit();
    editor->hide();
    controller->rotateLine(editor->getEditing());
    controller->mkscene();
    editor->setEditing(NULL);
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    editor->show();
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    int ang = event->angleDelta().y();
    double zf = controller->getZf();
    if (ang < 0) {
        zf/=ZOOM_STEP;
    } else {
        zf*=ZOOM_STEP;
    }
    if(zf>=controller->getZb()-3)
        zf = controller->getZf();
    controller->setZf(zf);
    ui->zfSpinBox->setValue(zf);
    controller->mkscene();
}

void MainWindow::pressed(QMouseEvent *event)
{
    xstart = event->x();
    ystart = event->y();
}

void MainWindow::released(QMouseEvent *event)
{
    xstart = -1;
    ystart = -1;
}

void MainWindow::moved(QMouseEvent *event)
{
    if(xstart>=0)
    {
        double dx = event->x()-xstart;
        double dy = (event->y()-ystart);
        xstart = event->x();
        ystart = event->y();
        dx/=314;
        dy/=160;
        if (selected == -1)
        {
            controller->setAngz(controller->getAngz() - dy);
            controller->setAngy(controller->getAngy() + dx);
            controller->mkscene();
        }
        else
        {
            controller->changeFigureAngleZ(selected, controller->getFigureAngleZ(selected)-dy);
            controller->changeFigureAngleY(selected, controller->getFigureAngleY(selected)+dx);
            controller->mkscene();
        }
    }
}


void MainWindow::on_actionEdit_Mode_triggered()
{
    if(controller->figures.size() == 0)
        return;
    if(ui->actionEdit_Mode->isChecked())
    {
        selected = 0;
        controller->setSelected(selected);
        controller->mkscene();
        ui->actionEdit_Mode->setChecked(true);
        ui->next->setEnabled(true);
        ui->previous->setEnabled(true);
        ui->DeleteFigure->setEnabled(true);
        ui->EditSpline->setEnabled(true);
        ui->shiftxdown->setEnabled(true);
        ui->shiftxup->setEnabled(true);
        ui->shiftYDown->setEnabled(true);
        ui->shiftYUp->setEnabled(true);
        ui->shiftZDown->setEnabled(true);
        ui->shiftZUp->setEnabled(true);
        ui->turnXDown->setEnabled(true);
        ui->turnXUp->setEnabled(true);
        ui->turnYDown->setEnabled(true);
        ui->turnYUp->setEnabled(true);
        ui->turnZDown->setEnabled(true);
        ui->turnZUp->setEnabled(true);
    }
    else
    {
        selected = -1;
        controller->setSelected(selected);
        controller->mkscene();
        ui->actionEdit_Mode->setChecked(false);
        ui->next->setEnabled(false);
        ui->previous->setEnabled(false);
        ui->DeleteFigure->setEnabled(false);
        ui->EditSpline->setEnabled(false);
        ui->shiftxdown->setEnabled(false);
        ui->shiftxup->setEnabled(false);
        ui->shiftYDown->setEnabled(false);
        ui->shiftYUp->setEnabled(false);
        ui->shiftZDown->setEnabled(false);
        ui->shiftZUp->setEnabled(false);
        ui->turnXDown->setEnabled(false);
        ui->turnXUp->setEnabled(false);
        ui->turnYDown->setEnabled(false);
        ui->turnYUp->setEnabled(false);
        ui->turnZDown->setEnabled(false);
        ui->turnZUp->setEnabled(false);
    }
}

void MainWindow::on_shiftxup_clicked()
{
    controller->changeFigureShiftX(selected,(controller->getFigureShiftX(selected)+SHIFT_STEP));
    controller->mkscene();
}

void MainWindow::on_shiftxdown_clicked()
{
    controller->changeFigureShiftX(selected,(controller->getFigureShiftX(selected)-SHIFT_STEP));
    controller->mkscene();
}

void MainWindow::on_shiftYUp_clicked()
{
    controller->changeFigureShiftY(selected,(controller->getFigureShiftY(selected)+SHIFT_STEP));
    controller->mkscene();
}

void MainWindow::on_shiftYDown_clicked()
{
    controller->changeFigureShiftY(selected,(controller->getFigureShiftY(selected)-SHIFT_STEP));
    controller->mkscene();
}

void MainWindow::on_shiftZUp_clicked()
{
    controller->changeFigureShiftZ(selected,(controller->getFigureShiftZ(selected)+SHIFT_STEP));
    controller->mkscene();
}

void MainWindow::on_shiftZDown_clicked()
{
    controller->changeFigureShiftZ(selected,(controller->getFigureShiftZ(selected)-SHIFT_STEP));
    controller->mkscene();
}

void MainWindow::on_turnXUp_clicked()
{
    controller->changeFigureAngleX(selected,controller->getFigureAngleX(selected)+ANGLE_STEP);
    controller->mkscene();
}

void MainWindow::on_turnXDown_clicked()
{
    controller->changeFigureAngleX(selected,controller->getFigureAngleX(selected)-ANGLE_STEP);
    controller->mkscene();
}

void MainWindow::on_turnYUp_clicked()
{
    controller->changeFigureAngleY(selected,controller->getFigureAngleY(selected)+ANGLE_STEP);
    controller->mkscene();
}

void MainWindow::on_turnYDown_clicked()
{
    controller->changeFigureAngleY(selected,controller->getFigureAngleY(selected)-ANGLE_STEP);
    controller->mkscene();
}

void MainWindow::on_turnZUp_clicked()
{
    controller->changeFigureAngleZ(selected,controller->getFigureAngleZ(selected)+ANGLE_STEP);
    controller->mkscene();
}

void MainWindow::on_turnZDown_clicked()
{
    controller->changeFigureAngleZ(selected,controller->getFigureAngleZ(selected)-ANGLE_STEP);
    controller->mkscene();
}

void MainWindow::on_next_clicked()
{
    if(selected == controller->figures.size()-1)
        selected = 0;
    else
        selected++;
    controller->setSelected(selected);
    controller->mkscene();
}

void MainWindow::on_previous_clicked()
{
    if(selected == 0)
        selected = controller->figures.size()-1;
    else
        selected--;
    controller->setSelected(selected);
    controller->mkscene();
}

void MainWindow::on_swSpinBox_valueChanged(double arg1)
{
    double coef = arg1/controller->getSh();
    int w =ui->mainField->width();
    int h = w/coef;
    ui->mainField->resize(QSize(w,h));
    controller->setSw(arg1);
    controller->mkscene();
}

void MainWindow::on_shSpinBox_valueChanged(double arg1)
{
    double coef = controller->getSw()/arg1;
    int w =ui->mainField->width();
    int h = w/coef;
    ui->mainField->resize(QSize(w,h));

    controller->setSh(arg1);
    controller->mkscene();
}

void MainWindow::on_zfSpinBox_valueChanged(double arg1)
{
    if(arg1>=controller->getZb()-3)
        ui->zfSpinBox->setValue(controller->getZf());
    else
    {
        controller->setZf(arg1);
        controller->mkscene();
    }

}

void MainWindow::on_zbSpinBox_valueChanged(double arg1)
{
    controller->setZb(arg1);
    controller->mkscene();
}

void MainWindow::on_DeleteFigure_clicked()
{
    controller->deleteFigure(selected);
    if(controller->figures.size() == 0)
    {
        selected = -1;
        ui->actionEdit_Mode->setChecked(false);
        ui->next->setEnabled(false);
        ui->previous->setEnabled(false);
        ui->DeleteFigure->setEnabled(false);
        ui->shiftxdown->setEnabled(false);
        ui->shiftxup->setEnabled(false);
        ui->shiftYDown->setEnabled(false);
        ui->shiftYUp->setEnabled(false);
        ui->shiftZDown->setEnabled(false);
        ui->shiftZUp->setEnabled(false);
        ui->turnXDown->setEnabled(false);
        ui->turnXUp->setEnabled(false);
        ui->turnYDown->setEnabled(false);
        ui->turnYUp->setEnabled(false);
        ui->turnZDown->setEnabled(false);
        ui->turnZUp->setEnabled(false);
    }
    else
        selected = 0;
    controller->setSelected(selected);
    controller->mkscene();
}

void MainWindow::on_EditSpline_clicked()
{
    editor->load(controller->figures[selected]);
    editor->show();
}
