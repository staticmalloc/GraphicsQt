#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    int width = 800;
    int height = 800;
    int depth = 3;
    int threads = 16;
    int quality = 1;
    QColor background = QColor(25,25,25);
    double gamma = 1.0;
    bool acceptflag = false;
    ~Settings();

private slots:
    void on_backR_valueChanged(int arg1);

    void on_backG_valueChanged(int arg1);

    void on_backB_valueChanged(int arg1);

    void on_gamma_valueChanged(double arg1);

    void on_quality_valueChanged(int arg1);

    void on_widthspin_valueChanged(int arg1);

    void on_heightspin_valueChanged(int arg1);


    void on_depth_valueChanged(int arg1);

    void on_threads_valueChanged(int arg1);

    void on_buttonBox_accepted();

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
