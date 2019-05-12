#include "paintwidget.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent) {

  drawPathIndex = 0;
  back = new QPixmap(800, 600);
  back->fill();
  m_arr_list = z_new_fpoint_arraylist();
}

void PaintWidget::mousePressEvent(QMouseEvent *event) {

  if (event->button() == Qt::LeftButton) {
    if (brushInterface) {
      QPainter painter;
      //            setupPainter(painter);
      brushInterface->mousePress(brushName, painter, event->pos());
      auto rect = this->drawInternal();
      this->update(rect);
    }
  }
}

void PaintWidget::mouseReleaseEvent(QMouseEvent *event) {

  if (event->button() == Qt::LeftButton) {
    if (brushInterface) {
      QPainter painter;
      //            setupPainter(painter);
      brushInterface->mouseRelease(brushName, painter, event->pos());
      auto rect = this->drawInternal();
      this->update(rect);
    }
  }
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event) {

  if ((event->buttons() & Qt::LeftButton)) {
    if (brushInterface) {
      QPainter painter;
      brushInterface->mouseMove(brushName, painter, event->pos());
      auto rect = this->drawInternal();
      update(rect);
    }
  }
}

void PaintWidget::paintEvent(QPaintEvent *event) {

  QPainter painter(this);
  setupPainter(painter);
  painter.drawPixmap(back->rect(), *back, back->rect());

  // TODO 取消重复渲染
  //    painter.drawPath(oneDraw);
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
  brushInterface->setArr_list(&m_arr_list);
  brushInterface->setCur_path(&m_cur_path);
  brushInterface->setW_max(15);
  brushInterface->setW_min(2);
  brushName = name;
}

void PaintWidget::redo() {
  if (stackUndo.empty()) {
    return;
  }
  if (m_arr_list->first == nullptr) {
    m_arr_list->end = stackUndo.pop();
    m_arr_list->cur = m_arr_list->end;
    m_arr_list->first = m_arr_list->end;
  } else {
    m_arr_list->end->n = stackUndo.pop();
    m_arr_list->end = m_arr_list->end->n;
    m_arr_list->cur = m_arr_list->end;
  }
  this->drawInternal();
  this->update();
//  this->update(getChangeRect(m_arr_list->end));
}

QRect PaintWidget::getChangeRect(wpoint_arraylist_node *item) {
  float fx = 0, fy = 0, tx = 0, ty = 0;
  for (auto i = 0; i != item->a->len; ++i) {
    auto point = item->a->point + i * sizeof(wfpoint);
    if (fx > point->p.x) {
      fx = point->p.x;
    }
    if (fy > point->p.y) {
      fy = point->p.y;
    }
    if (tx < point->p.x) {
      tx = point->p.x;
    }
    if (ty < point->p.y) {
      ty = point->p.y;
    }
  }
  return QRectF{fx, fy, tx, ty}.toRect();
}

void PaintWidget::undo() {
  if (!m_arr_list->end || !m_arr_list->first) {
    return;
  }
  if (m_arr_list->first == m_arr_list->end) {
    stackUndo.push(m_arr_list->first);
    m_arr_list->first = m_arr_list->end = nullptr;

  } else {
    auto it = m_arr_list->first;
    while (it && it->n != m_arr_list->end) {
      it = it->n;
    }
    stackUndo.push(m_arr_list->end);
    it->n = nullptr;
    m_arr_list->end = it;
  }
  auto item = stackUndo.top();
  m_arr_list->cur = m_arr_list->end;
  this->drawInternal(item);
  this->drawInternal();
//  auto rect = getChangeRect(item);
  this->update();
}

void PaintWidget::setupPainter(QPainter &painter) {
  // TODO 从这里改变连接处？
  //      brushInterface->
}

QRect PaintWidget::drawInternal(wpoint_arraylist_node *rev) {

  if (!m_arr_list || !m_arr_list->first)
    return QRect{};

  wpoint_arraylist_node *node = rev ? rev : m_arr_list->cur;

  QRegion updateed{0, 0, 0, 0};
  QPen pen;
  int l, t, r, b;
  float fx, fy, tx, ty, fw, tw;
  wpoints_array *path = nullptr;
  wfpoint *p = nullptr, *last = nullptr;

  QPainter painter(back);

  while (node) {
    if (rev) {

    } else {
      m_arr_list->cur = node;
    }

    path = node->a;
    //一个path就是一个连笔
    p = path->point + 0;
    last = path->point + (path->len - 1);

    while (p < last) {
      float fontwid = p->w * brushInterface->w_max();
      pen.setWidthF(fontwid);

      fx = p->p.x;
      fy = p->p.y;
      fw = p->w;
      p++;
      tx = p->p.x;
      ty = p->p.y;
      tw = p->w;

      // FIXME 有跳跃？
      static QLineF line;
      line.setLine(fx, fy, tx, ty);
      //            QPainterPath tmp(QPointF(fx, fy));
      //            tmp.lineTo(tx, ty);
      if(rev){
      pen.setColor(Qt::white);
      }
      painter.setPen(pen);
      painter.drawLine(line);
      //            painterPath.connectPath(stroker.createStroke(tmp));
      //            tmp=tmp.united(stroker.createStroke(tmp));
      //    painterPath.addPath(stroker.createStroke(tmp));
      //            painterPath.addPath(tmp);
      if (fx < tx) {
        l = fx;
        r = tx;
      } else {
        r = fx;
        l = tx;
      }

      if (fy < ty) {
        t = fy;
        b = ty;
      } else {
        b = fy;
        t = ty;
      }
      updateed = updateed.united(QRect(QPoint(l - fontwid, t - fontwid),
                                       QPoint(r + fontwid, b + fontwid)));
    };

    if(rev){
        break;
    }
    node = node->n;
    //            m_cur_last_index = 0;
  };

  m_arr_list->cur = m_arr_list->end;
  return updateed.boundingRect();
}
