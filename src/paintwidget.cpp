#include "paintwidget.h"

#include <QDebug>
#include <QFileDialog>
#include <QMouseEvent>
#include <QPainter>

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent) {

  image=new QImage(800,600,QImage::Format_ARGB32_Premultiplied);
  image->fill(Qt::transparent);
  this->undoStack=new QUndoStack(this);
}

void PaintWidget::mousePressEvent(QMouseEvent *event) {

  if (event->button() == Qt::LeftButton) {
    if (brushInterface) {
//      setupPainter(painter);
      brushInterface->makeUndoCommand(*this);
//brushInterface->anInterface.makeUndoCommand(*this);
      auto rect=brushInterface->mousePress(brushName,image, event->pos());
      //!!warning 这个颜色究竟用什么？
      this->update(rect);
    }
  }
}

void PaintWidget::mouseReleaseEvent(QMouseEvent *event) {

  if (event->button() == Qt::LeftButton) {
    if (brushInterface) {
//      QPainter painter(image);
//      setupPainter(painter);
      auto rect=brushInterface->mouseRelease(brushName,image, event->pos());
      this->update(rect);
    }
  }
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event) {

  if ((event->buttons() & Qt::LeftButton)) {
    if (brushInterface) {
      auto rect=brushInterface->mouseMove(brushName,image, event->pos());
      update(rect);
    }
  }
}

void PaintWidget::paintEvent(QPaintEvent *event) {

  QPainter painter(this);
  painter.drawImage(0,0,*image);

//  painter.drawImage(0, 0, image);
//  if (brushInterface) {
//    DrawPathParameter parameter;
    //      parameter.oneDraw=oneDraw;
//    parameter.paintPath = &paintPath;

//    brushInterface->draw(&painter, parameter);
//  }
}

void PaintWidget::resizeEvent(QResizeEvent *event) {
  QWidget::resizeEvent(event);
}

int PaintWidget::brushWidth() { return penWidth; }

const QColor &PaintWidget::brushColor() { return color; }

void PaintWidget::pushUndoStack(UndoCommand *comm) {
  if (comm)
    this->undoStack->push(comm);
}

void PaintWidget::setBrushColor(const QColor &color) {
  this->color = color;
  brushInterface->setColor(color);
}

void PaintWidget::setBrushAlpha(int alpha) {}

void PaintWidget::setBrushWidth(int width) {
  this->penWidth = width;
  brushInterface->setWidget(width);
}

void PaintWidget::setBrush(BrushInterface *i, const QString &name) {
  this->brushInterface = i;
  brushName = name;
}


void PaintWidget::setupPainter(QPainter &painter) {
  // TODO 从这里改变连接处？
      painter.setPen(QPen(color, penWidth, Qt::SolidLine));
}

QUndoStack *PaintWidget::getUndoStack() const
{
    return undoStack;
}

QImage *PaintWidget::getImage()
{
    return image;
}

void PaintWidget::setImage(QImage value)
{
    *image = value;
}

void PaintWidget::saveFile() {
    auto filepath =
            QFileDialog::getSaveFileUrl(this, tr("Save Path"), QDir::homePath());
  image->save(filepath.toLocalFile(), "PNG");
}
