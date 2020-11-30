#include "bevwidget.h"
#include "ui_bevwidget.h"

#include<QDebug>
#include<QActionGroup>

#include<vtkRenderer.h>
#include<vtkRenderWindow.h>
#include<vtkProperty.h>
#include<vtkCamera.h>
#include<vtkInteractorStyleImage.h>
#include<vtkRenderWindowInteractor.h>
#include<vtkXMLPolyDataReader.h>
#include<vtkPolyDataMapper.h>
#include<vtkPolyData.h>
#include<vtkPointData.h>
#include<vtkActor.h>
#include<vtkOrientationMarkerWidget.h>
#include<vtkTransform.h>
#include<vtkTransformFilter.h>
#include<vtkAxesActor.h>
#include<vtkOrientationMarkerWidget.h>


BEVWidget::BEVWidget(QWidget *parent,QActionGroup *contextMenus) :
    QWidget(parent),
    ui(new Ui::BEVWidget)
{
    ui->setupUi(this);

    for (int i=0;i<contextMenus->actions().size();i++)
    {
        this->ui->BEVWindow->addAction(contextMenus->actions()[i]);
    }

    this->ModelRenderer=vtkSmartPointer<vtkRenderer>::New();
    this->ModelRenderer2=vtkSmartPointer<vtkRenderer>::New();

    this->ui->BEVWindow->GetRenderWindow()->AddRenderer(this->ModelRenderer);
//    this->ModelRenderer->SetLayer(0);
//    this->ui->BEVWindow->GetRenderWindow()->AddRenderer(this->ModelRenderer2);
//    this->ModelRenderer2->SetLayer(1);
//    this->ui->BEVWindow->GetRenderWindow()->SetNumberOfLayers(2);


    this->ImageStyleImg=vtkSmartPointer<vtkInteractorStyleImage>::New();
    this->Interactor3D=vtkSmartPointer<vtkRenderWindowInteractor>::New();
    this->InteractorTrackball=vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();

//    //Initialize
//    this->AnnotatedCube=vtkSmartPointer<vtkAnnotatedCubeActor>::New();
//    this->AnnotatedCube->SetXPlusFaceText("L");
//    this->AnnotatedCube->SetXMinusFaceText("R");
//    this->AnnotatedCube->SetYPlusFaceText("P");
//    this->AnnotatedCube->SetYMinusFaceText("A");
//    this->AnnotatedCube->SetZPlusFaceText("H");
//    this->AnnotatedCube->SetZMinusFaceText("F");
//    this->AnnotatedCube->SetXFaceTextRotation(90.0);
//    this->AnnotatedCube->SetYFaceTextRotation(0.0);
//    this->AnnotatedCube->SetZFaceTextRotation(-90.0);
//    this->AnnotatedCube->GetTextEdgesProperty()->SetColor(0.5,0.5,0.5);
//    this->AnnotatedCube->GetTextEdgesProperty()->SetLineWidth(1.0);
//    this->AnnotatedCube->GetCubeProperty()->SetColor(1,0.68,0.79);
//    //this->AnnotatedCube->SetUserTransform(this->UserTransform);

    // Read all the data from the file
    vtkSmartPointer<vtkXMLPolyDataReader> reader =
            vtkSmartPointer<vtkXMLPolyDataReader>::New();
    reader->SetFileName("Human.vtp");
    reader->Update();


    vtkSmartPointer<vtkPolyData>HumanPolyData=
            vtkSmartPointer<vtkPolyData>::New();
    HumanPolyData->ShallowCopy(reader->GetOutput());
    HumanPolyData->GetPointData()->SetActiveScalars("Color");



    // Visualize
    vtkSmartPointer<vtkPolyDataMapper>iconMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    iconMapper->SetInputData(HumanPolyData);
    iconMapper->SetColorModeToDirectScalars();

    vtkSmartPointer<vtkActor> actor =
            vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(iconMapper);
    actor->RotateX(180.0);//Rotate 180 deg to account for vtk and DICOM axes differences
    actor->RotateY(180.0);



    //Initialize orientation marker widget forthe "boy"
    this->MarkerWidget=vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    this->MarkerWidget->SetOrientationMarker(actor);
    this->MarkerWidget->SetViewport(0.0,0.0,0.2,0.2);//[Xmin,Ymin.Xmax,Ymax]
    this->MarkerWidget->SetInteractor(this->ModelRenderer->GetRenderWindow()->GetInteractor());
    this->MarkerWidget->EnabledOn();
    this->MarkerWidget->InteractiveOff();


    //Initialize orientation marker widget forthe "boy"
    this->Axes=vtkSmartPointer<vtkAxesActor>::New();
    this->Axes->AxisLabelsOn();
    this->Axes->SetTotalLength(75,75,75);

    this->MarkerWidget2=vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    this->MarkerWidget2->SetOutlineColor( 0.93,0.57,0.13);
    this->MarkerWidget2->SetOrientationMarker(this->Axes);
    this->MarkerWidget2->SetInteractor(this->ModelRenderer->GetRenderWindow()->GetInteractor());
    this->MarkerWidget2->SetViewport( 0.0,0.7,0.1,1.0);//[Xmin,Ymin.Xmax,Ymax]
    this->MarkerWidget2->SetEnabled(1);
    this->MarkerWidget2->InteractiveOff();


}

BEVWidget::~BEVWidget()
{
    delete ui;
}


void BEVWidget::DisplayBEV()
{
    //qDebug()<<"No. of meshes found:"<<this->MeshActors->GetNumberOfItems();
    this->MeshActors->InitTraversal();
    for(vtkIdType i = 0; i < this->MeshActors->GetNumberOfItems(); i++)
    {
        double R=this->ROIColors[i][0]/255.0;
        double G=this->ROIColors[i][1]/255.0;
        double B=this->ROIColors[i][2]/255.0;

        vtkActor *CurActor=this->MeshActors->GetNextItem();
        CurActor->GetProperty()->SetColor(R,G,B);
        //CurActor->GetProperty()->SetOpacity(0.9);
        this->ModelRenderer->AddViewProp(CurActor);
        //qDebug()<<R<<G<<B<<"BEV RGB";
    }

    //Remove all beams if exists
    this->BeamActors->InitTraversal();
    for(vtkIdType i = 0; i < this->MeshActors->GetNumberOfItems(); i++)
    {
        vtkActor *CurActor=this->BeamActors->GetNextItem();
        this->ModelRenderer->RemoveViewProp(CurActor);
    }

    this->BEVField=vtkSmartPointer<vtkActor>::New();
    this->ModelRenderer2->AddActor(this->BEVField);


    //Settings for 3D rendering; flips camera according to DICOM coords
    this->ModelRenderer->GetActiveCamera()->ParallelProjectionOff();
    this->ModelRenderer->GetActiveCamera()->SetFocalPoint(this->Isocentre);
    this->ModelRenderer->GetActiveCamera()->SetPosition(0.0,this->Isocentre[1]-1000.0,0.0);
    this->ModelRenderer->GetActiveCamera()->SetViewUp(0,0,1);
    this->ModelRenderer->GetActiveCamera()->Azimuth(this->GantryAngle);
    //this->ModelRenderer->ResetCameraClippingRange();
    //this->ModelRenderer->ResetCamera();

    this->ModelRenderer->ResetCamera();
    this->ModelRenderer->GetRenderWindow()->GetInteractor()->SetInteractorStyle(this->ImageStyleImg);
    this->ModelRenderer->GetRenderWindow()->GetInteractor()->Start();
    //qDebug()<<this->Isocentre[0]<<this->Isocentre[1]<<this->Isocentre[2]<<"Isocentre";
    //qDebug()<<*this->GantryAngle<<"Ganntry Angle";

    this->ModelRenderer2->SetActiveCamera(this->ModelRenderer->GetActiveCamera());
    this->ModelRenderer2->ResetCamera();
    this->ModelRenderer2->ResetCameraClippingRange();
    this->ui->BEVWindow->GetRenderWindow()->Render();


}


void BEVWidget::DisplayMeshes()
{
    //qDebug()<<"No. of meshes found:"<<this->MeshActors->GetNumberOfItems();
    this->MeshActors->InitTraversal();
    for(vtkIdType i = 0; i < this->MeshActors->GetNumberOfItems(); i++)
    {
        double R=this->ROIColors[i][0]/255.0;
        double G=this->ROIColors[i][1]/255.0;
        double B=this->ROIColors[i][2]/255.0;

        vtkActor *CurActor=this->MeshActors->GetNextItem();
        CurActor->GetProperty()->SetColor(R,G,B);
        //CurActor->GetProperty()->SetOpacity(0.9);
        this->ModelRenderer->AddViewProp(CurActor);
        //qDebug()<<R<<G<<B<<"BEV RGB";
    }



    this->ModelRenderer->GetActiveCamera()->SetViewUp(0,-1,0);
    this->ModelRenderer->GetActiveCamera()->SetFocalPoint(0.0,0.0,0.0);
    this->ModelRenderer->GetActiveCamera()->SetPosition(0.0,0.0,-1.0);
    this->ModelRenderer->GetRenderWindow()->Render();    
    this->ModelRenderer->GetRenderWindow()->GetInteractor()->Start();


}

void BEVWidget::DisplayBeams()
{
    //qDebug()<<"No. of beams found:"<<this->BeamActors->GetNumberOfItems();
    this->BeamActors->InitTraversal();
    for(vtkIdType i = 0; i < this->BeamActors->GetNumberOfItems(); i++)
    {
        vtkActor *CurActor=this->BeamActors->GetNextItem();
        CurActor->GetProperty()->SetColor(255,255,0);
        CurActor->GetProperty()->SetOpacity(0.8);
        this->ModelRenderer->AddViewProp(CurActor);
    }

    this->ModelRenderer2->RemoveActor(this->BEVField);


    this->ModelRenderer->GetRenderWindow()->Render();
    this->ModelRenderer->ResetCamera();
    this->ModelRenderer->GetRenderWindow()->GetInteractor()->Start();


}

