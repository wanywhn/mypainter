#include "paintwidget.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent) {

  drawPathIndex = 0;
  back=new QPixmap(800,600);
  back->fill();
}

void PaintWidget::mousePressEvent(QMouseEvent *event) {

  if (event->button() == Qt::LeftButton) {
    if (brushInterface) {
      QPainter painter;
      //            setupPainter(painter);
      brushInterface->mousePress(brushName, painter, event->pos());
      oneDraw = oneDraw.subtracted(oneDraw);
      auto rect = brushInterface->drawInternal(oneDraw);
      paintPath.push_back(oneDraw);
      this->update(rect);
      if (paintPath.size() > 15) {
        painter.begin(back);
        for (auto it = paintPath.cbegin(); it != paintPath.cbegin() + 6; ++it) {
        painter.drawPath(*it);
        }
        painter.end();
        paintPath.erase(paintPath.begin(),paintPath.begin()+6);
      }
    }
  }
}

void PaintWidget::mouseReleaseEvent(QMouseEvent *event) {

  if (event->button() == Qt::LeftButton) {
    if (brushInterface) {
      QPainter painter;
      //            setupPainter(painter);
      brushInterface->mouseRelease(brushName, painter, event->pos());
      auto &d = paintPath[paintPath.size() - 1];
      auto rect = brushInterface->drawInternal(d);
      //            oneDraw.setFillRule(Qt::WindingFill);
      //            oneDraw = oneDraw.simplified();
      //            paintPath.push_back(oneDraw);
      //            paintPath.end()->addPath(oneDraw);
      this->update(rect);
    }
  }
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event) {

  if ((event->buttons() & Qt::LeftButton)) {
    if (brushInterface) {
      QPainter painter;
      //            setupPainter(painter);
      brushInterface->mouseMove(brushName, painter, event->pos());
      auto &d = paintPath[paintPath.size() - 1];
      auto rect = brushInterface->drawInternal(d);
      //            paintPath.end()->addPath(oneDraw);
      update(rect);
    }
  }
  this->update();
}

void PaintWidget::paintEvent(QPaintEvent *event) {

  QPainter painter(this);
  painter.drawPixmap(0,0,*back);
  // TODO 取消重复渲染
  //    painter.drawPath(oneDraw);
  setupPainter(painter);
  //    for (auto i = drawPathIndex-1; i < paintPath.size(); ++i) {
  for (const auto &i : paintPath) {
    //        if(i>=0){
    //            paintPath[i]=paintPath[i].simplified();
    //        painter.drawPath(paintPath[i]);
    painter.drawPath(i);
    //        }
  }
  //    drawPathIndex = paintPath.size();
}

void PaintWidget::resizeEvent(QResizeEvent *event) {
  QWidget::resizeEvent(event);
}

int PaintWidget::brushWidth() { return penWidth; }

const QColor &PaintWidget::brushColor() { return color; }

void PaintWidget::setBrushColor(const QColor &color) { this->color = color; }

void PaintWidget::setBrushAlpha(int alpha) {}

void PaintWidget::setBrushWidth(int width) { this->penWidth = width; }

void PaintWidget::setBrush(BrushInterface *i, const QString &name) {
  this->brushInterface = i;
  brushName = name;
}

void PaintWidget::setupPainter(QPainter &painter) {
  // TODO 从这里改变连接处？
  //    painter.setPen(QPen(color, penWidth, Qt::SolidLine));
}
