#ifndef UDPLISTENER_H
#define UDPLISTENER_H

#include <QObject>
#include <QUdpSocket>

//Tracking function headers
#include<vtkTransform.h>
#include<vtkTransformPolyDataFilter.h>
#include<vtkActor.h>
#include<vtkPolyDataMapper.h>
#include<imageviewer2d.h>
#include<bevwidget.h>



class UDPListener : public QObject
{
    Q_OBJECT
public:
    explicit UDPListener(QObject *parent = nullptr);
    ~UDPListener();
    void HelloUDP();
    double shifts[3]={0,0,0};

    //Tracking members
    vtkSmartPointer<vtkTransform>TrackingTranform;
    vtkSmartPointer<vtkTransformPolyDataFilter>TrackingPolydataTransform;
    vtkSmartPointer<vtkActor>TrackingActor3D;
    vtkSmartPointer<vtkPolyDataMapper>TrackingMapper;
    vtkSmartPointer<vtkActor>TrackingActorAxial;
    vtkSmartPointer<vtkActor>TrackingActorSagittal;
    vtkSmartPointer<vtkActor>TrackingActorCoronal;
    vtkSmartPointer<vtkPolyData>TrackingPolyData;
    vtkSmartPointer<vtkPolyData>TrackingTarget;
    ImageViewer2D *AxialViewer;
    ImageViewer2D *CoronalViewer;
    ImageViewer2D *SagittalViewer;
    BEVWidget *BEVViewer;
    void UpdateViews();





signals:

public slots:
    void readMessage();
    void StartListening();
    void StopListening();


private:
    QUdpSocket *socket;

};

#endif // UDPLISTENER_H
