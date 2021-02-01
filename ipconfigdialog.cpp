#include "ipconfigdialog.h"
#include "ui_ipconfigdialog.h"

#include<QDebug>
#include<QSettings>
#include<QDebug>

IPConfigDialog::IPConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IPConfigDialog)
{
    ui->setupUi(this);
}

IPConfigDialog::~IPConfigDialog()
{
    delete ui;
}

void IPConfigDialog::on_buttonBox_accepted()
{
//    qDebug()<<this->ui->lineEdit_KIMIP->text();
//    qDebug()<<this->ui->lineEdit_KIMPort->text().toInt();
//    qDebug()<<this->ui->lineEdit_KIMViewPort->text().toInt();

    QSettings settings("ImageX","KIMView");
    settings.setValue("KIMIP",this->ui->lineEdit_KIMIP->text());
    settings.setValue("KIMPort",this->ui->lineEdit_KIMPort->text().toInt());
    settings.setValue("KIMViewPort",this->ui->lineEdit_KIMViewPort->text().toInt());
}
