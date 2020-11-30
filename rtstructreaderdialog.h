/**************************************************************************
(c)Jothybasu Selvaraj
This program comes with no gaurantee of its accuracy.
Clinical decisions should not be made using this program
**************************************************************************/

#ifndef RTSTRUCTREADERDIALOG_H
#define RTSTRUCTREADERDIALOG_H

#include <QDialog>

namespace Ui {
    class RTStructReaderDialog;
}

class RTStructReaderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RTStructReaderDialog(QWidget *parent = 0);
    ~RTStructReaderDialog();
    int NumberOfROIs;
    double ROIColors[50][3]; //No. of ROIs,RGB- maximum 50 ROIs supported for now
    QList<QString> ROINames;
    QList<int>selectedItems;
    QString structFileName;
    void getSelectedItems();


private:
    Ui::RTStructReaderDialog *ui;

private slots:
    void on_pushButtonRead_clicked();
    void on_pushButtonChooseFile_clicked();
};

#endif // RTSTRUCTREADERDIALOG_H
