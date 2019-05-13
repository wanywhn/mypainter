#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QMap>
#include <QWidget>
#include <interfaces.h>

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = nullptr);
    void setBrushColor(const QColor &color);
    void setBrushAlpha(int alpha);
    void setBrushWidth(int width);
    void setBrush(BrushInterface *i,const QString &name);

signals:

public slots:

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

    // QPaintDevice interface
public:
    int brushWidth();

    const QColor &brushColor();


private:
    void setupPainter(QPainter &painter);
private:
    int penWidth{1};
    QColor color{Qt::red};
    int alpha {1};

    BrushInterface *brushInterface{nullptr};
    QString brushName;
    QPixmap *back{nullptr};
    QVector<QPainterPath >paintPath;
    QMap<int,QColor> paintPathColor;
    QMap<int,int> paintPathType;
    QStack<CommandInterface *> commandUndo;
    QStack<CommandInterface *> commandRedo;

    QPainterPath oneDraw;
    int drawPathIndex{0};
    int drawTmpPathIndex{0};

};

#endif // PAINTWIDGET_H
