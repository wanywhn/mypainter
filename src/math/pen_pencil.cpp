#include "pen_pencil.h"

#include <QPainter>
#include <stdio.h>
#include <QDebug>


PencilStyle::~PencilStyle() {

    if (m_mem_pixmap) delete m_mem_pixmap;
    if (m_cur_path) z_drop_fpoint_array(m_cur_path);
    if (m_arr_list) z_drop_fpoint_arraylist(m_arr_list);
}


void PencilStyle::insert_first(int x, int y) {
    spoint point = {(float) x, (float) y};
    if (m_cur_path)
        z_drop_fpoint_array(m_cur_path);

    m_cur_path = z_fpoint_arraylist_append_new(m_arr_list, m_w_max, m_w_min);

    z_insert_point(m_cur_path, point);
}

void PencilStyle::insert(int x, int y) {
    spoint point = {(float) x, (float) y};
    if (!m_cur_path)
        m_cur_path = z_fpoint_arraylist_append_new(m_arr_list, m_w_max, m_w_min);

    z_insert_point(m_cur_path, point);
}

void PencilStyle::insert_last(int x, int y) {
    if (!m_cur_path)
        return;
    spoint point = {(float) x, (float) y};
    z_insert_last_point(m_cur_path, point);

    z_drop_fpoint_array(m_cur_path);
    m_cur_path = nullptr;
}

QRect PencilStyle::drawInternal(QPaintDevice *device) {

    if (!m_arr_list || !m_arr_list->first) return QRect{};

    wpoint_arraylist_node *node = m_arr_list->cur;

    QPainter painter(device);

    QRegion updateed{0, 0, 0, 0};


    //TODO setup pen
    int l, t, r, b;
    float fx, fy, tx, ty, fw, tw;
    wpoints_array *path = NULL;
    wfpoint *p = NULL, *last = NULL;

    while (node) {
        m_arr_list->cur = node;

        path = node->a;
        p = path->point + m_cur_last_index;
        last = path->point + (path->len - 1);


        while (p < last) {
            float fontwid = p->w * m_w_max;
            QPen pen(painter.pen());
            pen.setWidth(fontwid);
            painter.setPen(pen);

            fx = p->p.x;
            fy = p->p.y;
            fw = p->w;
            p++;
            tx = p->p.x;
            ty = p->p.y;
            tw = p->w;

            painter.drawLine(fx,fy,tx,ty);
            //FIXME 有跳跃？
            //TODO get rect
            QPoint tl(fx,fy);
            QPoint br(tx,ty);
            updateed = updateed.united(QRect(tl,br));
        };

        node = node->n;
        m_cur_last_index = 0;
    };

    m_arr_list->cur = m_arr_list->end;

    return updateed.boundingRect();


}




QRect PencilStyle::mousePress(const QString &brush, QImage *image, const QPoint &pos) {
    //TODO set line width
    this->insert_first(pos.x(), pos.y());
    return drawInternal(image);
}

QRect PencilStyle::mouseMove(const QString &brush, QImage *image, const QPoint &newPos) {
    this->insert(newPos.x(), newPos.y());
    return drawInternal(image);
}

QRect PencilStyle::mouseRelease(const QString &brush, QImage *image, const QPoint &pos) {
    this->insert_last(pos.x(), pos.y());
    return drawInternal(image);
}

PencilStyle::PencilStyle(QObject *parent) : QObject(parent) {
    m_arr_list = z_new_fpoint_arraylist();
    this->m_w_max = 10;
    this->m_w_min = 1;
}




