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
      brushInterface->makeUndoCommand(*this);
      auto rect=brushInterface->mousePress(brushName,image, event->pos());
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

int PaintWidget::brushWidth() { return brushInterface->getWidth(); }

const QColor &PaintWidget::brushColor() { return brushInterface->getColor(); }

void PaintWidget::pushUndoStack(UndoCommand *comm) {
  if (comm)
    this->undoStack->push(comm);
}
void PaintWidget::setBrushColor(QColor &color) {
  color.setAlpha(alpha);
  brushInterface->setColor(color);
}
void PaintWidget::setBrushAlpha(int alpha) {
  this->alpha = alpha;
  auto c=brushInterface->getColor();
  c.setAlpha(alpha);
  brushInterface->setColor(c);

}
void PaintWidget::setBrushWidth(int width) {
  brushInterface->setWidget(width);
}

void PaintWidget::setBrush(BrushInterface *i, const QString &name) {
  this->brushInterface = i;
  brushName = name;
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

float PaintWidget::brushAlpha() {
    return alpha;
}
