#include "ipconfigdialog.h"
#include "ui_ipconfigdialog.h"

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
    qDebug()<<"Accepted";
}
