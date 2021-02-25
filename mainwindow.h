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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define vtkRenderingCore_AUTOINIT 3(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingOpenGL2)

#include<QMainWindow>
#include<QActionGroup>
#include<QString>
#include<QList>
#include<QDockWidget>
#include<QMap>

#include<vtkRenderer.h>
#include<vtkSmartPointer.h>
#include<vtkImageData.h>
#include<vtkPolyData.h>
#include<vtkBoxWidget2.h>
#include<vtkRenderWindowInteractor.h>
#include<vtkInteractorStyleTrackballCamera.h>
#include<vtkObject.h>

//Tracking function headers
#include<vtkTransform.h>
#include<vtkTransformPolyDataFilter.h>

#include<imageviewer2d.h>
#include<bevwidget.h>

#include<udplistener.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QActionGroup *ContextMenus;    
    vtkSmartPointer<vtkImageData>RTDose;
    std::vector<vtkSmartPointer<vtkPolyData> >MeshList;
    std::vector<vtkSmartPointer<vtkPolyData> >BeamList;
    std::vector<vtkSmartPointer<vtkPolyData> >POIList;
    vtkSmartPointer<vtkActorCollection>MeshActors;
    vtkSmartPointer<vtkActorCollection>BeamActors;
    //No. of ROIs,RGB- maximum 50 ROIs supported for now
    double ROIColors[50][3];
    QList<QString> ROINames;
    QList<QString>ROITypes;
    QList<int> ROINo;
    QList<double>BeamAngles;
    QList<double>CollAngles;
    int ROINum=0;
    vtkSmartPointer<vtkImageData>CTImage;
    QList<float>PhaseInfo;
    double DoseVOI[6];
    double Isocentre[3]={0.0,0.0,0.0};//zero by default (CT origin)
    unsigned int BeamNum=0;
    double DoseGridSpacing=3.0;//3mm default

    double TargetReduction=0.10;
    bool ROIVisibleFlag=1;//Visible by default
    int DefaultTargetIdx=0;

    vtkSmartPointer<vtkInteractorStyleTrackballCamera>InteractorTrackball;

    ImageViewer2D *SagittalViewer;
    ImageViewer2D *AxialViewer;
    ImageViewer2D *CoronalViewer;
    BEVWidget *BEVViewer;
    double CalcSSD(double Iso[3], double GantryAngle,vtkDataSet *BodyMesh);
    UDPListener * listener;

    QMap<QString,QString>PatientInfo;


private slots:
    void on_actionDose_triggered();    
    void on_actionCT_triggered();
    void on_actionStructures_triggered();      
    void on_actionGo_To_Isocentre_triggered();
    void on_actionBEV_triggered();
    void on_action3DView_triggered();
    void on_actionClose_Patient_triggered();
    void on_actionReset_Zoom_triggered();
    void on_actionShowBeams_triggered();
    void on_actionShowDose_triggered();
    void on_actionShowContours_triggered();
    void on_actionZoom_In_All_triggered();
    void on_actionZoom_Out_All_triggered();
    void on_actionInformation_triggered();
    void on_actionPlan_Information_triggered();
    void on_actionCalc_DVH_triggered();    
    void on_actionAdjust_Range_triggered();
    void on_actionReset_WL_WW_triggered();
    void on_actionRender_Bones_triggered();

    void on_actionHello_UDP_triggered();

    void on_actionMove_ROI_triggered();

    void on_actionRotate_ROI_triggered();

    void on_actionAdd_Arc_triggered();

    void on_actionSend_UDP_triggered();

    void on_actionAbout_QT_triggered();

    void on_actionIP_COnfiguration_triggered();

    void on_actionStart_triggered();

    void on_actionStop_triggered();

private:
    Ui::MainWindow *ui;
    vtkSmartPointer<vtkRenderer>renderer;



};

#endif // MAINWINDOW_H
