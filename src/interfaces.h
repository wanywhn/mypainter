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
                             const QPoint &pos) = 0;
    virtual QRect mouseMove(const QString &brush, QImage *image,
                            const QPoint &newPos) = 0;
    virtual QRect mouseRelease(const QString &brush, QImage *image,
                               const QPoint &pos) = 0;
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
    int width{5};
    int alpha;


};



class FilterInterface
{
public:
    virtual ~FilterInterface() {}

    virtual QStringList filters() const = 0;
    virtual QImage filterImage(const QString &filter, const QImage &image,
                               QWidget *parent) = 0;
};


#endif
