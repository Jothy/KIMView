/**************************************************************************
(c)Jothybasu Selvaraj
This program comes with no gaurantee of its accuracy.
Clinical decisions should not be made using this program
**************************************************************************/

#include "doseprofiledialog.h"
#include "ui_doseprofiledialog.h"

#include<vtkSmartPointer.h>
#include<vtkContextView.h>
#include<vtkChartXY.h>
#include<vtkTable.h>
#include<vtkFloatArray.h>
#include<vtkContextScene.h>
#include<vtkRenderer.h>
#include<vtkRenderWindowInteractor.h>
#include<vtkRenderWindow.h>
#include<vtkPen.h>
#include<vtkAxis.h>
#include<vtkProperty2D.h>
#include<vtkPlot.h>
#include<vtkDataObjectToTable.h>
#include<vtkLineRepresentation.h>
#include<vtkPolyData.h>
#include<vtkHandleRepresentation.h>
#include<vtkHandleWidget.h>
#include<vtkChart.h>

#include"vtklinecallbackdose.h"
#include <vtkCommand.h>
#include<vtkProperty2D.h>
#include<vtkProperty.h>
#include<vtkWindowToImageFilter.h>
#include<vtkPNGWriter.h>

#include<QFileDialog>
#include<QDebug>
#include<QObjectList>
#include<QAction>
#include <QtGui>
#include <QChartView>
#include <QSplineSeries>
#include <QLineSeries>
#include <QValueAxis>
#include<QCloseEvent>

QT_CHARTS_USE_NAMESPACE



DoseProfileDialog::DoseProfileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DoseProfileDialog)
{
    ui->setupUi(this);
    Qt::WindowFlags flags = nullptr;
    flags = Qt::Dialog;
    flags |= Qt::WindowMaximizeButtonHint;
    flags |= Qt::WindowMinimizeButtonHint;
    //flags |= Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    this->DoseData=new  QSplineSeries;

    this->addAction(this->ui->actionSave_Figure);
    this->show();

}


DoseProfileDialog::~DoseProfileDialog()
{
    delete ui;
    delete this->DoseData;

}

void DoseProfileDialog::plotProfile()
{
    QChart *chart = new QChart();
    chart->legend()->show();
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

void DoseProfileDialog::on_actionSave_Figure_triggered()
{

    QString fileName=QFileDialog::getSaveFileName(this,"Save DVH Figure","Dose profile", "png(*.png)");
    this->ui->ChartView->grab().save(fileName.append(".png"));


}


void DoseProfileDialog::closeEvent(QCloseEvent *event)
{




}
























