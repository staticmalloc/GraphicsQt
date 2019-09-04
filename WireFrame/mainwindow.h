#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent>
#include <QShowEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPointer>
#include "control.h"
#include "lineeditor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QPointer<Control> controller;
    QPointer<LineEditor> editor;
    int xstart = -1;
    int ystart = -1;
    int selected = -1;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_actionAdd_Figure_triggered();
    void line_accepted();
    void line_edited();
    void showEvent(QShowEvent *event);
    void wheelEvent(QWheelEvent *event);
    void pressed(QMouseEvent *event);
    void released(QMouseEvent *event);
    void moved(QMouseEvent *event);
    void on_actionEdit_Mode_triggered();

    void on_shiftxup_clicked();

    void on_shiftxdown_clicked();

    void on_shiftYUp_clicked();

    void on_shiftYDown_clicked();

    void on_shiftZUp_clicked();

    void on_shiftZDown_clicked();

    void on_turnXUp_clicked();

    void on_turnXDown_clicked();

    void on_turnYUp_clicked();

    void on_turnYDown_clicked();

    void on_turnZUp_clicked();

    void on_turnZDown_clicked();

    void on_next_clicked();

    void on_previous_clicked();

    void on_swSpinBox_valueChanged(double arg1);

    void on_shSpinBox_valueChanged(double arg1);

    void on_zfSpinBox_valueChanged(double arg1);

    void on_zbSpinBox_valueChanged(double arg1);

    void on_DeleteFigure_clicked();

    void on_EditSpline_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
