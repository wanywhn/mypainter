#include "pen_pencil.h"

#include <QPainter>
#include <stdio.h>
#include <QDebug>


PencilStyle::~PencilStyle() {

}


void PencilStyle::insert_first(int x, int y) {
    spoint point = {(float) x, (float) y};
    if (m_cur_path&&*m_cur_path)
        z_drop_fpoint_array(*m_cur_path);

    *m_cur_path = z_fpoint_arraylist_append_new(*m_arr_list, m_w_max, m_w_min);

    z_insert_point(*m_cur_path, point);
}

void PencilStyle::insert(int x, int y) {
    spoint point = {(float) x, (float) y};
    if (!m_cur_path&&!m_cur_path)
        *m_cur_path = z_fpoint_arraylist_append_new(*m_arr_list, m_w_max, m_w_min);

    z_insert_point(*m_cur_path, point);
}

void PencilStyle::insert_last(int x, int y) {
    if (!m_cur_path)
        return;
    spoint point = {(float) x, (float) y};
    z_insert_last_point(*m_cur_path, point);

    z_drop_fpoint_array(*m_cur_path);
    *m_cur_path = nullptr;
}





QStringList PencilStyle::brushes() const {
    return QStringList("Pencil");
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
    m_w_max=15;
    m_w_min=2;
}

