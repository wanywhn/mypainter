#include <QtWidgets>

#include <qmath.h>
#include <stdlib.h>
#include <math/pen_pencil.h>

#include "basictoolsplugin.h"

PencilStyle pencil(1,5);
QStringList BasicToolsPlugin::brushes() const {
    return QStringList() << tr("Pencil") << tr("Air Brush")
                         << tr("Random Letters");
}

QRect BasicToolsPlugin::mousePress(const QString &brush, QPainter &painter,
                                   const QPoint &pos) {
    pencil.insert_first()

}
QRect BasicToolsPlugin::mouseMove(const QString &brush, QPainter &painter, const QPoint &newPos) {
    return QRect();
}

QRect BasicToolsPlugin::mouseRelease(const QString & /* brush */,
                                     QPainter & /* painter */,
                                     const QPoint & /* pos */) {
    return QRect(0, 0, 0, 0);
}

QStringList BasicToolsPlugin::filters() const {
    return QStringList() << tr("Invert Pixels") << tr("Swap RGB")
                         << tr("Grayscale");
}

QImage BasicToolsPlugin::filterImage(const QString &filter, const QImage &image,
                                     QWidget * /* parent */) {
    QImage result = image.convertToFormat(QImage::Format_RGB32);

    if (filter == tr("Invert Pixels")) {
        result.invertPixels();
    } else if (filter == tr("Swap RGB")) {
        result = result.rgbSwapped();
    } else if (filter == tr("Grayscale")) {
        for (int y = 0; y < result.height(); ++y) {
            for (int x = 0; x < result.width(); ++x) {
                int pixel = result.pixel(x, y);
                int gray = qGray(pixel);
                int alpha = qAlpha(pixel);
                result.setPixel(x, y, qRgba(gray, gray, gray, alpha));
            }
        }
    }
    return result;
}

QRect BasicToolsPlugin::drawInternal(QPainterPath &path) {
    return QRect();
}

