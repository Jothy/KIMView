/********************************************************************************
MIT License

Copyright (c) 2021 Jothy Selvaraj

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
********************************************************************************/

#include "ipconfigdialog.h"

#include <QDebug>
#include <QSettings>

#include "ui_ipconfigdialog.h"

IPConfigDialog::IPConfigDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::IPConfigDialog) {
  ui->setupUi(this);

  // Receiver port
  QSettings settings("ImageX", "KIMView");
  QString KIMViewIP = settings.value("KIMViewIP").toString();
  int KIMViewPort = settings.value("KIMViewPort").toInt();
  // int KIMPort=settings.value("KIMPort").toInt();

  this->ui->lineEdit_KIMViewIP->setText(KIMViewIP);
  // this->ui->lineEdit_KIMPort->setText(QString::number(KIMPort));
  this->ui->lineEdit_KIMViewPort->setText(QString::number(KIMViewPort));
}

IPConfigDialog::~IPConfigDialog() { delete ui; }

void IPConfigDialog::on_buttonBox_accepted() {
  //    qDebug()<<this->ui->lineEdit_KIMViewIP->text();
  //    qDebug()<<this->ui->lineEdit_KIMPort->text().toInt();
  //    qDebug()<<this->ui->lineEdit_KIMViewPort->text().toInt();

  QSettings settings("ImageX", "KIMView");
  settings.setValue("KIMViewIP", this->ui->lineEdit_KIMViewIP->text());
  // settings.setValue("KIMPort",this->ui->lineEdit_KIMPort->text().toInt());
  settings.setValue("KIMViewPort",
                    this->ui->lineEdit_KIMViewPort->text().toInt());
}
