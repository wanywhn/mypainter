#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H


#include <QMap>
#include <QStack>
#include <QUndoStack>
#include <QWidget>
#include <interfaces.h>
#include <QtWidgets/QScrollArea>

class UndoCommand;
class BrushInterface;
class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = nullptr);


    void zoom(float factor, QPoint centerPos);

public slots:
    void setBrushColor(QColor &color);
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
    float brushAlpha();


    void pushUndoStack(UndoCommand *comm);
    QImage *getImage();
    void setImage(QImage value);

    QUndoStack *getUndoStack() const;

private:
private:

    BrushInterface *brushInterface{nullptr};
    QString brushName;
    QImage *image;
    QScrollArea *scrollArea;
    int alpha{255};

    QUndoStack *undoStack;

};

#endif // PAINTWIDGET_H
