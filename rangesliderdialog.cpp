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

#include "rangesliderdialog.h"

#include <bevwidget.h>
#include <imageviewer2d.h>
#include <vtkLookupTable.h>
#include <vtkRenderWindow.h>

#include <QDebug>

#include "ui_rangesliderdialog.h"

RangeSliderDialog::RangeSliderDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::RangeSliderDialog) {
  ui->setupUi(this);
}

RangeSliderDialog::~RangeSliderDialog() { delete ui; }

void RangeSliderDialog::SetDoseRange() {
  this->ui->horizontalSlider->setMinimum(this->minDose);
  this->ui->horizontalSlider->setMaximum(this->maxDose);

  this->ui->SpinBoxMinDose->setMaximum(this->maxDose);
  this->ui->SpinBoxMinDose->setMinimum(this->minDose);

  this->ui->SpinBoxMaxDose->setMaximum(this->maxDose);
  this->ui->SpinBoxMaxDose->setMinimum(this->minDose);
  // qDebug()<<"Range changed.";
}

void RangeSliderDialog::on_SpinBoxMinDose_editingFinished() {
  this->minDose = this->ui->SpinBoxMinDose->value();
  this->ui->horizontalSlider->setValue(this->minDose);
  // this->AxialViewer->AdjustDoseRange(this->ui->horizontalSlider->value(),this->maxDose);
  this->ModelViewer->ShowIsodoseSurface(this->minDose, this->minDose);
}

void RangeSliderDialog::on_SpinBoxMaxDose_editingFinished() {
  if (this->ui->SpinBoxMaxDose->value() > this->ui->SpinBoxMinDose->value()) {
    this->maxDose = this->ui->SpinBoxMaxDose->value();
    this->AxialViewer->AdjustDoseRange(this->ui->horizontalSlider->value(),
                                       this->maxDose);
    this->SagittalViewer->AdjustDoseRange(this->ui->horizontalSlider->value(),
                                          this->maxDose);
    this->CoronalViewer->AdjustDoseRange(this->ui->horizontalSlider->value(),
                                         this->maxDose);
    this->ModelViewer->ShowIsodoseSurface(this->minDose, this->minDose);
  } else {
    this->ui->SpinBoxMaxDose->setValue(this->ui->SpinBoxMinDose->value());
  }
}

void RangeSliderDialog::on_horizontalSlider_valueChanged(double) {
  this->ui->SpinBoxMinDose->setValue(this->ui->horizontalSlider->value());
  this->AxialViewer->AdjustDoseRange(this->ui->horizontalSlider->value(),
                                     this->maxDose);
  this->SagittalViewer->AdjustDoseRange(this->ui->horizontalSlider->value(),
                                        this->maxDose);
  this->CoronalViewer->AdjustDoseRange(this->ui->horizontalSlider->value(),
                                       this->maxDose);
  this->ModelViewer->ShowIsodoseSurface(this->minDose, this->minDose);
  this->ModelViewer->ModelRenderer->GetRenderWindow()->Render();
  this->ModelViewer->show();
}

// Reset dose range
void RangeSliderDialog::on_pushButton_clicked() {
  this->minDose = this->AxialViewer->RTDose->GetScalarRange()[0];
  this->maxDose = this->AxialViewer->RTDose->GetScalarRange()[1];
  this->ui->SpinBoxMinDose->setValue(this->minDose);
  this->ui->SpinBoxMaxDose->setValue(this->maxDose);
  this->AxialViewer->AdjustDoseRange(this->minDose, this->maxDose);
  this->SagittalViewer->AdjustDoseRange(this->minDose, this->maxDose);
  this->CoronalViewer->AdjustDoseRange(this->minDose, this->maxDose);
  this->ModelViewer->ShowIsodoseSurface(this->minDose, this->minDose);
  this->AxialViewer->UpdateView();
  this->SagittalViewer->UpdateView();
  this->CoronalViewer->UpdateView();
}
