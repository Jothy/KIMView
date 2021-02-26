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


#include "dvhdialog.h"
#include "ui_dvhdialog.h"
#include"dvhcalc.h"


#include<QDebug>
#include<QString>
#include<QTableWidget>
#include<QTableWidgetItem>
#include <QHeaderView>
#include<QColor>
#include<QBrush>
#include<QTime>
#include<QMessageBox>
#include<QFileDialog>
#include<QProgressDialog>
#include<QLineSeries>
#include<QValueAxis>

#include<vtkPolyData.h>
#include<vtkImageData.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkChartXY.h>
#include <vtkPlot.h>
#include <vtkTable.h>
#include <vtkFloatArray.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include<vtkAxis.h>
#include<vtkPen.h>
#include <vtkTable.h>
#include <vtkFloatArray.h>
#include<vtkProperty.h>
#include<vtkStdString.h>
#include<vtkPicker.h>
#include<vtkBrush.h>
#include<vtkWindowToImageFilter.h>
#include<vtkPNGWriter.h>
#include<vtkAbstractArray.h>
#include<vtkProperty2D.h>
#include<vtkXYPlotActor.h>
#include<vtkChartLegend.h>


#include <algorithm>
#include <numeric>
#include <vector>
#include <iterator>

QT_CHARTS_USE_NAMESPACE


DVHDialog::DVHDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DVHDialog)
{
    ui->setupUi(this);
    Qt::WindowFlags flags = nullptr;
    flags = Qt::Dialog;
    flags |= Qt::WindowMaximizeButtonHint;
    flags |= Qt::WindowMinimizeButtonHint;
    flags |= Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    this->ui->dvhTable->setAlternatingRowColors(true);
    this->ui->dvhTable->setColumnWidth(0,45);//Shrink select column width


    this->maxDose=70;//Set to 70 Gy
    this->bins=100;//Default value for No. of bins
    this->resampleDoseFlag=0;//Resample dose off by default
    this->interpolationTypeFlag=0;//Linear by default
    this->dvhTypeFlag=0;//Cumulative by default

    this->xMeanRan=0.0;this->yMeanRan=0.0;this->zMeanRan=0.0;
    this->xMeanSys=0.0;this->yMeanSys=0.0;this->zMeanSys=0.0;
    this->xSDRan=3.0;this->ySDRan=3.0;this->zSDRan=3.0;
    this->xSDSys=3.0;this->ySDSys=3.0;this->zSDSys=3.0;

    this->alpha=0.262;this->alphaMean=0.262;this->alphaByBeta=10.0;this->alphaStd=0.045;
    this->clonogenDensity=1.0E7;
    this->selectedStructureNum=0;//Default is 1st structure






}

DVHDialog::~DVHDialog()
{        
    this->sitems.clear();
    this->structNames.clear();
    this->ROIType.clear();
    this->refROINums.clear();
    this->refNoOfSelectedROIs.clear();
    this->ROINo.clear();
    this->meshes.clear();
    this->dvhBinsVector.clear();
    this->dvhTable.clear();
    delete ui;


}

void DVHDialog::setStructureNames(QList<QString> structNames)
{
    const int itemNos=structNames.count();//No. of items in QList
    this->ui->dvhTable->setRowCount( itemNos);
    for (int n=0;n<itemNos;n++)
    {
        this->ui->dvhTable->verticalHeaderItem(n)->setText(structNames[n]);
        QBrush brush(QColor(this->ROIColors[n][0],this->ROIColors[n][1],this->ROIColors[n][2], 255));
        brush.setStyle(Qt::SolidPattern);
        this->ui->dvhTable->item(n,0)->setBackground(brush);
    }
    this->structNames=structNames;
}

void DVHDialog::setROIColors(double ROIColors[][3])
{
    for (int x=0;x<50;x++)
    {
        this->ROIColors[x][0]=ROIColors[x][0];
        this->ROIColors[x][1]=ROIColors[x][1];
        this->ROIColors[x][2]=ROIColors[x][2];

    }

}

void DVHDialog::getSelectedItems()
{
    this->sitems.clear();//clean up at start
    int rows=this->ui->dvhTable->rowCount();
    for (int x=0;x<rows;x++)
    {
        int state=this->ui->dvhTable->item(x,0)->checkState();
        if(state==2)
        {
            this->sitems.append(x);
            //qDebug()<<x<<"selected item";
            this->refNoOfSelectedROIs.append(this->ROINo[x]);
            //qDebug()<<this->ROINo[x]<<"Ref ROI No";
        }
    }

}

void DVHDialog::on_pushButton_clicked()
{
    //Get DVH type to be calculated and displayed
    if(this->ui->radioButtonCum->isChecked())
    {
        this->dvhTypeFlag=0;

    }

    else if(this->ui->radioButtonDiff->isChecked())
    {
        this->dvhTypeFlag=1;

    }


        //Instantiate QChart and hide legend by default so the chart is bigger
        QChart *DVHChart = new QChart();
        DVHChart->legend()->hide();

        QValueAxis *axisX = new QValueAxis;
        axisX->setTickCount(10);
        axisX->setLabelFormat("%.0f");
        axisX->setTitleText("Dose (Gy)");

        QValueAxis *axisY = new QValueAxis;
        axisY->setTickCount(10);
        axisY->setLabelFormat("%.1f");
        axisY->setTitleText("Volume (%)");


        if(this->resampleDoseFlag!=1)
        {
            //this->bins=this->ui->binSpinBox->value();
            this->getSelectedItems();

            QProgressDialog *pd=new QProgressDialog("Calculating DVH...","Abort",0, 100,this);
            pd->setModal(true);
            pd->setRange(0,100);
            pd->setAutoClose(true);
            pd->setWindowTitle("Calculating DVH...");
            pd->show();
            QCoreApplication::processEvents();

            //Calculate DVH for each selected item
            for (int x=0;x<this->sitems.size();x++)
            {
                pd->setLabelText(this->structNames[x]);

                //qDebug()<<timer->currentTime();
                DVHCalc *myDVHCalc= new DVHCalc;//Instantiate inside the loop, otherwise it will create memory leaks
                int i=this->sitems[x];
                std::vector<double>results;


                if(this->ROIType[i]!="N/A")//Calculate only if structure really exist
                {
                     QLineSeries *CurROIDVH = new QLineSeries();
                    //Normal DVH calculation
                    results=myDVHCalc->calcStructDVH(this->meshes[i],this->doseMatrix);

                    myDVHCalc->indicesX.clear();
                    myDVHCalc->indicesY.clear();
                    myDVHCalc->indicesZ.clear();

                    this->ui->dvhTable->item(i,1)->setText(QString::number(results[0],'f',2));//volume
                    this->ui->dvhTable->item(i,2)->setText(QString::number(results[1],'f',2));//max dose
                    this->ui->dvhTable->item(i,3)->setText(QString::number(results[2],'f',2));//mean dose
                    this->ui->dvhTable->item(i,4)->setText(QString::number(results[3],'f',2));//min dose
                    this->ui->dvhTable->item(i,5)->setText(QString::number(results[4],'f',2));//std

                    this->ui->dvhTable->item(i,1)->setTextAlignment(Qt::AlignCenter);
                    this->ui->dvhTable->item(i,2)->setTextAlignment(Qt::AlignCenter);
                    this->ui->dvhTable->item(i,3)->setTextAlignment(Qt::AlignCenter);
                    this->ui->dvhTable->item(i,4)->setTextAlignment(Qt::AlignCenter);
                    this->ui->dvhTable->item(i,5)->setTextAlignment(Qt::AlignCenter);



                    if(dvhTypeFlag==1)//Differential
                    {
                        myDVHCalc->histogramData2(0.02,myDVHCalc->doseValues,results[5],1);


                    }

                    else if(dvhTypeFlag==0)//Cumulative
                    {
                        //10cGy bin width
                        myDVHCalc->histogramData2(0.10,myDVHCalc->doseValues,results[5],0);
                        myDVHCalc->diffToCumulative(myDVHCalc->doseBins,myDVHCalc->volBins);
                        myDVHCalc->doseValues.clear();
                    }

                    // Fill in the table values
                    if(dvhTypeFlag==1)//Differential
                    {
                        int numPoints =myDVHCalc->volBins.size();                        
                        for (int i=0; i<numPoints; ++i)
                        {                            
                            CurROIDVH->append(myDVHCalc->doseBins[i],myDVHCalc->volBins[i]);

                        }

                    }

                    else if(dvhTypeFlag==0)//Cumulative
                    {
                        CurROIDVH->append(0.0,100.0);
                        int numPoints = myDVHCalc->cumVolume.size();
                        for (int i = 1; i<numPoints; ++i)
                        {
                            CurROIDVH->append(myDVHCalc->doseBins[i],myDVHCalc->cumVolume[i]);
                            qDebug()<<myDVHCalc->doseBins[i]<<""<<myDVHCalc->cumVolume[i];
                        }

                    }

                    QPen pen = CurROIDVH->pen();
                    pen.setWidth(2);
                    pen.setBrush(QBrush(QColor(this->ROIColors[i][0],this->ROIColors[i][1],this->ROIColors[i][2])));
                    CurROIDVH->setPen(pen);

                    CurROIDVH->setName(QString(this->structNames[i]));
                    DVHChart->addSeries(CurROIDVH);


                    //Clear DVH values for current ROI
                    myDVHCalc->doseBins.clear();
                    myDVHCalc->volBins.clear();
                    myDVHCalc->cumVolume.clear();
                    //delete myDVHCalc;


                }
                DVHChart->createDefaultAxes();
                DVHChart->axisX()->setTitleText("Dose (Gy)");
                DVHChart->axisY()->setMin(0);
                if(this->dvhTypeFlag==0)//If cumulative
                {
                    DVHChart->axisY()->setMax(100);
                    DVHChart->axisY()->setTitleText("Volume (%)");
                }

                else if(this->dvhTypeFlag==1)//If differential
                {
                    DVHChart->axisY()->setTitleText("No. of bins");
                }

                DVHChart->axisX()->setMin(0);


                //Set progress value
                int progress=double(x)/double(this->sitems.size())*100;
                pd->setValue(progress);
                QCoreApplication::processEvents();

            }

            this->ui->ChartView->setChart(DVHChart);
            this->ui->ChartView->setRenderHint(QPainter::Antialiasing);


            delete pd;
        }


}









//    }


//    else if(this->resampleDoseFlag==1)
//    {
//        if(this->ui->radioButtonLinear->isChecked())
//        {
//            this->interpolationTypeFlag=0;
//        }
//        else if(this->ui->radioButtonCubic->isChecked())
//        {
//            this->interpolationTypeFlag=1;
//        }
//        else if(this->ui->radioButtonNeighbor->isChecked())
//        {
//            this->interpolationTypeFlag=2;
//        }


//        DVHCalc* resampler=new DVHCalc;
//        double xSpacing=this->ui->xSpcSpinBox->value();
//        double ySpacing=this->ui->ySpcSpinBox->value();
//        double zSpacing=this->ui->zSpcSpinBox->value();
//        QMessageBox* msgBox=new QMessageBox(this);
//        msgBox->setText("Resampling dose grid");
//        msgBox->setIcon(QMessageBox::Information);
//        msgBox->show();
//        QCoreApplication::processEvents();

//        if(this->doseMatrix->GetSpacing()[0]!=1.0)
//        {
//           resampler->resampleImage(this->doseMatrix,xSpacing,ySpacing,zSpacing,this->interpolationTypeFlag);
//           this->doseMatrix=resampler->resampledImage;
//        }

//        msgBox->close();
//        delete msgBox;
//        QCoreApplication::processEvents();


//        //this->bins=this->ui->binSpinBox->value();
//        this->getSelectedItems();
//        // Set up the view
//        this->chart->GetAxis(vtkAxis::BOTTOM)->SetMaximum(this->maxDose+1);//Extra 1Gy for visual clarity
//        // this->chart->SetShowLegend(true);
//        this->chart->ClearPlots();

//        //Calculate DVH for each selected item
//        for (int x=0;x<this->sitems.size();x++)
//        {
//            // qDebug()<<timer->currentTime();
//            DVHCalc *myDVHCalc= new DVHCalc;//Instantiate inside the loop, otherwise it will create memory leaks
//            int i=this->sitems[x];
//            std::vector<double>results;
//            results=myDVHCalc->calcStructDVH(this->meshes[i],this->doseMatrix);
//            myDVHCalc->indicesX.clear();
//            myDVHCalc->indicesY.clear();
//            myDVHCalc->indicesZ.clear();

//            this->ui->dvhTable->item(i,1)->setText(QString::number(results[0]));//volume
//            this->ui->dvhTable->item(i,2)->setText(QString::number(results[1]));//max dose
//            this->ui->dvhTable->item(i,3)->setText(QString::number(results[2]));//mean dose
//            this->ui->dvhTable->item(i,4)->setText(QString::number(results[3]));//min dose
//            this->ui->dvhTable->item(i,5)->setText(QString::number(results[4]));//std

//            if(dvhTypeFlag==1)//Differential
//            {
//                myDVHCalc->histogramData2(0.01,myDVHCalc->doseValues,results[5],1);
//            }
//            else if(dvhTypeFlag==0)//Cumulative
//            {
//                myDVHCalc->histogramData2(0.01,myDVHCalc->doseValues,results[5],0);
//                myDVHCalc->diffToCumulative(myDVHCalc->doseBins,myDVHCalc->volBins);
//                myDVHCalc->doseValues.clear();
//            }

//            vtkSmartPointer<vtkTable >table=
//                    vtkSmartPointer<vtkTable>::New();
//            vtkSmartPointer<vtkFloatArray> arrX =
//                    vtkSmartPointer<vtkFloatArray>::New();
//            arrX->SetName("X Axis");
//            table->AddColumn(arrX);
//            vtkSmartPointer<vtkFloatArray> arrY =
//                    vtkSmartPointer<vtkFloatArray>::New();
//            arrY->SetName(this->structNames[i].toLatin1());
//            table->AddColumn(arrY);
//            // Fill in the table with some example values
//            if(dvhTypeFlag==1)//Differential
//            {
//                int numPoints =myDVHCalc->doseBins.size();
//                table->SetNumberOfRows(numPoints);
//                for (int i = 0; i < numPoints; ++i)
//                {
//                    table->SetValue(i, 0, myDVHCalc->doseBins[i]);
//                    table->SetValue(i, 1, myDVHCalc->volBins[i]);
//                }
//                // qDebug()<<"Diff calculated";
//            }

//            else if(dvhTypeFlag==0)//Cumulative
//            {
//                int numPoints = myDVHCalc->cumVolume.size();
//                table->SetNumberOfRows(numPoints);
//                table->SetValue(0,1,100);
//                table->SetValue(0,0,0);
//                for (int i = 1; i < numPoints; ++i)
//                {
//                    table->SetValue(i, 0, myDVHCalc->doseBins[i]);
//                    table->SetValue(i, 1, myDVHCalc->cumVolume[i]);

//                }
//                //             qDebug()<<"Cum calculated";
//            }

//            myDVHCalc->doseBins.clear();
//            myDVHCalc->cumVolume.clear();

//            vtkPlot *line = this->chart->AddPlot(vtkChart::LINE);
//            line->SetInputData(table, 0, 1);
//            line->SetColor(this->ROIColors[i][0],this->ROIColors[i][1],this->ROIColors[i][2],255);
//            line->SetWidth(2.0);

//            delete myDVHCalc;
//            //qDebug()<<timer->currentTime();

//        }
//        delete resampler;
//    }









void DVHDialog::on_ResetViewButton_clicked()
{

}

void DVHDialog::on_groupBoxResampleDoseGrid_toggled(bool )
{
    bool flag=this->ui->groupBoxResampleDoseGrid->isChecked();
    this->resampleDoseFlag=flag;
//    qDebug()<<this->resampleDoseFlag<<"Resample Dose";

}

void DVHDialog::on_checkBoxLegend_clicked()
{
    if(this->ui->checkBoxLegend->isChecked())
    {
        this->ui->ChartView->chart()->legend()->show();
    }

    else
    {
       this->ui->ChartView->chart()->legend()->hide();
    }

}

void DVHDialog::on_saveFig_clicked()
{
    QString fileName=QFileDialog::getSaveFileName(this,"Save DVH Figure","", "png(*.png)");
    this->ui->ChartView->grab().save(fileName.append(".png"));
}

void DVHDialog::on_actionSave_Figure_triggered()
{
    this->on_saveFig_clicked();
}

void DVHDialog::on_actionReset_View_triggered()
{
    this->on_ResetViewButton_clicked();
}

void DVHDialog::on_actionCalculate_DVH_triggered()
{
    this->on_pushButton_clicked();
}



