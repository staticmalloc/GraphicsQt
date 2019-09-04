#include "saveaction.h"
#include "ui_saveaction.h"

QString SaveAction::getName() const
{
    return name;
}

bool SaveAction::getAccept() const
{
    return accept;
}

SaveAction::SaveAction(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveAction)
{
    ui->setupUi(this);
}

SaveAction::~SaveAction()
{
    delete ui;
}

void SaveAction::on_buttonBox_accepted()
{
    if(ui->lineEdit->text().isEmpty())
        QMessageBox::critical(this,"Error","Empty filename",QMessageBox::Ok);
    else
    {
        accept = true;
        name = ui->lineEdit->text()+".png";
    }
}
