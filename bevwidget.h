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
    vtkSmartPointer<vtkRenderer>ViewRenderer;    
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


    double *Isocentre;
    double GantryAngle;

    void DisplayBEV();
    void DisplayMeshes();
    void DisplayBeams();


private:
    Ui::BEVWidget *ui;
};

#endif // BEVWIDGET_H
