//
// Created by tender on 19-5-13.
//

#include "Earser.h"

QStringList Earse::brushes() const {
    return QStringList("Earser");
}

QRect Earse::mousePress(const QString &brush, QPainter &painter, const QPoint &pos) {
    earserPath.moveTo(pos);

    return QRect();
}

QRect Earse::mouseMove(const QString &brush, QPainter &painter, const QPoint &newPos) {
    earserPath.lineTo(newPos);
    return QRect();
}

QRect Earse::mouseRelease(const QString &brush, QPainter &painter, const QPoint &pos) {
    earserPath.lineTo(pos);
    return QRect();
}

QRect Earse::drawInternal(QPainterPath *path) {
    stroker.setWidth(width);
    *path=path->united(earserPath);
    return QRect();
}

void Earse::draw(QPainter *painter, DrawPathParameter drawPathObj) {

    for (auto item:*drawPathObj.paintPath) {
        if (drawPathObj.paintPathType->value(item) == 0) {
            painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
        } else {
            painter->setCompositionMode(QPainter::CompositionMode_Xor);
        }
        painter->fillPath(*item,drawPathObj.painterPathColor->value(item,Qt::green));
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
