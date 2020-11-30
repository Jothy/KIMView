#include "rangesliderdialog.h"
#include "ui_rangesliderdialog.h"

#include<QDebug>
#include<imageviewer2d.h>

#include<vtkLookupTable.h>

RangeSliderDialog::RangeSliderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RangeSliderDialog)
{
    ui->setupUi(this);
    //this->AxialViewer=new ImageViewer2D();


}

RangeSliderDialog::~RangeSliderDialog()
{
    delete ui;
}

void RangeSliderDialog::SetDoseRange()
{

    this->ui->horizontalSlider->setMinimum(this->minDose);
    this->ui->horizontalSlider->setMaximum(this->maxDose);

    this->ui->SpinBoxMinDose->setMaximum(this->maxDose);
    this->ui->SpinBoxMinDose->setMinimum(this->minDose);

    this->ui->SpinBoxMaxDose->setMaximum(this->maxDose);
    this->ui->SpinBoxMaxDose->setMinimum(this->minDose);
    //qDebug()<<"Range changed.";

}

void RangeSliderDialog::on_SpinBoxMinDose_editingFinished()
{
    this->minDose=this->ui->SpinBoxMinDose->value();
    this->ui->horizontalSlider->setValue(this->minDose);
    //this->AxialViewer->AdjustDoseRange(this->ui->horizontalSlider->value(),this->maxDose);


}

void RangeSliderDialog::on_SpinBoxMaxDose_editingFinished()
{
    if(this->ui->SpinBoxMaxDose->value()>this->ui->SpinBoxMinDose->value())
    {
        this->maxDose=this->ui->SpinBoxMaxDose->value();
        this->AxialViewer->AdjustDoseRange(this->ui->horizontalSlider->value(),this->maxDose);
        this->SagittalViewer->AdjustDoseRange(this->ui->horizontalSlider->value(),this->maxDose);
        this->CoronalViewer->AdjustDoseRange(this->ui->horizontalSlider->value(),this->maxDose);
    }
    else
    {
        this->ui->SpinBoxMaxDose->setValue(this->ui->SpinBoxMinDose->value());
    }


}


void RangeSliderDialog::on_horizontalSlider_valueChanged(double)
{
    this->ui->SpinBoxMinDose->setValue(this->ui->horizontalSlider->value());
    this->AxialViewer->AdjustDoseRange(this->ui->horizontalSlider->value(),this->maxDose);
    this->SagittalViewer->AdjustDoseRange(this->ui->horizontalSlider->value(),this->maxDose);
    this->CoronalViewer->AdjustDoseRange(this->ui->horizontalSlider->value(),this->maxDose);

}

//Reset dose range
void RangeSliderDialog::on_pushButton_clicked()
{
    this->minDose=this->AxialViewer->RTDose->GetScalarRange()[0];
    this->maxDose=this->AxialViewer->RTDose->GetScalarRange()[1];
    this->ui->SpinBoxMinDose->setValue(this->minDose);
    this->ui->SpinBoxMaxDose->setValue(this->maxDose);
    this->AxialViewer->AdjustDoseRange(this->minDose,this->maxDose);
    this->SagittalViewer->AdjustDoseRange(this->minDose,this->maxDose);
    this->CoronalViewer->AdjustDoseRange(this->minDose,this->maxDose);
    this->AxialViewer->UpdateView();
    this->SagittalViewer->UpdateView();
    this->CoronalViewer->UpdateView();

}
