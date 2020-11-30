/**************************************************************************
(c)Jothybasu Selvaraj
This program comes with no gaurantee of its accuracy.
Clinical decisions should not be made using this program
**************************************************************************/

#ifndef DOSEPROFILEDIALOG_H
#define DOSEPROFILEDIALOG_H

#include <QDialog>

#include <QChartView>
#include <QSplineSeries>
#include <QLineSeries>
#include<QCloseEvent>

QT_CHARTS_USE_NAMESPACE


namespace Ui {
    class DoseProfileDialog;
}

class DoseProfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DoseProfileDialog(QWidget *parent = 0);
    ~DoseProfileDialog();
    void plotProfile();
    QSplineSeries *DoseData;
    void closeEvent(QCloseEvent *event);



private:
    Ui::DoseProfileDialog *ui;

private slots:    
    void on_actionSave_Figure_triggered();


};

#endif // DOSEPROFILEDIALOG_H
