#ifndef EDITORFIELD_H
#define EDITORFIELD_H

#include <QWidget>
#include <QMouseEvent>
#include <QImage>
#include <QPainter>

class EditorField : public QWidget
{
    Q_OBJECT
    QImage image;
public:
    explicit EditorField(QWidget *parent = nullptr);
    virtual void paintEvent(QPaintEvent *event) override;
    QImage getImage() const;
    void setImage(const QImage &value);
signals:
    void mouseMoveHappened(QMouseEvent *event);
    void mouseReleased(QMouseEvent *event);
    void mousePressed(QMouseEvent *event);
public slots:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;

signals:

public slots:
};

#endif // EDITORFIELD_H
