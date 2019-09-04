#ifndef LINEEDITOR_H
#define LINEEDITOR_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QShowEvent>
#include <QImage>
#include <math.h>
#include <QMatrix4x4>
#include <QVector4D>
#include <figure.h>

namespace Ui {
class LineEditor;
}

class LineEditor : public QMainWindow
{
    Q_OBJECT
    QImage frame;
    double a = -3.14;
    double b = 3.14;
    double c = -3.14;
    double d = 3.14;
    uint rc = 0;
    uint gc = 0;
    uint bc = 0;
    int pointFlag = -1;
    int addflag = false;
    int delflag = false;
    Figure *editing = NULL;

    std::vector<std::pair<int,int>> pixelBPoints;

    std::vector<std::pair<double,double>> controllerLine;
public:
    std::vector<std::pair<double,double>> bSplinePoints;
    std::vector<std::pair<double,double>> linePoints;
    explicit LineEditor(QWidget *parent = 0);
    void fillSin(int points);
    void load(Figure *f);
    void edit();
    void commit();
    void setContrLine(std::vector<std::pair<double, double> > &line);
    ~LineEditor();
    Figure *getEditing() const;
    QColor getColor();
    void setEditing(Figure *value);

signals:
    void accepted();
    void edited();

private slots:
    void mouseMoved(QMouseEvent *event);
    void mouseReleased(QMouseEvent *event);
    void mousePressed(QMouseEvent *event);
    void on_actionTestSIN_triggered();

    void on_addPoint_clicked();
    void showEvent(QShowEvent* event);
    void on_deletePoint_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_R_Spin_valueChanged(double arg1);

    void on_G_Spin_valueChanged(double arg1);

    void on_B_Spin_valueChanged(double arg1);

private:
    Ui::LineEditor *ui;
    void makeLine();
    void drawSpline();
    void drawPoints();
    QVector4D countT(float t);
    void setColors(QColor c);
};

#endif // LINEEDITOR_H
