#ifndef OPENACTION_H
#define OPENACTION_H

#include <QDialog>
#include <QFileDialog>
#include <QDir>

namespace Ui {
class openaction;
}

class openaction : public QDialog
{
    Q_OBJECT
    QString filename = "";
    bool accept = false;
public:
    explicit openaction(QWidget *parent = 0);
    ~openaction();

    QString getFilename() const;

    bool getAccept() const;

private slots:
    void on_pushButton_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_lineEdit_editingFinished();

private:
    Ui::openaction *ui;
};

#endif // OPENACTION_H
