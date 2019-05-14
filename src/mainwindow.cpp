#include "interfaces.h"
#include "mainwindow.h"
#include "pen_pencil.h"
#include "Eraser.h"

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
    QColor newColor = QColorDialog::getColor(paintWidget->brushColor());
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

    actAlpha=new QAction (tr("set Alpha"),this);
    connect(actAlpha,&QAction::triggered,this,&MainWindow::brushAlpha);


    brushActionGroup = new QActionGroup(this);

    actPen=new QAction(tr("Pen"),this);
    brushActionGroup->addAction(actPen);

    actEarser=new QAction(tr("E"),this);
    brushActionGroup->addAction(actEarser);


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
    brushMenu->addAction(actAlpha);
    brushMenu->addSeparator();
    brushMenu->addAction(actPen);
    brushMenu->addAction(actEarser);


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
    connect(actEarser,&QAction::triggered,[this](){
        this->paintWidget->setBrush(new Eraser(this),"E");

    });
}

void MainWindow::brushAlpha() {
    float alpha=paintWidget->brushAlpha();
    bool ok;
    const int newAlpha=QInputDialog::getInt(this,tr("Alpha"),tr("Select Alpha"),alpha,1,255,1,&ok);
    if(ok){
        paintWidget->setBrushAlpha(newAlpha);
    }

}
