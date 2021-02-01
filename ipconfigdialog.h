#ifndef IPCONFIGDIALOG_H
#define IPCONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class IPConfigDialog;
}

class IPConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IPConfigDialog(QWidget *parent = nullptr);
    ~IPConfigDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::IPConfigDialog *ui;
};

#endif // IPCONFIGDIALOG_H
