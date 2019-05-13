#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "paintwidget.h"

#include <QDir>
#include <QMainWindow>
#include <QStringList>
#include <QUndoGroup>

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QMenu;
class QScrollArea;
QT_END_NAMESPACE
class PaintArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void brushColor();
    void brushWidth();

private:
    void createActions();
    void createMenus();
    void init_ui();
    void init_conn();

    PaintWidget *paintWidget;

    QMenu *fileMenu;
    QMenu *brushMenu;
    QActionGroup *brushActionGroup;
    QAction *actPen;
    QAction *actEarser;
    QAction *exitAct;
    QAction *brushWidthAct;
    QAction *brushColorAct;
    QUndoGroup *mUndoGroup;
    QAction *actUndo;
    QAction *actRedo;
    QAction *actSave;
};

#endif
