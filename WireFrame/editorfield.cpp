#include "editorfield.h"

EditorField::EditorField(QWidget *parent) : QWidget(parent)
{

}


QImage EditorField::getImage() const
{
    return image;
}

void EditorField::setImage(const QImage &value)
{
    image = value;
    update();
}

void EditorField::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMoveHappened(event);
}

void EditorField::mouseReleaseEvent(QMouseEvent *event)
{
    emit mouseReleased(event);
}

void EditorField::mousePressEvent(QMouseEvent *event)
{
    emit mousePressed(event);
}

void EditorField::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(0,0,image);
}
