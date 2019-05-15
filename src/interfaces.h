#ifndef INTERFACES_H
#define INTERFACES_H

#include <QColor>
#include <QRect>
#include <QMap>
#include <undocommand.h>

class BrushInterface:public CommandInterface{

public:
    virtual ~BrushInterface() {}

    virtual QRect mousePress(const QString &brush, QImage *image,
                             const QPointF &pos) = 0;
    virtual QRect mouseMove(const QString &brush, QImage *image,
                            const QPointF &newPos) = 0;
    virtual QRect mouseRelease(const QString &brush, QImage *image,
                               const QPointF &pos) = 0;
    virtual QRect drawInternal(QPaintDevice *path)=0;
    void setColor(QColor color){this->color= color;}
    void setWidget(int wid){this->width=wid;}

    const QColor &getColor() const {
        return color;
    }

    int getAlpha() const {
        return alpha;
    }

    void setAlpha(int alpha) {
        BrushInterface::alpha = alpha;
    }
    int getWidth(){return width;}
public:

protected:
    QColor color;

protected:
    int width{10};
    int alpha;


};




#endif
