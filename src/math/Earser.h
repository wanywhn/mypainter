//
// Created by tender on 19-5-13.
//

#ifndef MPAINTER_EARSER_H
#define MPAINTER_EARSER_H

#include <interfaces.h>
#include <QPainterPath>
#include <QPainter>

class EarserCommonCommand: public CommandInterface{

public:
    explicit EarserCommonCommand(DrawPathParameter parameter):m_parameter(parameter) {}

    // CommandInterface interface
public:
    QRect undo() Q_DECL_OVERRIDE;
    QRect redo() Q_DECL_OVERRIDE;

private:
    DrawPathParameter m_parameter{};
};
class Earse:public QObject ,public BrushInterface{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.PlugAndPaint.BrushInterface" FILE "basicearser.json")
    Q_INTERFACES(BrushInterface)
public:
    QStringList brushes() const override;

    QRect mousePress(const QString &brush, QImage *image, const QPoint &pos) override;

    QRect mouseMove(const QString &brush, QImage *image, const QPoint &newPos) override;

    QRect mouseRelease(const QString &brush, QImage *image, const QPoint &pos) override;

    QRect drawInternal(QPaintDevice *path) override;

    void draw(QPainter *painter) override;


private:
    QPainterPath earserPath;
    QPainterPathStroker stroker;

};



#endif //MPAINTER_EARSER_H
