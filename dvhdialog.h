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
#ifndef DVHDIALOG_H
#define DVHDIALOG_H


#include<QDialog>
#include<QList>
#include<QString>
#include<QSplineSeries>

#include<vtkImageData.h>
#include<vtkPolyData.h>
#include<vtkSmartPointer.h>
#include<vtkContextView.h>
#include<vtkChartXY.h>
#include<vtkTable.h>

#include<vector>
//#include"boost/multi_array.hpp"

QT_CHARTS_USE_NAMESPACE

namespace Ui {
    class DVHDialog;
}

class DVHDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DVHDialog(QWidget *parent = 0);
    ~DVHDialog();
    void setStructureNames(QList<QString>structNames);
    void setROIColors(double[30][3]);
    void getSelectedItems();
    vtkSmartPointer<vtkImageData>doseMatrix;
    //vtkSmartPointer< vtkImageData>resampledDoseMatrix;
    std::vector<vtkSmartPointer<vtkPolyData> > meshes;    
    QList<QString>structNames;
    QList<QString>ROIType;
    double ROIColors[50][3]; //No. of ROIs,RGB- maximum 30 ROIs supported for now
    double maxDose;    
    unsigned int bins;
    unsigned int resampleDoseFlag;
    unsigned int interpolationTypeFlag;
    unsigned int dvhTypeFlag;
    double xSpc,ySpc,zSpc;
    std::vector<std::vector<double> > dvhTable;
    std::vector<int>dvhBinsVector;//Imported dvh bin Nos   
    QList<int>refROINums;//Referenced ROi nos from DVH in RTDose.dcm
    QList<int> ROINo;//ROi Referenced No. from RTStruct.dcm
    QList<int>refNoOfSelectedROIs;
    QList<int>sitems;//Selected items


    double alphaByBeta;
    double alpha;
    double alphaMean;
    double alphaStd;
    double clonogenDensity;
    double fractions;
    double temporalCF;
    double xMeanSys,yMeanSys,zMeanSys;
    double xSDSys,ySDSys,zSDSys;
    double xMeanRan,yMeanRan,zMeanRan;
    double xSDRan,ySDRan,zSDRan;
    int selectedStructureNum;


private:
    Ui::DVHDialog *ui;

private slots:
    void on_actionCalculate_DVH_triggered();
    void on_actionReset_View_triggered();
    void on_actionSave_Figure_triggered();    
    void on_saveFig_clicked();
    void on_checkBoxLegend_clicked();
    void on_groupBoxResampleDoseGrid_toggled(bool);
    void on_ResetViewButton_clicked();
    void on_pushButton_clicked();

};

#endif // DVHDIALOG_H
