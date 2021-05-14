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

#ifndef IMAGEVIEWER2D_H
#define IMAGEVIEWER2D_H

#define vtkRenderingCore_AUTOINIT \
  3(vtkInteractionStyle, vtkRenderingFreeType, vtkRenderingOpenGL2)

#include <QVTKWidget.h>
#include <vtkActor.h>
#include <vtkAngleWidget.h>
#include <vtkAnnotatedCubeActor.h>
#include <vtkAxesActor.h>
#include <vtkBoxWidget2.h>
#include <vtkColorSeries.h>
#include <vtkCornerAnnotation.h>
#include <vtkCursor2D.h>
#include <vtkDistanceWidget.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkImagePlaneWidget.h>
#include <vtkInteractorStyleImage.h>
#include <vtkInteractorStyleUser.h>
#include <vtkLineWidget2.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkPolyData.h>
#include <vtkRenderer.h>
#include <vtkScalarBarActor.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkTransform.h>
#include <vtkVolume.h>
#include <vtkinteractorstyleimagecustom.h>
#include <vtklinecallbackdose.h>

#include <QAction>
#include <QActionGroup>
#include <QKeyEvent>
#include <QList>
#include <QString>
#include <QWheelEvent>
#include <QWidget>
#include <vector>

namespace Ui {
class ImageViewer2D;
}

class ImageViewer2D : public QWidget {
  Q_OBJECT

 public:
  explicit ImageViewer2D(QWidget *parent = nullptr,
                         QActionGroup *contextMenus = 0);
  ~ImageViewer2D();
  void SetImageData(vtkSmartPointer<vtkImageData> Img);
  void SetRTDose(vtkSmartPointer<vtkImageData> Dose);
  void SetInteractorStyleToRegistration();
  // 0=Axial,1=Sagittal,2=Coronal
  void SetSliceOrientation(int Orientation);
  void SetUpView();
  void ShowImageAndDose(double SliceLoc);
  virtual void SliceImageAndDose(double SliceLoc);
  void ShowCursor(double *Bounds);
  void TriggerActionShowBeams();
  void TriggerActionShowDose();
  void TriggerActionShowContours();
  void TriggerReset_WL_WW();
  void DisplayROIs(double SliceLoc, int Orientation);
  void DisplayBeams(double SliceLoc, int Orientation);
  void DisplayPOIs(double SliceLoc, int Orientation);
  void DisplayScalarBar();
  void HideBeams();
  void HideContours();
  void HideDose();
  void UpdateView();
  void MoveToLocation(double loc);
  void AdjustDoseRange(double min, double max);
  void AdjustImageWLWW();


  double SliceStep;
  int SliceOrientation;                     // Axial by default
  double WindowUp = 600, WindowLow = -400;  // corresponds to WW=1000 & WL=100
  double WindowWidth = 0.0, Windowlevel = 0.0;
  double Bounds[6];
  double DoseRange[2] = {0.0, 1.0};
  double SliceLoc;
  double (*ROIColors)[3];  // 50 ROIs supported as of now
  int NumOfROIs;
  bool BeamVisibility;
  bool ContourVisibility;
  bool DoseVisibility;
  bool POIVisibility;
  bool ScalarBarVisibility;

  vtkSmartPointer<vtkImageData> ImageData;
  vtkSmartPointer<vtkImageData> RTDose;
  vtkSmartPointer<vtkCornerAnnotation> ImgPositionAnnotation;
  vtkSmartPointer<vtkCornerAnnotation> WLWAnnotation;
  vtkSmartPointer<vtkAnnotatedCubeActor> AnnotatedCube;
  vtkSmartPointer<vtkInteractorStyleUser> InteractorImage2D;
  vtkSmartPointer<vtkInteractorStyleImageCustom> InteractorImage;
  vtkSmartPointer<vtkDistanceWidget> DistanceWidget;
  vtkSmartPointer<vtkAngleWidget> AngleWidget;
  vtkSmartPointer<vtkTransform> UserTransform;
  vtkSmartPointer<vtkActor> CursorActor;
  vtkSmartPointer<vtkActor> ROIActor;
  vtkSmartPointer<vtkRenderer> ViewRenderer;
  vtkSmartPointer<vtkImageActor> ImageSlice;
  vtkSmartPointer<vtkImageActor> DoseSlice;
  vtkSmartPointer<vtkActor> CutROI(vtkPolyData *mesh, double sliceNo, double R,
                                   double G, double B, int Orientation);
  vtkSmartPointer<vtkActorCollection> ContourActors;
  vtkSmartPointer<vtkActorCollection> BeamActors;
  vtkSmartPointer<vtkActorCollection> POIActors;
  vtkSmartPointer<vtkColorSeries> BeamColors;
  vtkSmartPointer<vtkAxesActor> IsocentreAxes;
  vtkSmartPointer<vtkScalarBarActor> DoseScalarBar;
  vtkSmartPointer<vtkLookupTable> DoseLUT;
  vtkSmartPointer<vtkLineCallbackDose> lineCallbackDose;
  vtkSmartPointer<vtkLineWidget2> lineWidget;
  vtkSmartPointer<vtkScalarBarActor> ScalarBar;
  vtkSmartPointer<vtkImageReslice> ImageReslice;

  vtkSmartPointer<vtkAxesActor> Axes;
  vtkSmartPointer<vtkActor> HumanActor;
  vtkSmartPointer<vtkOrientationMarkerWidget> MarkerWidget;
  vtkSmartPointer<vtkOrientationMarkerWidget> AxesWidget;

  std::vector<vtkSmartPointer<vtkPolyData> > MeshList;
  std::vector<vtkSmartPointer<vtkPolyData> > BeamList;
  std::vector<vtkSmartPointer<vtkPolyData> > POIList;

  QList<QString> ROINames;
  QList<QString> ROITypes;
  QList<int> ROINo;

 private slots:
  void on_toolButtonSagittal_clicked();

  void on_toolButtonAxial_clicked();

  void on_toolButtonCoronal_clicked();

  void on_toolButtonScreenshot_clicked();

  void on_verticalScrollBar_valueChanged(int value);

  virtual void on_toolButtonContrast_clicked();

  void on_actionMeasureDistance_triggered();

  void on_actionMeasureAngle_triggered();

  void on_actionReset_WL_WW_triggered();

  void on_toolButtonProbe_clicked();

  void on_toolButtonScalarBar_clicked();

  void on_actionShowContours_triggered();

  void on_actionShowDose_triggered();

  void on_actionRubberBandZoom_triggered();

  void on_actionShowBeams_triggered();

  void on_actionShow_Image_Extent_triggered();

 private:
  Ui::ImageViewer2D *ui;

 protected:
  void keyPressEvent(QKeyEvent *event);
  void wheelEvent(QWheelEvent *event);
};

#endif  // IMAGEVIEWER2D_H
