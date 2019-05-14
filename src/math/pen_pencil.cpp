#include "pen_pencil.h"

#include <QPainter>
#include <stdio.h>
#include <QDebug>


QRect PencilStyle::drawInternal(QPaintDevice *device) {

    wpoint_arraylist *m_arr_list = base->getM_arr_list();
    if (!m_arr_list || !m_arr_list->first) return QRect{};

    wpoint_arraylist_node *node = m_arr_list->cur;

    QPainter painter(device);
    painter.setRenderHint(QPainter::RenderHint::HighQualityAntialiasing, true);
    auto pen = painter.pen();
    pen.setCapStyle(Qt::PenCapStyle::RoundCap);
    pen.setColor(this->color);
    painter.setPen(pen);

    QRegion updateed{0, 0, 0, 0};

    //TODO setup pen
    int l, t, r, b;
    float fx, fy, tx, ty, fw, tw;
    wpoints_array *path = nullptr;
    wfpoint *p = nullptr, *last = nullptr;

    while (node) {
        m_arr_list->cur = node;

        path = node->a;
        p = path->point;//+ m_cur_last_index;
        last = path->point + (path->len - 1);


        while (p < last) {
            float fontwid = p->w * base->getM_w_max();
            QPen pen(painter.pen());
            pen.setWidthF(fontwid);
            painter.setPen(pen);

            fx = p->p.x;
            fy = p->p.y;
            fw = p->w;
            p++;
            tx = p->p.x;
            ty = p->p.y;
            tw = p->w;

            QLineF line(fx, fy, tx, ty);
            painter.drawLine(line);
            //FIXME 有跳跃？
            //TODO get rect

            if (fx > tx) {
                l = tx;
                r = fx;
            } else {
                r = tx;
                l = fx;
            }
            if (fy > ty) {
                t = ty;
                b = fy;
            } else {
                b = ty;
                t = fy;

            }
            QPoint tl(l - fontwid, t - fontwid);
            QPoint br(r + fontwid, b + fontwid);
            updateed = updateed.united(QRect(tl, br));
        };

        node = node->n;
//        m_cur_last_index = 0;
    };

    m_arr_list->cur = m_arr_list->end;

    return updateed.boundingRect();
}


QRect PencilStyle::mousePress(const QString &brush, QImage *image, const QPointF &pos) {
    //TODO set line width
    base->setWidth(width);
    base->insert_first(pos.x(), pos.y());
    return drawInternal(image);
}

QRect PencilStyle::mouseMove(const QString &brush, QImage *image, const QPointF &newPos) {
    base->insert(newPos.x(), newPos.y());
    return drawInternal(image);
}

QRect PencilStyle::mouseRelease(const QString &brush, QImage *image, const QPointF &pos) {
    base->insert_last(pos.x(), pos.y());
    return drawInternal(image);
}

PencilStyle::PencilStyle(QObject *parent) : QObject(parent) {
    base = new BezierBase;
}




