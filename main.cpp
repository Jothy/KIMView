#include "mainwindow.h"

#include<QApplication>
#include<QStyleFactory>

//Testing


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
    a.setStyle(QStyleFactory::create("Fusion"));


    return a.exec();
}
