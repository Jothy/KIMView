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
#include "selecttargetdialog.h"

#include <QDebug>

#include "ui_selecttargetdialog.h"

SelectTargetDialog::SelectTargetDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::SelectTargetDialog) {
  ui->setupUi(this);
  this->parent = parent;
}

SelectTargetDialog::~SelectTargetDialog() { delete ui; }

void SelectTargetDialog::setROINames() {
  for (int i = 0; i < this->ROINames.size(); i++) {
    // qDebug() << this->ROINames[i];
    this->ui->comboBox->addItem(this->ROINames[i]);
    this->ui->comboBox->setItemData(i,
                                    QColor(this->ROIColors[i][0],
                                           this->ROIColors[i][1],
                                           this->ROIColors[i][2]),
                                    Qt::BackgroundRole);
  }
}

void SelectTargetDialog::on_buttonBox_accepted() {
  this->selectedROINum = this->ui->comboBox->currentIndex();
  this->ROISelected = true;
  if (this->ROISelected) {
    qDebug() << "Selected item: " << this->selectedROINum;
  }
}
