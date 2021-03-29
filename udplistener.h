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

#ifndef UDPLISTENER_H
#define UDPLISTENER_H

#include <QObject>
#include <QUdpSocket>

// Tracking function headers
#include <bevwidget.h>
#include <imageviewer2d.h>
#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

class UDPListener : public QObject {
  Q_OBJECT
public:
  explicit UDPListener(QObject *parent = nullptr);
  ~UDPListener();
  void HelloUDP();
  double shifts[3] = {0, 0, 0};

  // Tracking members
  vtkSmartPointer<vtkTransform> TrackingTransform;
  vtkSmartPointer<vtkTransformPolyDataFilter> TrackingPolydataTransform;
  vtkSmartPointer<vtkActor> TrackingActor3D;
  vtkSmartPointer<vtkPolyDataMapper> TrackingMapper;
  vtkSmartPointer<vtkActor> TrackingActorAxial;
  vtkSmartPointer<vtkActor> TrackingActorSagittal;
  vtkSmartPointer<vtkActor> TrackingActorCoronal;
  vtkSmartPointer<vtkPolyData> TrackingPolyData;
  vtkSmartPointer<vtkPolyData> TrackingTarget;
  std::vector<vtkSmartPointer<vtkPolyData>> MeshList;
  ImageViewer2D *AxialViewer;
  ImageViewer2D *CoronalViewer;
  ImageViewer2D *SagittalViewer;
  BEVWidget *BEVViewer;
  unsigned int SelectROINum;
  void UpdateViews();
  double (*ROIColors)[3];
  unsigned int selectedStructureNum;
  QList<QString> ROINames;
  bool connectionState = false; // disconnected by default

  QWidget *parent;
  struct UDPMsg {
    double shiftX = 0.0;
    double shiftY = 0.0;
    double shiftZ = 0.0;
    double rotationX = 0.0;
    double rotationY = 0.0;
    double rotationZ = 0.0;
    double gantryAngle = 0.0;
    bool beamHold = false;
  };

signals:

public slots:
  void readMessage();
  void StartListening();
  void StopListening();

private:
  QUdpSocket *socket;
};

#endif // UDPLISTENER_H
