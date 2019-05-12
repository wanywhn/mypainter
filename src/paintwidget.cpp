#include "paintwidget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

PencilStyle pencilStyle(2, 5);

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent) {
    pencilStyle.init_canvas(800, 600);
}

void PaintWidget::mousePressEvent(QMouseEvent *event) {

    pencilStyle.insert_first(event->pos().x(), event->pos().y());
    auto lop = event->pos();
    auto region = QRegion(lop.x() - 1, lop.y() - 1, 2, 2);
    pencilStyle.draw_internal(region);
//  this->update(region);
}

void PaintWidget::mouseReleaseEvent(QMouseEvent *event) {

    pencilStyle.insert_last(event->pos().x(), event->pos().y());
    auto lop = event->pos();
    auto region = QRegion(lop.x() - 1, lop.y() - 1, 2, 2);
    pencilStyle.draw_internal(region);
    this->update(region);
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event) {

    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }
    pencilStyle.insert(event->pos().x(), event->pos().y());
    auto lop = event->pos();
    auto region = QRegion(lop.x() - 1, lop.y() - 1, 2, 2);
    pencilStyle.draw_internal(region);
    this->update(region);
}

void PaintWidget::paintEvent(QPaintEvent *event) {
    auto &pixmap = pencilStyle.getPixmap();

    QPainter painter(this);
  painter.drawPixmap(event->rect(),pixmap,event->rect());
}

void PaintWidget::resizeEvent(QResizeEvent *event) {
    qDebug() << "resize to " << event->size();
    pencilStyle.resize(event->size().width(), event->size().height());
    QWidget::resizeEvent(event);
}

