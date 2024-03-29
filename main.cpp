/********************************************************************************
 MIT License

 Copyright (c) 2021 Jothy Selvaraj

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 ********************************************************************************/
#include <QApplication>
#include <QButtonGroup>
#include <QDebug>
#include <QStyleFactory>

#include "aboutdialog.h"
#include "mainwindow.h"

// Testing

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;

  AboutDialog *license = new AboutDialog();
  license->exec();

  if (license->acceptLicense == true) {
    // qDebug() << "Accepted";
    w.showMaximized();
    a.setStyle(QStyleFactory::create("Fusion"));
    a.setOrganizationName("ImageX");
    a.setOrganizationDomain("https://image-x.sydney.edu.au/");
    a.setApplicationName("KIMView");
    return a.exec();

  }

  else {
    // qDebug() << "Declined";
  }

  //  // Default style
  //  QFile file(":/Styles/ElegantDark.qss");
  //  file.open(QFile::ReadOnly);
  //  QString styleSheet = QLatin1String(file.readAll());
  //  a.setStyleSheet(styleSheet);
}
