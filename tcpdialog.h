/**************************************************************************
(c)Jothybasu Selvaraj
This program comes with no gaurantee of its accuracy.
Clinical decisions should not be made using this program
**************************************************************************/

#ifndef TCPDIALOG_H
#define TCPDIALOG_H

#include <QDialog>
#include<QList>
#include<QString>

#include<vtkImageData.h>
#include<vtkPolyData.h>
#include<vtkSmartPointer.h>

#include<vector>

namespace Ui {
    class TCPDialog;
}

class TCPDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TCPDialog(QWidget *parent = 0);
    ~TCPDialog();
     void setStructureNames(QList<QString>structNames);
     int selectedStructureNum;
     std::vector<vtkSmartPointer<vtkPolyData> >meshes;
     vtkSmartPointer<vtkPolyData>structure;
     vtkSmartPointer<vtkImageData>doseGrid;
     vtkSmartPointer<vtkImageData>clonogenGrid;
     double alphaByBeta;
     double alpha;
     double alphaMean;
     double alphaStd;
     double clonogenDensity;
     double fractions;
     double temporalCF;
     std::vector<double>doseBins;
     std::vector<double>volBins;
     std::vector<float>clonogenVector;
     double voxelVol;
     bool calcType;
     void calcTCP();
     void calcTCPVaryingClonogens();


private:
    Ui::TCPDialog *ui;

private slots:    
    void on_pushButton_clicked();

};

#endif // TCPDIALOG_H
