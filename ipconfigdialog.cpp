#include "ipconfigdialog.h"
#include "ui_ipconfigdialog.h"

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
