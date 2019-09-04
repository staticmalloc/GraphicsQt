#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_backR_valueChanged(int arg1)
{
    background.setRed(arg1);
}

void Settings::on_backG_valueChanged(int arg1)
{
    background.setGreen(arg1);
}

void Settings::on_backB_valueChanged(int arg1)
{
    background.setBlue(arg1);
}

void Settings::on_gamma_valueChanged(double arg1)
{
    gamma = arg1;
}

void Settings::on_quality_valueChanged(int arg1)
{
    quality = arg1;
}

void Settings::on_widthspin_valueChanged(int arg1)
{
    width = arg1;
}

void Settings::on_heightspin_valueChanged(int arg1)
{
    height = arg1;
}

void Settings::on_depth_valueChanged(int arg1)
{
    depth = arg1;
}

void Settings::on_threads_valueChanged(int arg1)
{
    threads = arg1;
}

void Settings::on_buttonBox_accepted()
{
    acceptflag = true;
}
