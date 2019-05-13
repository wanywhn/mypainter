//
// Created by tender on 19-5-13.
//

#include "Earser.h"

QStringList Earse::brushes() const {
    return QStringList("Earser");
}

QRect Earse::mousePress(const QString &brush, QImage *image, const QPoint &pos) {
    QPainterPath tmp;
    earserPath.swap(tmp);
    earserPath.moveTo(pos);

    return QRect();
}

QRect Earse::mouseMove(const QString &brush, QImage *image, const QPoint &newPos) {
    earserPath.lineTo(newPos);
    return QRect();
}

QRect Earse::mouseRelease(const QString &brush, QImage *image, const QPoint &pos) {
    earserPath.lineTo(pos);
    return QRect();
}

QRect Earse::drawInternal(QPaintDevice *path) {
    stroker.setWidth(width);
    *path=path->united(stroker.createStroke(earserPath));
    return path->boundingRect().toRect();
}

void Earse::draw(QPainter *painter) {

    for (auto item:*drawPathObj.paintPath) {
        if (drawPathObj.paintPathType->value(item) == 0) {
//            painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter->fillPath(*item,drawPathObj.painterPathColor->value(item,Qt::green));
        } else {
            painter->setCompositionMode(QPainter::CompositionMode_Clear);
            painter->fillPath(*item,QBrush(Qt::transparent));
        }
    }
}

CommandInterface *Earse::createCommand(DrawPathParameter parameter) {
    return new EarserCommonCommand(parameter);
}

QRect EarserCommonCommand::undo() {
    return QRect();
}

QRect EarserCommonCommand::redo() {
    return QRect();
}
