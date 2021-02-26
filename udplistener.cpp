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

#include "udplistener.h"

#include<udplistener.h>
#include<QUdpSocket>
#include<QElapsedTimer>
#include<QApplication>
#include<QSettings>

//Tracking includes
#include<vtkProperty.h>
#include<imageviewer2d.h>
#include<vtkRenderWindow.h>

UDPListener::UDPListener(QObject *parent) : QObject(parent)
{
    // create a QUDP socket
    socket = new QUdpSocket(this);

    //Instantiate tracking members
    this->TrackingTransform=vtkSmartPointer<vtkTransform>::New();
    this->TrackingPolydataTransform=vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    this->TrackingPolyData=vtkSmartPointer<vtkPolyData>::New();
    this->TrackingTarget=vtkSmartPointer<vtkPolyData>::New();

    this->TrackingActor3D=vtkSmartPointer<vtkActor>::New();
    this->TrackingActor3D->GetProperty()->SetColor(1,1,0);
    this->TrackingActorAxial=vtkSmartPointer<vtkActor>::New();
    this->TrackingActorSagittal=vtkSmartPointer<vtkActor>::New();
    this->TrackingActorCoronal=vtkSmartPointer<vtkActor>::New();


    this->TrackingMapper=vtkSmartPointer<vtkPolyDataMapper>::New();
    //this->TrackingMapper->ImmediateModeRenderingOn();
}

UDPListener::~UDPListener()
{
    socket->close();
    socket->deleteLater();
}

void UDPListener::HelloUDP()
{
//    QByteArray Data;
//    Data.append("Hello from UDP");
//    //Sends the datagram datagram
//    socket->writeDatagram(Data, QHostAddress::LocalHost,45617);
//    qDebug()<<"Writing...";
}

void UDPListener::readMessage()
{
//    QElapsedTimer timer;
//    timer.start();

    //When data comes in
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;

    // qint64 QUdpSocket::readDatagram(char * data, qint64 maxSize,
    //                 QHostAddress * address = 0, quint16 * port = 0)
    // Receives a datagram no larger than maxSize bytes and stores it in data.
    // The sender's host address and port is stored in *address and *port
    // (unless the pointers are 0).

    socket->readDatagram(buffer.data(), buffer.size(),
                         &sender, &senderPort);

//    qDebug() << "Message from: " << sender.toString();
//    qDebug() << "Message port: " << senderPort;
//    qDebug() << "Message: " << buffer;

    //The UDP format is [X,Y,Z,Gantry] in IEC(cm) and Varian degrees
    //IEC to LPS conversion, simple approach as it only supports HFS orientation now
    this->shifts[0]=buffer.split(' ')[0].toDouble()*10;//cm to mm
    this->shifts[1]=-buffer.split(' ')[2].toDouble()*10;//cm to mm
    this->shifts[2]=buffer.split(' ')[1].toDouble()*10;//cm to mm
    //qDebug()<<"Shifts: "<<this->shifts[0]<<""<<this->shifts[1]<<""<<this->shifts[2];

    this->UpdateViews();
    QApplication::processEvents();

//    qDebug() <<"Rendering took" << timer.elapsed() << "milliseconds";


}

void UDPListener::StartListening()
{
    //qDebug()<<"Start";
    //Receiver port
    QSettings settings("ImageX","KIMView");
    int KIMViewPort=settings.value("KIMViewPort").toInt();
    socket->bind(KIMViewPort);
    //KIM IP and port
    QString KIMIP=settings.value("KIMIP").toString();
    int KIMPort=settings.value("KIMPort").toInt();
    socket->connectToHost(QHostAddress(KIMIP),KIMPort);
    //socket->waitForConnected(1000);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readMessage()));

}

void UDPListener::StopListening()
{
    socket->close();
}

void UDPListener::UpdateViews()
{
    /*******************************Tracking.......................................*/
    //Remove previous last actors
    this->BEVViewer->ModelRenderer->RemoveViewProp(this->TrackingActor3D);
    this->AxialViewer->ViewRenderer->RemoveViewProp(this->TrackingActorAxial);
    this->SagittalViewer->ViewRenderer->RemoveViewProp(this->TrackingActorSagittal);
    this->CoronalViewer->ViewRenderer->RemoveViewProp(this->TrackingActorCoronal);

    //Transform actor
    this->TrackingTransform->Identity();
    this->TrackingTransform->Translate(this->shifts);
    this->TrackingPolydataTransform->SetTransform(this->TrackingTransform);
    this->TrackingPolydataTransform->SetInputData(this->TrackingTarget);
    this->TrackingMapper->SetInputConnection(this->TrackingPolydataTransform->GetOutputPort());
    this->TrackingActor3D->SetMapper(this->TrackingMapper);

    this->BEVViewer->ModelRenderer->AddViewProp(this->TrackingActor3D);
    this->BEVViewer->ModelRenderer->GetRenderWindow()->Render();

    this->TrackingPolyData->DeepCopy(this->TrackingPolydataTransform->GetOutput());

    //Update 2D views
    this->TrackingActorAxial=this->AxialViewer->CutROI(this->TrackingPolyData,this->AxialViewer->SliceLoc,1,1,0,this->AxialViewer->SliceOrientation);
    this->TrackingActorAxial->GetProperty()->SetLineWidth(3.0);    
    this->AxialViewer->ViewRenderer->AddActor(this->TrackingActorAxial);
    this->AxialViewer->ViewRenderer->GetRenderWindow()->Render();

    this->TrackingActorSagittal=this->SagittalViewer->CutROI(this->TrackingPolyData,this->SagittalViewer->SliceLoc,1,1,0,this->SagittalViewer->SliceOrientation);
    this->TrackingActorSagittal->GetProperty()->SetLineWidth(3.0);
    this->SagittalViewer->ViewRenderer->AddActor(this->TrackingActorSagittal);
    this->SagittalViewer->ViewRenderer->GetRenderWindow()->Render();

    this->TrackingActorCoronal=this->CoronalViewer->CutROI(this->TrackingPolyData,this->CoronalViewer->SliceLoc,1,1,0,this->CoronalViewer->SliceOrientation);
    this->TrackingActorCoronal->GetProperty()->SetLineWidth(3.0);
    this->CoronalViewer->ViewRenderer->AddActor(this->TrackingActorCoronal);
    this->CoronalViewer->ViewRenderer->GetRenderWindow()->Render();
}




