/******************************************************************************
MIT License

Copyright (c) 2021 Jothy Selvaraj

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
                                                              copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

       THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
            ******************************************************************************/
#include "wlwwdialog.h"

#include <vtkImageProperty.h>
#include <vtkRenderWindow.h>

#include <QDebug>
#include <QScrollBar>

#include "ui_wlwwdialog.h"

WLWWDialog::WLWWDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::WLWWDialog) {
  ui->setupUi(this);
}

WLWWDialog::~WLWWDialog() { delete ui; }

void WLWWDialog::on_pushButtonReset_clicked() {
  this->ui->ScrollBar_width->setValue(1000);
  this->ui->ScrollBar_level->setValue(100);
  this->ui->lineEditWidth->setText("1000");
  this->ui->lineEditLevel->setText("100");

  this->AxialViewer->TriggerReset_WL_WW();
  this->SagittalViewer->TriggerReset_WL_WW();
  this->CoronalViewer->TriggerReset_WL_WW();
}

void WLWWDialog::on_ScrollBar_level_sliderMoved(int position) {
  // qDebug() << position;
  this->ui->lineEditLevel->setText(
      QString::number(this->ui->ScrollBar_level->value()));
  this->updateWLWW();
}

void WLWWDialog::on_ScrollBar_width_sliderMoved(int position) {
  // qDebug() << position;
  this->ui->lineEditWidth->setText(
      QString::number(this->ui->ScrollBar_width->value()));
  this->updateWLWW();
}

void WLWWDialog::on_lineEditWidth_editingFinished() {
  this->ui->ScrollBar_width->setValue(this->ui->lineEditWidth->text().toInt());
  this->updateWLWW();
}

void WLWWDialog::on_lineEditLevel_editingFinished() {
  this->ui->ScrollBar_level->setValue(this->ui->lineEditLevel->text().toInt());
  this->updateWLWW();
}

void WLWWDialog::updateWLWW() {
  // Calculate windowLow adn WindowUp for WL & WW
  double winUp = (this->ui->ScrollBar_width->value() / 2.0) +
                 this->ui->ScrollBar_level->value();
  double winLow = -(this->ui->ScrollBar_width->value() / 2.0) +
                  this->ui->ScrollBar_level->value();

  this->AxialViewer->ViewRenderer->RemoveActor(this->AxialViewer->ImageSlice);
  this->AxialViewer->WindowLow = winLow;
  this->AxialViewer->WindowUp = winUp;
  this->AxialViewer->ShowImageAndDose(this->AxialViewer->SliceLoc);
  this->AxialViewer->ViewRenderer->GetRenderWindow()->Render();

  this->SagittalViewer->ViewRenderer->RemoveActor(
      this->SagittalViewer->ImageSlice);
  this->SagittalViewer->WindowLow = winLow;
  this->SagittalViewer->WindowUp = winUp;
  this->SagittalViewer->ShowImageAndDose(this->SagittalViewer->SliceLoc);
  this->SagittalViewer->ViewRenderer->GetRenderWindow()->Render();

  this->CoronalViewer->ViewRenderer->RemoveActor(
      this->CoronalViewer->ImageSlice);
  this->CoronalViewer->WindowLow = winLow;
  this->CoronalViewer->WindowUp = winUp;
  this->CoronalViewer->ShowImageAndDose(this->CoronalViewer->SliceLoc);
  this->CoronalViewer->ViewRenderer->GetRenderWindow()->Render();
}
