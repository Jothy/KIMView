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

#include <udplistener.h>

#include <QApplication>
#include <QElapsedTimer>
#include <QSettings>
#include <QUdpSocket>

// Tracking includes
#include <imageviewer2d.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>

UDPListener::UDPListener(QObject *parent) : QObject(parent) {
  // create a QUDP socket
  socket = new QUdpSocket(this);

  // Instantiate tracking members
  this->TrackingTransform = vtkSmartPointer<vtkTransform>::New();
  this->TrackingPolydataTransform =
      vtkSmartPointer<vtkTransformPolyDataFilter>::New();
  this->TrackingPolyData = vtkSmartPointer<vtkPolyData>::New();
  this->TrackingTarget = vtkSmartPointer<vtkPolyData>::New();

  this->TrackingActor3D = vtkSmartPointer<vtkActor>::New();
  this->TrackingActor3D->GetProperty()->SetColor(1, 1, 0);
  this->TrackingActorAxial = vtkSmartPointer<vtkActor>::New();
  this->TrackingActorSagittal = vtkSmartPointer<vtkActor>::New();
  this->TrackingActorCoronal = vtkSmartPointer<vtkActor>::New();

  this->TrackingMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  // this->TrackingMapper->ImmediateModeRenderingOn();
}

UDPListener::~UDPListener() {
  socket->close();
  socket->deleteLater();
}

void UDPListener::HelloUDP() {
  //    QByteArray Data;
  //    Data.append("Hello from UDP");
  //    //Sends the datagram datagram
  //    socket->writeDatagram(Data, QHostAddress::LocalHost,45617);
  //    qDebug()<<"Writing...";
}

void UDPListener::readMessage() {
  //    QElapsedTimer timer;
  //    timer.start();

  // When data comes in
  QByteArray buffer;
  buffer.resize(socket->pendingDatagramSize());

  QHostAddress sender;
  quint16 senderPort;

  socket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

  //    qDebug() << "Message from: " << sender.toString();
  //    qDebug() << "Message port: " << senderPort;
  //    qDebug() << "Message: " << buffer;

  UDPMsg *UDPShifts = new UDPMsg;

  QByteArray shiftXArray;
  for (int i = 0; i < 8; i++) {
    shiftXArray[i] = buffer.at(i);
  }

  QByteArray shiftYArray;
  for (int j = 8; j < 16; j++) {
    shiftYArray[j] = buffer.at(j);
  }

  QByteArray shiftZArray;
  for (int k = 16; k < 24; k++) {
    shiftZArray[k] = buffer.at(k);
  }

  QByteArray rotXArray;
  for (int x = 24; x < 32; x++) {
    rotXArray[x] = buffer.at(x);
  }

  QByteArray rotYArray;
  for (int j = 32; j < 40; j++) {
    rotYArray[j] = buffer.at(j);
  }

  QByteArray rotZArray;
  for (int z = 40; z < 48; z++) {
    rotZArray[z] = buffer.at(z);
  }

  QByteArray gantryArray;
  for (int w = 48; w < 56; w++) {
    gantryArray[w] = buffer.at(w);
  }

  QByteArray beamHold;
  for (int v = 56; v < 58; v++) {
    gantryArray[v] = buffer.at(v);
  }

  UDPShifts->shiftX = *(double *)(shiftXArray.constData());
  UDPShifts->shiftY = *(double *)(shiftYArray.remove(0, 8).constData());
  UDPShifts->shiftZ = *(double *)(shiftZArray.remove(0, 16).constData());
  UDPShifts->rotationX = *(double *)(rotXArray.remove(0, 24).constData());
  UDPShifts->rotationY = *(double *)(rotYArray.remove(0, 32).constData());
  UDPShifts->rotationZ = *(double *)(rotZArray.remove(0, 40).constData());
  UDPShifts->gantryAngle = *(double *)(gantryArray.remove(0, 48).constData());
  UDPShifts->beamHold = *(bool *)(gantryArray.remove(0, 56).constData());

  qDebug() << UDPShifts->shiftX << "" << UDPShifts->shiftY << ""
           << UDPShifts->shiftZ << "" << UDPShifts->rotationX << ""
           << UDPShifts->rotationY << "" << UDPShifts->rotationZ << ""
           << UDPShifts->gantryAngle << "" << UDPShifts->beamHold;

  //   The UDP format is [X,Y,Z,Gantry] in IEC(cm) and Varian degrees
  //   IEC to LPS conversion, simple approach as it only supports HFS
  //   orientation now
  this->shifts[0] = UDPShifts->shiftX * 10;   // cm to mm
  this->shifts[1] = -UDPShifts->shiftZ * 10;  // cm to mm
  this->shifts[2] = UDPShifts->shiftY * 10;   // cm to mm

  delete UDPShifts;
  shiftXArray.clear();
  shiftYArray.clear();
  shiftZArray.clear();
  rotXArray.clear();
  rotYArray.clear();
  rotZArray.clear();
  gantryArray.clear();
  beamHold.clear();

  this->UpdateViews();
  QApplication::processEvents();

  //    qDebug() <<"Rendering took" << timer.elapsed() << "milliseconds";
}

void UDPListener::StartListening() {
  //    //qDebug()<<"Start";
  //    //Receiver port
  //    QSettings settings("ImageX","KIMView");
  //    int KIMViewPort=settings.value("KIMViewPort").toInt();
  //    socket->bind(KIMViewPort);
  //    //KIM IP and port
  //    QString KIMIP=settings.value("KIMIP").toString();
  //    int KIMPort=settings.value("KIMPort").toInt();
  //    socket->connectToHost(QHostAddress(KIMIP),KIMPort);
  //    //socket->waitForConnected(1000);
  //    connect(socket, SIGNAL(readyRead()), this, SLOT(readMessage()));

  socket->bind(QHostAddress("127.0.0.1"), 45617);
  connect(socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
}

void UDPListener::StopListening() { socket->close(); }

void UDPListener::UpdateViews() {
  /*******************************Tracking.......................................*/
  // Remove previous last actors
  this->BEVViewer->ModelRenderer->RemoveViewProp(this->TrackingActor3D);
  this->AxialViewer->ViewRenderer->RemoveViewProp(this->TrackingActorAxial);
  this->SagittalViewer->ViewRenderer->RemoveViewProp(
      this->TrackingActorSagittal);
  this->CoronalViewer->ViewRenderer->RemoveViewProp(this->TrackingActorCoronal);

  // Transform actor
  this->TrackingTransform->Identity();
  this->TrackingTransform->Translate(this->shifts);
  this->TrackingPolydataTransform->SetTransform(this->TrackingTransform);
  this->TrackingPolydataTransform->SetInputData(this->TrackingTarget);
  this->TrackingMapper->SetInputConnection(
      this->TrackingPolydataTransform->GetOutputPort());
  this->TrackingActor3D->SetMapper(this->TrackingMapper);

  this->BEVViewer->ModelRenderer->AddViewProp(this->TrackingActor3D);
  this->BEVViewer->ModelRenderer->GetRenderWindow()->Render();

  this->TrackingPolyData->DeepCopy(
      this->TrackingPolydataTransform->GetOutput());

  // Update 2D views
  this->TrackingActorAxial = this->AxialViewer->CutROI(
      this->TrackingPolyData, this->AxialViewer->SliceLoc, 1, 1, 0,
      this->AxialViewer->SliceOrientation);
  this->TrackingActorAxial->GetProperty()->SetLineWidth(3.0);
  this->AxialViewer->ViewRenderer->AddActor(this->TrackingActorAxial);
  this->AxialViewer->ViewRenderer->GetRenderWindow()->Render();

  this->TrackingActorSagittal = this->SagittalViewer->CutROI(
      this->TrackingPolyData, this->SagittalViewer->SliceLoc, 1, 1, 0,
      this->SagittalViewer->SliceOrientation);
  this->TrackingActorSagittal->GetProperty()->SetLineWidth(3.0);
  this->SagittalViewer->ViewRenderer->AddActor(this->TrackingActorSagittal);
  this->SagittalViewer->ViewRenderer->GetRenderWindow()->Render();

  this->TrackingActorCoronal = this->CoronalViewer->CutROI(
      this->TrackingPolyData, this->CoronalViewer->SliceLoc, 1, 1, 0,
      this->CoronalViewer->SliceOrientation);
  this->TrackingActorCoronal->GetProperty()->SetLineWidth(3.0);
  this->CoronalViewer->ViewRenderer->AddActor(this->TrackingActorCoronal);
  this->CoronalViewer->ViewRenderer->GetRenderWindow()->Render();
}
