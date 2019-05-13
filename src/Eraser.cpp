//
// Created by tender on 19-5-13.
//

#include <QtGui/QPainter>
#include "Eraser.h"

QRect Eraser::mousePress(const QString &brush, QImage *image, const QPoint &pos) {
    mStart=mEnd=pos;
    return QRect();
}

QRect Eraser::mouseMove(const QString &brush, QImage *image, const QPoint &newPos) {
    mEnd=newPos;
    auto rect=drawInternal(image);
    mStart=mEnd;
    return rect;
}

QRect Eraser::mouseRelease(const QString &brush, QImage *image, const QPoint &pos) {
    mEnd=pos;
    auto rect=drawInternal(image);
    return rect;
}

QRect Eraser::drawInternal(QPaintDevice *path) {

    QPainter painter(path);
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.drawLine(mStart,mEnd);
    return QRect(mStart,mEnd);
}

Eraser::Eraser(QObject *parent) : QObject(parent) {}
