#ifndef PEN_PENCIL_H
#define PEN_PENCIL_H

#include "bezier.h"
#include <QPixmap>

#include <QPainter>
#include <interfaces.h>

class PencilStyle : public QObject, public BrushInterface {
Q_OBJECT
public:
    explicit PencilStyle(QObject *parent = nullptr);

private:


public:

    QRect mousePress(const QString &brush, QImage *image, const QPointF &pos) override;

    QRect mouseMove(const QString &brush, QImage *image, const QPointF &newPos) override;

    QRect mouseRelease(const QString &brush, QImage *image, const QPointF &pos) override;

    QRect drawInternal(QPaintDevice *device) override;

private:
    BezierBase *base;


};

#endif //PEN_PELCIL_H
