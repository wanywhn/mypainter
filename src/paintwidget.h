#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H


#include <QMap>
#include <QStack>
#include <QUndoStack>
#include <QWidget>
#include <interfaces.h>

class UndoCommand;
class BrushInterface;
class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = nullptr);

signals:

public slots:
    void setBrushColor(const QColor &color);
    void setBrushAlpha(int alpha);
    void setBrushWidth(int width);
    void setBrush(BrushInterface *i,const QString &name);
    void saveFile();

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


    void pushUndoStack(UndoCommand *comm);
    QImage *getImage();
    void setImage(QImage value);

    QUndoStack *getUndoStack() const;

private:
    void setupPainter(QPainter &painter);
private:
    int penWidth{1};
    QColor color{Qt::red};
    int alpha {1};

    BrushInterface *brushInterface{nullptr};
    QString brushName;
    QImage *image;

    QUndoStack *undoStack;

    QPainter *mPainter;
};

#endif // PAINTWIDGET_H
