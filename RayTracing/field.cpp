#include "field.h"

Field::Field(QWidget *parent) : QWidget(parent)
{

}

QImage Field::getImage() const
{
    return image;
}

void Field::setImage(const QImage &value)
{
    image = value;
    update();
}

void Field::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMoveHappened(event);
}

void Field::mouseReleaseEvent(QMouseEvent *event)
{
    emit mouseReleased(event);
}

void Field::mousePressEvent(QMouseEvent *event)
{
    emit mousePressed(event);
}

void Field::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(0,0,image);
}
