#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QStack>
#include <QWidget>
#include <interfaces.h>
#include <bezier.h>

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

    void redo();
    void undo();
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


    QRect getChangeRect(wpoint_arraylist_node *item);

private:
    void setupPainter(QPainter &painter);
    QRect drawInternal(wpoint_arraylist_node *rev=nullptr);
private:
    int penWidth{1};
    QColor color{Qt::red};
    int alpha {1};

    BrushInterface *brushInterface{nullptr};
    QString brushName;
    QPixmap *back{nullptr};
//    QVector<QPainterPath >paintPath;
    QStack<wpoint_arraylist_node *> stackUndo;
    int drawPathIndex{0};
    int drawTmpPathIndex{0};

    //line data
    wpoint_arraylist *m_arr_list{nullptr};
    wpoints_array *m_cur_path{nullptr};

};

#endif // PAINTWIDGET_H
