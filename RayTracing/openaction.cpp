#include "openaction.h"
#include "ui_openaction.h"

QString openaction::getFilename() const
{
    return filename;
}

bool openaction::getAccept() const
{
    return accept;
}

openaction::openaction(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::openaction)
{
    ui->setupUi(this);
}

openaction::~openaction()
{
    delete ui;
}

void openaction::on_pushButton_clicked()
{
    filename = QFileDialog::getOpenFileName(this,"Open scene",QCoreApplication::applicationDirPath(),"*.scene");
    ui->lineEdit->setText(filename);
}

void openaction::on_buttonBox_accepted()
{
    accept = true;
}

void openaction::on_buttonBox_rejected()
{
    accept = false;
    if(!filename.isEmpty())
        filename = "";
}

void openaction::on_lineEdit_editingFinished()
{
    filename = ui->lineEdit->text();
}
