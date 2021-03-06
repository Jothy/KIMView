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

#include "doseprofiledialog.h"

#include <vtkAxis.h>
#include <vtkChart.h>
#include <vtkChartXY.h>
#include <vtkCommand.h>
#include <vtkContextScene.h>
#include <vtkContextView.h>
#include <vtkDataObjectToTable.h>
#include <vtkFloatArray.h>
#include <vtkHandleRepresentation.h>
#include <vtkHandleWidget.h>
#include <vtkLineRepresentation.h>
#include <vtkPNGWriter.h>
#include <vtkPen.h>
#include <vtkPlot.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkProperty2D.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTable.h>
#include <vtkWindowToImageFilter.h>

#include <QAction>
#include <QChartView>
#include <QCloseEvent>
#include <QDebug>
#include <QFileDialog>
#include <QLineSeries>
#include <QObjectList>
#include <QSplineSeries>
#include <QValueAxis>
#include <QtGui>

#include "ui_doseprofiledialog.h"
#include "vtklinecallbackdose.h"

QT_CHARTS_USE_NAMESPACE

DoseProfileDialog::DoseProfileDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::DoseProfileDialog) {
  ui->setupUi(this);
  Qt::WindowFlags flags = nullptr;
  flags = Qt::Dialog;
  flags |= Qt::WindowMaximizeButtonHint;
  flags |= Qt::WindowMinimizeButtonHint;
  // flags |= Qt::WindowCloseButtonHint;
  this->setWindowFlags(flags);

  this->DoseData = new QSplineSeries;

  this->addAction(this->ui->actionSave_Figure);
  this->show();
}

DoseProfileDialog::~DoseProfileDialog() {
  delete ui;
  delete this->DoseData;
}

void DoseProfileDialog::plotProfile() {
  QChart *chart = new QChart();
  chart->legend()->show();

  // Enable anti-aliasing
  this->ui->ChartView->setRenderHint(QPainter::Antialiasing, true);

  chart->addSeries(this->DoseData);
  this->ui->ChartView->setChart(chart);

  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(10);
  axisX->setLabelFormat("%.0f");
  axisX->setTitleText("Distance (mm)");
  this->ui->ChartView->chart()->setAxisX(axisX, this->DoseData);

  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(10);
  axisY->setLabelFormat("%.1f");
  axisY->setTitleText("Dose (Gy)");
  this->ui->ChartView->chart()->setAxisY(axisY, this->DoseData);
}

void DoseProfileDialog::on_actionSave_Figure_triggered() {
  QString fileName = QFileDialog::getSaveFileName(this, "Save DVH Figure",
                                                  "Dose profile", "png(*.png)");
  this->ui->ChartView->grab().save(fileName.append(".png"));
}

void DoseProfileDialog::closeEvent(QCloseEvent *event) {}
