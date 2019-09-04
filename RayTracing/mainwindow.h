#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent>
#include <QShowEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPointer>
#include "control.h"
#include "openaction.h"
#include <QFile>
#include <settings.h>
#include <saveaction.h>
#include <QApplication>
#include <QVector3D>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QPointer<Control> controller;
    int xstart = -1;
    int ystart = -1;
    bool rendering = false;
    bool selecting = false;
    int thread_num = 16;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void wheelEvent(QWheelEvent *event);
    void pressed(QMouseEvent *event);
    void released(QMouseEvent *event);
    void moved(QMouseEvent *event);
    void on_actionOpen_triggered();
    void progress(int prog);
    void rdone();
    void on_actionRender_triggered();

    void on_actionLoad_Render_Settings_triggered();

    void on_actionInit_triggered();

    void on_actionSettings_triggered();

    void on_actionSelect_View_triggered();

    void on_actionSaveImage_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
