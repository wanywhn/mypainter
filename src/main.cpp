#include <stdio.h>
#include <iostream>
#include <QApplication>


#include "mainwidnow.h"



int main(int argc,char **argv){
    QApplication app(argc,argv);

    MainWidnow w;
    w.resize(800,600);
    w.show();
    return QApplication::exec();

}
