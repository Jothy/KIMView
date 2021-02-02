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

    //Receiver port
    QSettings settings("ImageX","KIMView");
    QString KIMIP=settings.value("KIMIP").toString();
    int KIMViewPort=settings.value("KIMViewPort").toInt();
    int KIMPort=settings.value("KIMPort").toInt();

    this->ui->lineEdit_KIMIP->setText(KIMIP);
    this->ui->lineEdit_KIMPort->setText(QString::number(KIMPort));
    this->ui->lineEdit_KIMViewPort->setText(QString::number(KIMViewPort));



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
