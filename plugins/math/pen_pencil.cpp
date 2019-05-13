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

QRect PencilStyle::drawInternal(QPainterPath *ppath) {

    if (!m_arr_list || !m_arr_list->first) return QRect{};

    wpoint_arraylist_node *node = m_arr_list->cur;


    QRegion updateed{0, 0, 0, 0};


    QPainterPath painterPath;
    QPainterPathStroker stroker;
    stroker.setCapStyle(Qt::RoundCap);
    int l, t, r, b;
    float fx, fy, tx, ty, fw, tw;
    wpoints_array *path = NULL;
    wfpoint *p = NULL, *last = NULL;

    while (node) {
        m_arr_list->cur = node;

        path = node->a;
        //一个path就是一个连笔
        p = path->point + m_cur_last_index;
        last = path->point + (path->len - 1);


        while (p < last) {
            float fontwid = p->w * m_w_max;
            stroker.setWidth(fontwid);
//            pen.setWidthF(fontwid);


            fx = p->p.x;
            fy = p->p.y;
            fw = p->w;
            p++;
            tx = p->p.x;
            ty = p->p.y;
            tw = p->w;

            QPainterPath tmp(QPointF(fx, fy));
            //FIXME 有跳跃？
            tmp.lineTo(tx, ty);
//            painterPath.connectPath(stroker.createStroke(tmp));
            painterPath = painterPath.united(stroker.createStroke(tmp));
//            painterPath.addPath(stroker.createStroke(tmp));
            updateed = updateed.united(painterPath.boundingRect().toRect());
        };

        node = node->n;
        m_cur_last_index = 0;
    };
    *ppath = ppath->united(painterPath);
//    ppath.addPath(painterPath);

    m_arr_list->cur = m_arr_list->end;

    return updateed.boundingRect();


}

CommandInterface *PencilStyle::createCommand(DrawPathParameter parameter) {
    return new PenCommonCommand(parameter);

}

void PencilStyle::draw(QPainter *painter, DrawPathParameter drawPathObj) {
    // TODO 取消重复渲染
    for (auto item:*drawPathObj.paintPath) {
        painter->fillPath(*item, drawPathObj.painterPathColor->value(item, Qt::black));

    }
}



QStringList PencilStyle::brushes() const {
    return QStringList({"Pencil" });
}

QRect PencilStyle::mousePress(const QString &brush, QPainter &painter, const QPoint &pos) {
    this->insert_first(pos.x(), pos.y());
    return QRect();
}

QRect PencilStyle::mouseMove(const QString &brush, QPainter &painter, const QPoint &newPos) {
    this->insert(newPos.x(), newPos.y());
    return QRect();
}

QRect PencilStyle::mouseRelease(const QString &brush, QPainter &painter, const QPoint &pos) {
    this->insert_last(pos.x(), pos.y());
    return QRect();
}

PencilStyle::PencilStyle(QObject *parent) : QObject(parent) {
    m_arr_list = z_new_fpoint_arraylist();
    this->m_w_max = 10;
    this->m_w_min = 1;
}


PenCommonCommand::PenCommonCommand(DrawPathParameter parameter) : m_parameter(parameter) {

}

QRect PenCommonCommand::undo() {
//    std::remove(m_v->begin(),m_v->end(),m_path);
//!! warnning
    auto rect = m_parameter.paintPath->back()->boundingRect().toRect();
    m_parameter.paintPath->pop_back();
    return rect;
}

QRect PenCommonCommand::redo() {
    m_parameter.paintPath->push_back(m_parameter.oneDraw);
    return m_parameter.oneDraw->boundingRect().toRect();

}


