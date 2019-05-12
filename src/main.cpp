#include "mainwindow.h"

#include <stdio.h>
#include <iostream>
#include <QApplication>
#include <interfaces.h>





int main(int argc,char **argv){
    QApplication app(argc,argv);

    MainWindow w;
    w.resize(800,600);
    w.show();
    return QApplication::exec();

}


wpoint_arraylist **BrushInterface::arr_list() const
{
return m_arr_list;
}

void BrushInterface::setArr_list(wpoint_arraylist **arr_list)
{
m_arr_list = arr_list;
}

wpoints_array **BrushInterface::cur_path() const
{
return m_cur_path;
}
int BrushInterface::w_max() const
{
return m_w_max;
}

void BrushInterface::setW_max(int w_max)
{
m_w_max = w_max;
}

void BrushInterface::setW_min(int w_min)
{
m_w_min = w_min;
}

void BrushInterface::setCur_path(wpoints_array **cur_path)
{
m_cur_path = cur_path;
}
