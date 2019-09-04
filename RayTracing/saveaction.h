#ifndef SAVEACTION_H
#define SAVEACTION_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class SaveAction;
}

class SaveAction : public QDialog
{
    Q_OBJECT
    QString name = "image";
    bool accept = false;
public:
    explicit SaveAction(QWidget *parent = 0);
    ~SaveAction();

    QString getName() const;

    bool getAccept() const;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SaveAction *ui;
};

#endif // SAVEACTION_H
