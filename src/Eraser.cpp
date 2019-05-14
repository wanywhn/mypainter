//
// Created by tender on 19-5-13.
//

#include <QtGui/QPainter>
#include "Eraser.h"

QRect Eraser::mousePress(const QString &brush, QImage *image, const QPointF &pos) {
    mStart=mEnd=pos;
    return QRect();
}

QRect Eraser::mouseMove(const QString &brush, QImage *image, const QPointF &newPos) {
    mEnd=newPos;
    auto rect=drawInternal(image);
    mStart=mEnd;
    return rect;
}

QRect Eraser::mouseRelease(const QString &brush, QImage *image, const QPointF &pos) {
    mEnd=pos;
    auto rect=drawInternal(image);
    return rect;
}

QRect Eraser::drawInternal(QPaintDevice *path) {

    QPainter painter(path);
    pen.setWidthF(width);
    painter.setPen(pen);
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    if(mStart==mEnd){
        painter.drawPoint(mStart);

    }else{
        painter.drawLine(mStart,mEnd);
    }
    QPoint adj(width/2,width/2);
    QPoint tl;
    QPoint br;
    tl.setX(qMin(mStart.x(),mEnd.x()));
    tl.setY(qMin(mStart.y(),mEnd.y()));
    br.setX(qMax(mStart.x(),mEnd.x()));
    br.setY(qMax(mStart.y(),mEnd.y()));

    return QRect{tl-adj,br+adj};
}

Eraser::Eraser(QObject *parent) : QObject(parent) {
}
