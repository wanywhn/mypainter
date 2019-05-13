#ifndef INTERFACES_H
#define INTERFACES_H

#include <QColor>
#include <QRect>
#include <QMap>
#include <undocommand.h>

//FIXME 当CommandInterface有虚函数的时候，QtPlugin系统貌似不能起作用?
class BrushInterface:public CommandInterface{

public:
    virtual ~BrushInterface() {}

    virtual QStringList brushes() const = 0;
    virtual QRect mousePress(const QString &brush, QImage *image,
                             const QPoint &pos) = 0;
    virtual QRect mouseMove(const QString &brush, QImage *image,
                            const QPoint &newPos) = 0;
    virtual QRect mouseRelease(const QString &brush, QImage *image,
                               const QPoint &pos) = 0;
    virtual QRect drawInternal(QPaintDevice *path)=0;
    virtual void draw(QPainter *painter) =0;
    void setColor(QColor color){this->color= color;}
    void setWidget(int wid){this->width=wid;}

public:
//    CommandInterface anInterface;

protected:
    QColor color;
    int width;


};

class FilterInterface
{
public:
    virtual ~FilterInterface() {}

    virtual QStringList filters() const = 0;
    virtual QImage filterImage(const QString &filter, const QImage &image,
                               QWidget *parent) = 0;
};

/*
QT_BEGIN_NAMESPACE
#define BrushInterface_iid "org.qt-project.Qt.Examples.PlugAndPaint.BrushInterface"

Q_DECLARE_INTERFACE(BrushInterface, BrushInterface_iid)

#define FilterInterface_iid "org.qt-project.Qt.Examples.PlugAndPaint.FilterInterface"

Q_DECLARE_INTERFACE(FilterInterface,FilterInterface_iid)

QT_END_NAMESPACE
*/

#endif
