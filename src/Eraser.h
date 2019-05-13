//
// Created by tender on 19-5-13.
//

#ifndef MPAINTER_ERASER_H
#define MPAINTER_ERASER_H


#include "interfaces.h"

class Eraser : public QObject, public BrushInterface{
public:
    explicit Eraser(QObject *parent);

    QRect mousePress(const QString &brush, QImage *image, const QPoint &pos) override;

    QRect mouseMove(const QString &brush, QImage *image, const QPoint &newPos) override;

    QRect mouseRelease(const QString &brush, QImage *image, const QPoint &pos) override;

    QRect drawInternal(QPaintDevice *path) override;

private:
    QPoint mStart,mEnd;

};


#endif //MPAINTER_ERASER_H
