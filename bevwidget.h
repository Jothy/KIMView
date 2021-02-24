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

#ifndef BEVWIDGET_H
#define BEVWIDGET_H

#include <QWidget>
#include<QActionGroup>

#include<vtkSmartPointer.h>
#include<vtkPolyData.h>
#include<vtkActorCollection.h>
#include<vtkRenderer.h>
#include<vtkInteractorStyleImage.h>
#include<vtkRenderWindowInteractor.h>
#include<vtkAnnotatedCubeActor.h>
#include<vtkOrientationMarkerWidget.h>
#include<vtkAxesActor.h>
#include<vtkImageData.h>


namespace Ui {
class BEVWidget;
}

class BEVWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BEVWidget(QWidget *parent = nullptr,QActionGroup *contextMenus=0);
    ~BEVWidget();
    double (*ROIColors)[3];
    std::vector<vtkSmartPointer<vtkPolyData> >MeshList;
    std::vector<vtkSmartPointer<vtkPolyData> >BeamList;
    //vtkSmartPointer<vtkRenderer>ViewRenderer;
    vtkSmartPointer<vtkActorCollection>MeshActors;
    vtkSmartPointer<vtkActorCollection>BeamActors;
    vtkSmartPointer<vtkRenderer>ModelRenderer;
    vtkSmartPointer<vtkRenderer>ModelRenderer2;
    vtkSmartPointer<vtkInteractorStyleImage>ImageStyleImg;
    vtkSmartPointer<vtkRenderWindowInteractor>Interactor3D;
    vtkSmartPointer<vtkInteractorStyleTrackballCamera>InteractorTrackball;
    vtkSmartPointer<vtkAnnotatedCubeActor>AnnotatedCube;
    vtkSmartPointer<vtkOrientationMarkerWidget>MarkerWidget;
    vtkSmartPointer<vtkOrientationMarkerWidget>MarkerWidget2;
    vtkSmartPointer<vtkActor>BEVField;
    vtkSmartPointer<vtkAxesActor>Axes;
    vtkSmartPointer<vtkActor>IsodoseSurface;
    vtkSmartPointer<vtkImageData>RTDose;



    double *Isocentre;
    double GantryAngle;

    void DisplayBEV();
    void DisplayMeshes();
    void DisplayBeams();
    void ShowIsodoseSurface(double minDose, double maxDose);


private:
    Ui::BEVWidget *ui;
};

#endif // BEVWIDGET_H
