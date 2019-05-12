#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>

#include "math/pen_pencil.h"
class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = nullptr);

signals:

public slots:

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

    // QPaintDevice interface
public:
};

#endif // PAINTWIDGET_H
