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

#ifndef RANGESLIDERDIALOG_H
#define RANGESLIDERDIALOG_H

#include <bevwidget.h>
#include <imageviewer2d.h>

#include <QDialog>

namespace Ui {
class RangeSliderDialog;
}

class RangeSliderDialog : public QDialog {
  Q_OBJECT

 public:
  explicit RangeSliderDialog(QWidget *parent = nullptr);
  ~RangeSliderDialog();
  void SetDoseRange();
  double minDose = 0;
  double maxDose = 100;
  ImageViewer2D *AxialViewer;
  ImageViewer2D *CoronalViewer;
  ImageViewer2D *SagittalViewer;
  BEVWidget *ModelViewer;

 private slots:
  void on_SpinBoxMinDose_editingFinished();
  void on_SpinBoxMaxDose_editingFinished();
  void on_horizontalSlider_valueChanged(double value);

  void on_pushButton_clicked();

 private:
  Ui::RangeSliderDialog *ui;
};

#endif  // RANGESLIDERDIALOG_H
