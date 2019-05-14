#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "paintwidget.h"

#include <QDir>
#include <QMainWindow>
#include <QStringList>
#include <QUndoGroup>
#include <QWheelEvent>

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QMenu;
class QScrollArea;
QT_END_NAMESPACE

class myScrollArea;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void brushColor();
    void brushWidth();
    void brushAlpha();

private:
    void createActions();
    void createMenus();
    void init_ui();
    void init_conn();

protected:
    void wheelEvent(QWheelEvent *event) override;

private:

    PaintWidget *paintWidget;
    myScrollArea *scrollArea;

    QMenu *fileMenu;
    QMenu *brushMenu;
    QActionGroup *brushActionGroup;
    QAction *actPen;
    QAction *actEarser;
    QAction *exitAct;
    QAction *brushWidthAct;
    QAction *brushColorAct;
    QAction *actAlpha;
    QUndoGroup *mUndoGroup;
    QAction *actUndo;
    QAction *actRedo;
    QAction *actSave;
};

#endif
