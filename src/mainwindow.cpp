#include "interfaces.h"
#include "mainwindow.h"
#include "pen_pencil.h"

#include <QTimer>

#include <QScrollArea>
#include <QMessageBox>
#include <QActionGroup>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QColorDialog>
#include <QInputDialog>
#include <QApplication>

MainWindow::MainWindow()
{
    paintWidget=new PaintWidget(this);
    mUndoGroup=new QUndoGroup(this);
    mUndoGroup->addStack(paintWidget->getUndoStack());
    mUndoGroup->setActiveStack(paintWidget->getUndoStack());
    init_ui();
    init_conn();
    this->setCentralWidget(paintWidget);


    setWindowTitle(tr("Plug & Paint"));

    if (!brushActionGroup->actions().isEmpty())
        brushActionGroup->actions().first()->trigger();

}

void MainWindow::brushColor()
{
    const QColor newColor = QColorDialog::getColor(paintWidget->brushColor());
    if (newColor.isValid())
        paintWidget->setBrushColor(newColor);
}

void MainWindow::brushWidth()
{
    bool ok;
    const int newWidth = QInputDialog::getInt(this, tr("Plug & Paint"),
                                              tr("Select brush width:"),
                                              paintWidget->brushWidth(),
                                              1, 50, 1, &ok);
    if (ok)
        paintWidget->setBrushWidth(newWidth);
}


void MainWindow::createActions()
{
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    brushColorAct = new QAction(tr("&Brush Color..."), this);
    connect(brushColorAct, SIGNAL(triggered()), this, SLOT(brushColor()));

    brushWidthAct = new QAction(tr("&Brush Width..."), this);
    connect(brushWidthAct, SIGNAL(triggered()), this, SLOT(brushWidth()));

    brushActionGroup = new QActionGroup(this);
    actPen=new QAction(tr("Pen"),this);
    brushActionGroup->addAction(actPen);


    actRedo=mUndoGroup->createRedoAction(this,tr("Redo"));
    actUndo=mUndoGroup->createUndoAction(this,tr("Redo"));

    actSave=new QAction(tr("Save"),this);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(actRedo);
    fileMenu->addAction(actUndo);
    fileMenu->addAction(actSave);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    brushMenu = menuBar()->addMenu(tr("&Brush"));
    brushMenu->addAction(brushColorAct);
    brushMenu->addAction(brushWidthAct);
    brushMenu->addSeparator();

    shapesMenu = menuBar()->addMenu(tr("&Shapes"));

    filterMenu = menuBar()->addMenu(tr("&Filter"));

    menuBar()->addSeparator();

}

void MainWindow::init_ui()
{
    createActions();
    createMenus();

}

void MainWindow::init_conn()
{
    connect(actSave,&QAction::triggered,paintWidget,&PaintWidget::saveFile);
    connect(actPen,&QAction::triggered,[this](){
        this->paintWidget->setBrush(new PencilStyle(this),"pen");

    });
}
