/**************************************************************************
(c)Jothybasu Selvaraj
This program comes with no gaurantee of it accuracy.
Clinical decisions should not be made using this program
**************************************************************************/

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
