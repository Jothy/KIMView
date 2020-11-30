#ifndef RANGESLIDERDIALOG_H
#define RANGESLIDERDIALOG_H

#include <QDialog>

#include<imageviewer2d.h>

namespace Ui {
class RangeSliderDialog;
}

class RangeSliderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RangeSliderDialog(QWidget *parent = nullptr);
    ~RangeSliderDialog();
    void SetDoseRange();
    double minDose=0;
    double maxDose=100;
    ImageViewer2D *AxialViewer;
    ImageViewer2D *CoronalViewer;
    ImageViewer2D *SagittalViewer;


private slots:
    void on_SpinBoxMinDose_editingFinished();
    void on_SpinBoxMaxDose_editingFinished();    
    void on_horizontalSlider_valueChanged(double);


    void on_pushButton_clicked();

private:
    Ui::RangeSliderDialog *ui;
};

#endif // RANGESLIDERDIALOG_H
