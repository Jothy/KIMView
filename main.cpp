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
    a.setOrganizationName("ImageX");
    a.setOrganizationDomain("https://image-x.sydney.edu.au/");
    a.setApplicationName("KIMView");

    return a.exec();
}
