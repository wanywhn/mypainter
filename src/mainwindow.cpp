

#include "interfaces.h"
#include "mainwindow.h"

#include <QPluginLoader>
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
    init_ui();
    init_conn();
    this->setCentralWidget(paintWidget);

    loadPlugins();

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

//! [0]
void MainWindow::changeBrush()
{
    auto *action = qobject_cast<QAction *>(sender());
    BrushInterface *iBrush = qobject_cast<BrushInterface *>(action->parent());
    const QString brush = action->text();

    paintWidget->setBrush(iBrush, brush);
}
//! [0]

void MainWindow::applyFilter()
{
    QAction *action = qobject_cast<QAction *>(sender());
    FilterInterface *iFilter =
            qobject_cast<FilterInterface *>(action->parent());

//    const QImage image = iFilter->filterImage(action->text(), paintArea->image(),
//                                              this);
//    paintArea->setImage(image);
}
//! [2]


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

    actRedo=new QAction(tr("Redo"),this);
    connect(actRedo,&QAction::triggered,paintWidget,&PaintWidget::redo);
    actRedo->setEnabled(false);

    actUndo=new QAction(tr("Undo"),this);
    connect(actUndo,&QAction::triggered,paintWidget,&PaintWidget::undo);
    actUndo->setEnabled(false);

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

//! [4]
void MainWindow::loadPlugins()
{
    foreach (QObject *plugin, QPluginLoader::staticInstances())
        populateMenus(plugin);
//! [4] //! [5]

    pluginsDir = QDir(qApp->applicationDirPath());

    pluginsDir.cd("../plugins");
//! [5]

//! [6]
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin) {
            populateMenus(plugin);
            pluginFileNames += fileName;
//! [6] //! [7]
        }
//! [7] //! [8]
    }
//! [8]

//! [9]
    brushMenu->setEnabled(!brushActionGroup->actions().isEmpty());
    shapesMenu->setEnabled(!shapesMenu->actions().isEmpty());
    filterMenu->setEnabled(!filterMenu->actions().isEmpty());
}
//! [9]

//! [10]
void MainWindow::populateMenus(QObject *plugin)
{
    BrushInterface *iBrush = qobject_cast<BrushInterface *>(plugin);
    if (iBrush)
        addToMenu(plugin, iBrush->brushes(), brushMenu, SLOT(changeBrush()),
                  brushActionGroup);

    FilterInterface *iFilter = qobject_cast<FilterInterface *>(plugin);
    if (iFilter)
        addToMenu(plugin, iFilter->filters(), filterMenu, SLOT(applyFilter()));
}
//! [10]

void MainWindow::addToMenu(QObject *plugin, const QStringList &texts,
                           QMenu *menu, const char *member,
                           QActionGroup *actionGroup)
{
    foreach (QString text, texts) {
        QAction *action = new QAction(text, plugin);
        connect(action, SIGNAL(triggered()), this, member);
        menu->addAction(action);

        if (actionGroup) {
            action->setCheckable(true);
            actionGroup->addAction(action);
        }
    }
}

void MainWindow::init_ui()
{
    createActions();
    createMenus();

}

void MainWindow::init_conn()
{
    connect(paintWidget,&PaintWidget::redoEmpty,this->actRedo,&QAction::setDisabled);
    connect(paintWidget,&PaintWidget::undoEmpty,this->actUndo,&QAction::setDisabled);
    connect(actSave,&QAction::triggered,paintWidget,&PaintWidget::saveFile);
}
