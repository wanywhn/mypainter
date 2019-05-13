#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "paintwidget.h"

#include <QDir>
#include <QMainWindow>
#include <QStringList>

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
    void changeBrush();
    void applyFilter();

private:
    void createActions();
    void createMenus();
    void loadPlugins();
    void populateMenus(QObject *plugin);
    void addToMenu(QObject *plugin, const QStringList &texts, QMenu *menu,
                   const char *member, QActionGroup *actionGroup = 0);
    void init_ui();
    void init_conn();

    PaintWidget *paintWidget;
    QDir pluginsDir;
    QStringList pluginFileNames;

    QMenu *fileMenu;
    QMenu *brushMenu;
    QMenu *shapesMenu;
    QMenu *filterMenu;
    QActionGroup *brushActionGroup;
    QAction *exitAct;
    QAction *brushWidthAct;
    QAction *brushColorAct;
    QAction *actUndo;
    QAction *actRedo;
    QAction *actSave;
};

#endif
