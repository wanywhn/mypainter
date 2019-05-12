
#ifndef BASICTOOLSPLUGIN_H
#define BASICTOOLSPLUGIN_H

//! [0]
#include <interfaces.h>

#include <QRect>
#include <QObject>
#include <QtPlugin>
#include <QStringList>
#include <QPainterPath>
#include <QImage>

//! [1]
class BasicToolsPlugin : public QObject,
                         public BrushInterface,
//                         public ShapeInterface,
                         public FilterInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.PlugAndPaint.BrushInterface" FILE "basictools.json")
    Q_INTERFACES(BrushInterface ShapeInterface FilterInterface)
//! [0]

//! [2]
public:
//! [1]
    // BrushInterface
    QStringList brushes() const override;
    QRect mousePress(const QString &brush, QPainter &painter,
                     const QPoint &pos) override;
    QRect mouseRelease(const QString &brush, QPainter &painter,
                       const QPoint &pos) override;


    // FilterInterface
    QStringList filters() const override;
    QImage filterImage(const QString &filter, const QImage &image,
                       QWidget *parent) override;

    QRect drawInternal(QPainterPath &path) override;

    QRect mouseMove(const QString &brush, QPainter &painter, const QPoint &newPos) override;
//! [3]
};
//! [2] //! [3]

#endif
