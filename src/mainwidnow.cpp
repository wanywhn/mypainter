#include "mainwidnow.h"

MainWidnow::MainWidnow(QWidget *parent) : QMainWindow(parent)
{
    this->centerWidget=new PaintWidget(this);
//  this->setAttribute(Qt::WA_PaintOnScreen);
    init_ui();
    init_conn();

}

void MainWidnow::init_ui()
{
    this->setCentralWidget(this->centerWidget);

}

void MainWidnow::init_conn()
{

}


