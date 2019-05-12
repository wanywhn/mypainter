#ifndef MAINWIDNOW_H
#define MAINWIDNOW_H

#include "paintwidget.h"

#include <QMainWindow>

class MainWidnow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWidnow(QWidget *parent = nullptr);

signals:

public slots:
private:
    void init_ui();
    void init_conn();

private:
    PaintWidget *centerWidget{nullptr};

};

#endif // MAINWIDNOW_H
