#ifndef CREATEOBJECTS_H
#define CREATEOBJECTS_H

#include"vtkCornerAnnotation.h"
#include"vtkTextActor.h"
#include"vtkSmartPointer.h"
#include"vtkActor.h"
#include<vtkFollower.h>
#include"vtkPolyData.h"
#include<vtkAnnotatedCubeActor.h>
#include <vtkLegendBoxActor.h>
#include<vtkImageData.h>
#include<vtkCaptionActor2D.h>

#include<vector>
#include<QString>

class CreateObjects
{
public:
    CreateObjects();
    vtkSmartPointer<vtkCornerAnnotation> annotation;
    void createAnnotation(int location,double RGB[3],QString text1);    
    vtkSmartPointer<vtkPolyData> createSphere(double radius,int thetaR,int phiR,double center[3]);
    vtkSmartPointer<vtkPolyData> createCube(double x,double y,double z,double center[3]);
    vtkSmartPointer<vtkPolyData> createCylinder(double radius,double height,double center[3]);    
    vtkSmartPointer<vtkLegendBoxActor> createIsodoseLegend(std::vector<double>isodoseValues,std::vector<double>colorsR,std::vector<double>colorsG,std::vector<double>colorsB);

    vtkSmartPointer<vtkActor> createRectangle(double x1,double x2,double y1,double y2,double gantryAngle,double collAngle,double isocenter[3]);
    vtkSmartPointer<vtkActor> createLeaf(double length,double thickness,double position[3],double isocenter[3]);
    vtkSmartPointer<vtkAssembly> createVarian120MLC(double isocenter[3],double gantryAngle,double collAngle,vtkTransform *userTr);
    vtkSmartPointer<vtkActor> createVectorText(QString txt,double collAngle,double gantryAngle);
    vtkSmartPointer<vtkActor> createAxes(double shift[3],double isocenter[3]);
    vtkSmartPointer<vtkCaptionActor2D> createCaption2D();    
    vtkSmartPointer<vtkAssembly> createGraticule();

};

#endif // CREATEOBJECTS_H
