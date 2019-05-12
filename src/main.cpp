#include "mainwindow.h"

#include <stdio.h>
#include <iostream>
#include <QApplication>





int main(int argc,char **argv){
    QApplication app(argc,argv);

    MainWindow w;
    w.resize(800,600);
    w.show();
    return QApplication::exec();

}
