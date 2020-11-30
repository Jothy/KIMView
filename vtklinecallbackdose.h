/**************************************************************************
(c)Jothybasu Selvaraj
This program comes with no gaurantee of its accuracy.
Clinical decisions should not be made using this program
**************************************************************************/

#ifndef VTKLINECALLBACKDOSE_H
#define VTKLINECALLBACKDOSE_H

#include<vtkPolyData.h>
#include<vtkSmartPointer.h>
#include<vtkCommand.h>
#include<vtkImageData.h>
#include<vtkLineWidget2.h>

#include<QMdiArea>
#include<QSplineSeries>

#include"doseprofiledialog.h"

class vtkLineCallbackDose: public vtkCommand
{
public:
    vtkLineCallbackDose(QWidget *parent);
    ~ vtkLineCallbackDose();
    vtkLineCallbackDose *New(QWidget *parent);
    vtkSmartPointer<vtkPolyData>lineData;    
    void Execute(vtkObject *caller, unsigned long, void*);
    vtkSmartPointer<vtkImageData>dose;    
    DoseProfileDialog* doseProfiler;
    float distance;
    double trX,trY,trZ;//X,Y,Z translations
    void transformPolyData();    
    int SliceOrientation=0;//Axial by default
    QSplineSeries *SplineSeries;

};

#endif // VTKLINECALLBACKDOSE_H
