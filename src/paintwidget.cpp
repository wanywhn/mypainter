#include "paintwidget.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QFileDialog>

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent) {

  drawPathIndex = 0;
  back=new QImage(800,600,QImage::Format_ARGB32_Premultiplied);
  back->fill(Qt::transparent);
}

void PaintWidget::mousePressEvent(QMouseEvent *event) {

  if (event->button() == Qt::LeftButton) {
    if (brushInterface) {
      QPainter painter;
      //            setupPainter(painter);
      brushInterface->mousePress(brushName, painter, event->pos());
      oneDraw=new QPainterPath();
      //!!warning 这个颜色究竟用什么？
      paintPath.push_back(oneDraw);
      if(brushName=="Earser"){

          QColor t(color);
//          t.setAlpha(0);
          paintPathType.insert(oneDraw,1);
          paintPathColor.insert(oneDraw,t);
      }else{
          paintPathType.insert(oneDraw,0);
          paintPathColor.insert(oneDraw,color);
      }
      auto rect = brushInterface->drawInternal(oneDraw);
      this->update(rect);
      if (paintPath.size() > 15) {
        painter.begin(back);
        for (auto it = paintPath.cbegin(); it != paintPath.cbegin() + 6; ++it) {
        painter.fillPath(*(*it),paintPathColor.value(*it,Qt::green));
        paintPathColor.remove(*it);
        paintPathType.remove(*it);
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
      brushInterface->mouseRelease(brushName, painter, event->pos());
      auto rect = brushInterface->drawInternal(paintPath.last());
      this->update(rect);
      DrawPathParameter parameter;
      //TODO 这个是用来记录这条命令所画的线的
      parameter.oneDraw=paintPath.last();
      parameter.paintPath=&paintPath;
      parameter.painterPathColor=&paintPathColor;
      parameter.paintPathType=&paintPathType;
      commandUndo.push(brushInterface->createCommand(parameter));
      oneDraw=nullptr;
      emit undoEmpty(false);
    }
  }
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event) {

  if ((event->buttons() & Qt::LeftButton)) {
    if (brushInterface) {
      QPainter painter;
      //            setupPainter(painter);
      brushInterface->mouseMove(brushName, painter, event->pos());
      auto rect = brushInterface->drawInternal(paintPath.last());
      update(rect);
    }
  }
  this->update();
}

void PaintWidget::paintEvent(QPaintEvent *event) {

  QPainter painter(this);

  painter.drawImage(0,0,*back);
  if(brushInterface){
      DrawPathParameter parameter;
//      parameter.oneDraw=oneDraw;
      parameter.paintPath=&paintPath;
      parameter.painterPathColor=&paintPathColor;
      parameter.paintPathType=&paintPathType;

      brushInterface->draw(&painter, parameter);
  }

}

void PaintWidget::resizeEvent(QResizeEvent *event) {
  QWidget::resizeEvent(event);
}

int PaintWidget::brushWidth() { return penWidth; }

const QColor &PaintWidget::brushColor() { return color; }

void PaintWidget::setBrushColor(const QColor &color) { this->color = color;brushInterface->setColor(color) ;}

void PaintWidget::setBrushAlpha(int alpha) {}

void PaintWidget::setBrushWidth(int width) { this->penWidth = width;brushInterface->setWidget(width); }

void PaintWidget::setBrush(BrushInterface *i, const QString &name) {
  this->brushInterface = i;
    brushName = name;
}

void PaintWidget::undo()
{
    if(commandUndo.empty()){
        emit undoEmpty(true);
        return;
    }
    emit redoEmpty(false);
    auto com=commandUndo.pop();
    auto rect=com->undo();
    update(rect);
    commandRedo.push(com);
    emit undoEmpty(commandUndo.empty());
    //repaint
}

void PaintWidget::redo()
{
    if(commandRedo.empty()){
        emit redoEmpty(true);
        return ;
    }
        emit undoEmpty(false);
    auto com=commandRedo.pop();
    auto rect=com->redo();
    update(rect);
    commandUndo.push(com);
    emit redoEmpty(commandRedo.empty());
}

void PaintWidget::setupPainter(QPainter &painter) {
  // TODO 从这里改变连接处？
  //    painter.setPen(QPen(color, penWidth, Qt::SolidLine));
}

void PaintWidget::saveFile() {
    auto filepath=QFileDialog::getSaveFileUrl(this,tr("Save Path"),QDir::homePath());
    back->save(filepath.toLocalFile(),"PNG");

}
