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

#include "imageviewer2d.h"

#include <alignimageinteractorstyle.h>
#include <vtkActor.h>
#include <vtkActorCollection.h>
#include <vtkAngleRepresentation2D.h>
#include <vtkAngleWidget.h>
#include <vtkAnnotatedCubeActor.h>
#include <vtkAssembly.h>
#include <vtkAxesActor.h>
#include <vtkAxisActor2D.h>
#include <vtkBoxRepresentation.h>
#include <vtkBoxWidget2.h>
#include <vtkCamera.h>
#include <vtkCellPicker.h>
#include <vtkChartXY.h>
#include <vtkColorSeries.h>
#include <vtkContextScene.h>
#include <vtkContextView.h>
#include <vtkContourTriangulator.h>
#include <vtkCoordinate.h>
#include <vtkCornerAnnotation.h>
#include <vtkCursor2D.h>
#include <vtkCutter.h>
#include <vtkDataSetMapper.h>
#include <vtkDistanceRepresentation2D.h>
#include <vtkDistanceWidget.h>
#include <vtkFloatArray.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkImageMapToColors.h>
#include <vtkImageMapper3D.h>
#include <vtkImagePlaneWidget.h>
#include <vtkImageProperty.h>
#include <vtkImageReslice.h>
#include <vtkInteractorStyleImage.h>
#include <vtkInteractorStyleRubberBandZoom.h>
#include <vtkInteractorStyleSwitch.h>
#include <vtkInteractorStyleUser.h>
#include <vtkLeaderActor2D.h>
#include <vtkLineRepresentation.h>
#include <vtkLineWidget2.h>
#include <vtkLookupTable.h>
#include <vtkMatrix4x4.h>
#include <vtkNew.h>
#include <vtkObject.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkPen.h>
#include <vtkPlane.h>
#include <vtkPlot.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkProp.h>
#include <vtkProperty.h>
#include <vtkProperty2D.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkScalarBarActor.h>
#include <vtkScalarBarWidget.h>
#include <vtkScalarsToColors.h>
#include <vtkSmartPointer.h>
#include <vtkTIFFWriter.h>
#include <vtkTable.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkTransform.h>
#include <vtkVersion.h>
#include <vtkWindowToImageFilter.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkinteractorstyleimagecustom.h>
#include <vtklinecallbackdose.h>

#include <QAction>
#include <QDebug>
#include <QFileDialog>
#include <QKeyEvent>
#include <QList>
#include <QMessageBox>
#include <QPixmap>
#include <QString>
#include <QWheelEvent>
#include <QWidget>

#include "ui_imageviewer2d.h"

ImageViewer2D::ImageViewer2D(QWidget *parent, QActionGroup *contextMenus)
    : QWidget(parent), ui(new Ui::ImageViewer2D) {
  ui->setupUi(this);

  // Automoatically resolves contour/beams coincidence with image (being on the
  // same plane)
  vtkMapper::SetResolveCoincidentTopologyToPolygonOffset();

  // Add context menu
  this->ui->widget->addAction(this->ui->actionMeasureDistance);
  this->ui->widget->addAction(this->ui->actionMeasureAngle);
  this->ui->widget->addAction(this->ui->actionShowContours);
  this->ui->widget->addAction(this->ui->actionShowDose);
  this->ui->widget->addAction(this->ui->actionShowBeams);
  this->ui->widget->addAction(this->ui->actionRubberBandZoom);
  this->ui->widget->addAction(this->ui->actionReset_WL_WW);
  this->ui->widget->addAction(this->ui->actionShow_Image_Extent);

  //    for (unsigned int i=0;i<contextMenus->actions().size();i++)
  //    {
  //        this->ui->widget->addAction(contextMenus->actions()[i]);
  //    }

  this->ui->verticalScrollBar->setPageStep(10);

  //    //For microCT, assuming WL=3500 and WW=1000
  //    this->WindowLow=-1500.0;
  //    this->WindowUp=8500.0;

  this->SliceStep = 3.0; // Default slice step

  // initialize class members
  this->ImageData = vtkSmartPointer<vtkImageData>::New();
  this->RTDose = vtkSmartPointer<vtkImageData>::New();
  this->ViewRenderer = vtkSmartPointer<vtkRenderer>::New();
  this->ImageSlice = vtkSmartPointer<vtkImageActor>::New();
  this->DoseSlice = vtkSmartPointer<vtkImageActor>::New();
  this->InteractorImage2D = vtkSmartPointer<vtkInteractorStyleUser>::New();
  this->InteractorImage = vtkSmartPointer<vtkInteractorStyleImageCustom>::New();
  this->UserTransform = vtkSmartPointer<vtkTransform>::New();
  this->CursorActor = vtkSmartPointer<vtkActor>::New();
  this->ROIActor = vtkSmartPointer<vtkActor>::New();
  this->Axes = vtkSmartPointer<vtkAxesActor>::New();

  // Centre the image to the display port
  this->ViewRenderer->GetActiveCamera()->ParallelProjectionOn();

  // Initialize corner annotation
  this->ImgPositionAnnotation = vtkSmartPointer<vtkCornerAnnotation>::New();
  this->ImgPositionAnnotation->SetLinearFontScaleFactor(5.0);
  this->ImgPositionAnnotation->SetNonlinearFontScaleFactor(3.0);
  this->ImgPositionAnnotation->SetMaximumFontSize(15.0);
  this->ImgPositionAnnotation->GetTextProperty()->SetColor(1, 0.95, 0);
  this->ImgPositionAnnotation->GetTextProperty()->SetFontFamilyToTimes();

  // Initialize Window Level/Width annotation
  this->WLWAnnotation = vtkSmartPointer<vtkCornerAnnotation>::New();
  this->WLWAnnotation->SetLinearFontScaleFactor(5.0);
  this->WLWAnnotation->SetNonlinearFontScaleFactor(3.0);
  this->WLWAnnotation->SetMaximumFontSize(15.0);
  this->WLWAnnotation->GetTextProperty()->SetColor(1, 0.95, 0);
  this->WLWAnnotation->GetTextProperty()->SetFontFamilyToTimes();
  // Calculate WL/Width
  int WindowWidth = this->WindowUp - (this->WindowLow);
  int WindowLevel = (this->WindowLow + this->WindowUp) / 2;
  QString *WLW = new QString;
  WLW->append("W/L: ");
  QString str1;
  str1.setNum(WindowWidth);
  WLW->append("--");
  WLW->append("/");
  QString str2;
  str2.setNum(WindowLevel);
  WLW->append("--");
  this->WLWAnnotation->SetText(3, WLW->toLatin1().data());
  this->ViewRenderer->AddViewProp(this->WLWAnnotation);

  this->SliceOrientation = 0; // Axial by default

  // Setup renderer and interactor style (no interaction -hohoo..)
  this->ui->widget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(
      InteractorImage2D);
  this->ui->widget->GetRenderWindow()->AddRenderer(this->ViewRenderer);

  // Enable anti-aliasing
  this->ui->widget->GetRenderWindow()
      ->LineSmoothingOff(); // Off to avoid color issues with MPR ROIs
  this->ui->widget->GetRenderWindow()->PolygonSmoothingOn();
  this->ui->widget->GetRenderWindow()->PointSmoothingOff();

  //    //Initialize
  //    this->AnnotatedCube=vtkSmartPointer<vtkAnnotatedCubeActor>::New();
  //    this->AnnotatedCube->SetXPlusFaceText("L");
  //    this->AnnotatedCube->SetXMinusFaceText("R");
  //    this->AnnotatedCube->SetYPlusFaceText("A");
  //    this->AnnotatedCube->SetYMinusFaceText("P");
  //    this->AnnotatedCube->SetZPlusFaceText("H");
  //    this->AnnotatedCube->SetZMinusFaceText("F");
  //    this->AnnotatedCube->SetXFaceTextRotation(90.0);
  //    this->AnnotatedCube->SetYFaceTextRotation(180.0);
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
  vtkSmartPointer<vtkPolyData> HumanPolyData =
      vtkSmartPointer<vtkPolyData>::New();
  HumanPolyData->ShallowCopy(reader->GetOutput());
  HumanPolyData->GetPointData()->SetActiveScalars("Color");
  // Visualize
  vtkSmartPointer<vtkPolyDataMapper> IconMapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
  IconMapper->SetInputData(HumanPolyData);
  IconMapper->SetColorModeToDirectScalars();
  this->HumanActor = vtkSmartPointer<vtkActor>::New();
  this->HumanActor->SetMapper(IconMapper);

  // Initialize orientation marker widget
  this->MarkerWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
  this->MarkerWidget->SetOrientationMarker(this->HumanActor);
  this->MarkerWidget->SetViewport(0.0, 0.0, 0.15, 0.35);
  this->MarkerWidget->SetInteractor(
      this->ViewRenderer->GetRenderWindow()->GetInteractor());
  this->MarkerWidget->EnabledOn();
  this->MarkerWidget->InteractiveOff();

  // Initialize orientation marker widget forthe "boy"
  this->Axes = vtkSmartPointer<vtkAxesActor>::New();
  this->Axes->AxisLabelsOn();
  this->Axes->SetTotalLength(75, 75, 75);

  this->AxesWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
  this->AxesWidget->SetOutlineColor(0.93, 0.57, 0.13);
  this->AxesWidget->SetOrientationMarker(this->Axes);
  this->AxesWidget->SetInteractor(
      this->ViewRenderer->GetRenderWindow()->GetInteractor());
  this->AxesWidget->SetViewport(0.0, 0.7, 0.1, 1.0); //[Xmin,Ymin.Xmax,Ymax]
  this->AxesWidget->SetEnabled(1);
  this->AxesWidget->InteractiveOff();

  this->DoseLUT = vtkSmartPointer<vtkLookupTable>::New();
  this->DoseScalarBar = vtkSmartPointer<vtkScalarBarActor>::New();

  this->DoseSlice->PickableOff();

  this->ContourActors = vtkSmartPointer<vtkActorCollection>::New();
  this->BeamActors = vtkSmartPointer<vtkActorCollection>::New();
  this->POIActors = vtkSmartPointer<vtkActorCollection>::New();

  this->ScalarBar = vtkScalarBarActor::New();
  this->ScalarBar->SetWidth(0.05);
  this->ScalarBar->SetTitle("Gy");
  this->ScalarBar->GetTitleTextProperty()->SetFontFamilyToTimes();
  this->ScalarBar->GetTitleTextProperty()->SetFontSize(3);
  this->ScalarBar->SetTitleRatio(0.1);
  this->ScalarBar->SetPosition(0.95, 0.1);
  // this->scalarBar->SetMaximumNumberOfColors(5);
  // this->scalarBar->SetNumberOfLabels(5);

  this->ContourVisibility = 1;
  this->DoseVisibility = 1;
  this->POIVisibility = 1;
  this->ScalarBarVisibility = 0;

  setAcceptDrops(true);
}

ImageViewer2D::~ImageViewer2D() { delete ui; }

void ImageViewer2D::SetSliceOrientation(int Orientation) {
  this->SliceOrientation = Orientation;
  if (Orientation == 0) {
    // To make scrolling move in the direction of Foot-to-Head
    this->ui->verticalScrollBar->setInvertedAppearance(true);
  } else {
    this->ui->verticalScrollBar->setInvertedAppearance(false);
  }
}

void ImageViewer2D::SetImageData(vtkSmartPointer<vtkImageData> Img) {
  this->ImageData = Img;
  Img->GetBounds(this->Bounds);

  // iniialize ImageReslice
  this->ImageReslice = vtkSmartPointer<vtkImageReslice>::New();
  this->ImageReslice->SetInputData(this->ImageData);
  this->ImageReslice->SetOutputDimensionality(2);
  this->ImageReslice->SetInterpolationModeToCubic();
  this->ImageReslice->InterpolateOn();
}

void ImageViewer2D::SetRTDose(vtkSmartPointer<vtkImageData> Dose) {
  this->RTDose = Dose;
  this->DoseRange[0] = this->RTDose->GetScalarRange()[0];
  this->DoseRange[1] = this->RTDose->GetScalarRange()[1];
}

void ImageViewer2D::SetUpView() {
  this->ViewRenderer->GetActiveCamera()->ParallelProjectionOn();
  this->ViewRenderer->GetActiveCamera()->SetViewUp(0, -1, 0);
  this->ViewRenderer->GetActiveCamera()->SetFocalPoint(0.0, 0.0, 0.0);
  this->ViewRenderer->GetActiveCamera()->SetPosition(0.0, 0.0, -1.0);

  // Setup scroll bounds
  double *Bounds = new double[6];
  this->ImageData->GetBounds(Bounds);
  if (this->SliceOrientation == 0) // Axial
  {
    this->ui->verticalScrollBar->setMinimum(this->Bounds[4]);
    this->ui->verticalScrollBar->setMaximum(this->Bounds[5]);
    // this->setWindowTitle("Axial");

  } else if (this->SliceOrientation == 1) // Sagittal
  {
    this->ui->verticalScrollBar->setMinimum(this->Bounds[0]);
    this->ui->verticalScrollBar->setMaximum(this->Bounds[1]);
    // this->setWindowTitle("Sagittal");

  } else if (this->SliceOrientation == 2) // Coronal
  {
    this->ui->verticalScrollBar->setMinimum(this->Bounds[2]);
    this->ui->verticalScrollBar->setMaximum(this->Bounds[3]);
    // this->setWindowTitle("Coronal");
  }

  this->ui->verticalScrollBar->setValue(0);
  this->SliceLoc = 0.0;

  try {
    this->ShowImageAndDose(0.0);
    // Reset once, otherwise the camera zoom changes for every Showimage() call
    this->ViewRenderer->ResetCamera();

  } catch (...) {
    qDebug() << "Error displaying image";
  }

  delete[] Bounds;
  this->DisplayROIs(this->SliceLoc, this->SliceOrientation);

  /*Save window and level for resetting, VTK uses it own mapping so the values
  are different to this->WindowLow & this->WindowUp */
  this->Windowlevel = this->ImageSlice->GetProperty()->GetColorLevel();
  this->WindowWidth = this->ImageSlice->GetProperty()->GetColorWindow();
}

void ImageViewer2D::SliceImageAndDose(double SliceLoc) {
  int *extent = new int[6];
  double *spacing = new double[3];
  double *origin = new double[3];
  double *center = new double[3];
  this->ImageData->GetExtent(extent);
  this->ImageData->GetSpacing(spacing);
  this->ImageData->GetOrigin(origin);
  this->ImageData->GetCenter(center);

  // Matrices for axial, coronal, sagittal view orientations
  double *AxialElements = new double[16];
  AxialElements[0] = 1.0;
  AxialElements[1] = 0.0;
  AxialElements[2] = 0.0;
  AxialElements[3] = 0.0;
  AxialElements[4] = 0.0;
  AxialElements[5] = 1.0;
  AxialElements[6] = 0.0;
  AxialElements[7] = 0.0;
  AxialElements[8] = 0.0;
  AxialElements[9] = 0.0;
  AxialElements[10] = 1.0;
  AxialElements[11] = 0.0;
  AxialElements[12] = 0.0;
  AxialElements[13] = 0.0;
  AxialElements[14] = 0.0;
  AxialElements[15] = 1.0;

  double *SagittalElements = new double[16];
  SagittalElements[0] = 0.0;
  SagittalElements[1] = 0.0;
  SagittalElements[2] = -1.0;
  SagittalElements[3] = 0.0;
  SagittalElements[4] = 1.0;
  SagittalElements[5] = 0.0;
  SagittalElements[6] = 0.0;
  SagittalElements[7] = 0.0;
  SagittalElements[8] = 0.0;
  SagittalElements[9] = -1.0;
  SagittalElements[10] = 0.0;
  SagittalElements[11] = 0.0;
  SagittalElements[12] = 0.0;
  SagittalElements[13] = 0.0;
  SagittalElements[14] = 0.0;
  SagittalElements[15] = 1.0;

  double *CoronalElements = new double[16];
  CoronalElements[0] = 1.0;
  CoronalElements[1] = 0.0;
  CoronalElements[2] = 0.0;
  CoronalElements[3] = 0.0;
  CoronalElements[4] = 0.0;
  CoronalElements[5] = 0.0;
  CoronalElements[6] = 1.0;
  CoronalElements[7] = 0.0;
  CoronalElements[8] = 0.0;
  CoronalElements[9] = -1.0;
  CoronalElements[10] = 0.0;
  CoronalElements[11] = 0.0;
  CoronalElements[12] = 0.0;
  CoronalElements[13] = 0.0;
  CoronalElements[14] = 0.0;
  CoronalElements[15] = 1.0;

  // Set the slice orientation
  vtkSmartPointer<vtkMatrix4x4> resliceAxes =
      vtkSmartPointer<vtkMatrix4x4>::New();
  if (this->SliceOrientation == 0) {
    resliceAxes->DeepCopy(AxialElements);

  }

  else if (this->SliceOrientation == 1) {
    resliceAxes->DeepCopy(SagittalElements);
  }

  else if (this->SliceOrientation == 2) {
    resliceAxes->DeepCopy(CoronalElements);
  }

  switch (this->SliceOrientation) {
  case 0:
    resliceAxes->SetElement(0, 3, 0);
    resliceAxes->SetElement(1, 3, 0);
    resliceAxes->SetElement(2, 3, SliceLoc);
    break;

  case 1:
    resliceAxes->SetElement(0, 3, SliceLoc);
    resliceAxes->SetElement(1, 3, 0);
    resliceAxes->SetElement(2, 3, 0);
    break;

  case 2:
    resliceAxes->SetElement(0, 3, 0);
    resliceAxes->SetElement(1, 3, SliceLoc);
    resliceAxes->SetElement(2, 3, 0);
    break;
  }

  // Extract a slice in the desired orientation
  // Inialialize ImageReslice
  this->ImageReslice->SetResliceAxes(resliceAxes);
  this->ImageReslice->Update();

  // Create a greyscale lookup table
  vtkSmartPointer<vtkLookupTable> table =
      vtkSmartPointer<vtkLookupTable>::New();
  table->SetRange(this->WindowLow, this->WindowUp); // image intensity range
  table->SetValueRange(0.0, 1.0);                   // from black to white
  table->SetSaturationRange(0, 0);                  // no color saturation
  table->SetRampToSCurve();
  table->Build();

  // qDebug()<<table->GetNumberOfColors()<<"Colors";

  // Map the image through the lookup table
  vtkSmartPointer<vtkImageMapToColors> color =
      vtkSmartPointer<vtkImageMapToColors>::New();
  color->SetLookupTable(table);
  color->SetInputConnection(this->ImageReslice->GetOutputPort());
  color->Update();
  this->ImageSlice->GetMapper()->SetInputConnection(color->GetOutputPort());
  this->ImageSlice->SetOpacity(0.95);
  this->ImageSlice->InterpolateOn();
  //*****************End of slicing image****************

  // Extract a dose slice in the desired orientation
  vtkSmartPointer<vtkImageReslice> resliceDose =
      vtkSmartPointer<vtkImageReslice>::New();
  resliceDose->SetInputData(this->RTDose);
  resliceDose->SetOutputDimensionality(2);
  resliceDose->SetResliceAxes(resliceAxes);
  resliceDose->SetInterpolationModeToCubic();
  resliceDose->Update();

  // qDebug()<<this->RTDose->GetScalarRange()[0]<<":Min"<<this->RTDose->GetScalarRange()[1]<<":Max";

  // Dose color wash.......................................
  // Create a greyscale lookup table
  this->DoseLUT->SetRange(this->DoseRange);
  this->DoseLUT->SetValueRange(0.5, 1.0); // from black to white
  this->DoseLUT->SetSaturationRange(1.0, 1.0);
  this->DoseLUT->SetHueRange(0.667, 0.0);
  this->DoseLUT->SetAlpha(0.7);
  this->DoseLUT->SetRampToLinear();
  this->DoseLUT->SetNumberOfColors(256);
  this->DoseLUT->Build();
  this->DoseLUT->SetTableValue(0.0, 0, 0, 0,
                               0); // Makes zero values totally transparent
  this->DoseLUT->Build();

  // Map the image through the lookup table
  vtkSmartPointer<vtkImageMapToColors> colorDose =
      vtkSmartPointer<vtkImageMapToColors>::New();
  colorDose->SetLookupTable(this->DoseLUT);
  colorDose->SetInputConnection(resliceDose->GetOutputPort());
  colorDose->Update();

  // Set the dose image
  this->DoseSlice->SetInputData(colorDose->GetOutput());
  this->DoseSlice->PickableOff();

  // Toggle interpolation
  this->DoseSlice->InterpolateOn();
  // Dose is always above image
  this->DoseSlice->SetPosition(0, 0, -1);

  if (this->RTDose->GetScalarRange()[1] > 1.0) {
    this->DoseScalarBar->SetLookupTable(this->DoseLUT);
  }

  delete[] extent;
  delete[] spacing;
  delete[] origin;
  delete[] center;

  delete[] AxialElements;
  delete[] SagittalElements;
  delete[] CoronalElements;
}

void ImageViewer2D::ShowImageAndDose(double SliceLoc) {
  this->SliceImageAndDose(SliceLoc);
  this->ViewRenderer->AddViewProp(this->ImageSlice);
  if (this->DoseVisibility == 1) {
    this->ViewRenderer->AddViewProp(this->DoseSlice);
  }

  if (this->ScalarBarVisibility == 1) {
    this->ViewRenderer->AddViewProp(this->DoseScalarBar);
  }

  // Add image position annotation
  QString *leftLowerText = new QString;
  if (this->SliceOrientation == 0) {
    leftLowerText->append("Z:");
    // this->AnnotatedCube->GetAssembly()->SetOrientation(0,0,0);
    this->HumanActor->SetOrientation(0, 0, 180);
    vtkSmartPointer<vtkTransform> transform =
        vtkSmartPointer<vtkTransform>::New();
    transform->PostMultiply(); // this is the key line
    transform->RotateX(0);
    transform->RotateY(0);
    transform->RotateZ(0);
    this->Axes->SetUserTransform(transform);

  } else if (this->SliceOrientation == 1) {
    leftLowerText->append("X:");
    // this->AnnotatedCube->GetAssembly()->SetOrientation(0,90,0);
    this->HumanActor->SetOrientation(0, -90, 90);
    vtkSmartPointer<vtkTransform> transform =
        vtkSmartPointer<vtkTransform>::New();
    transform->PostMultiply(); // this is the key line
    transform->RotateX(90);
    transform->RotateY(90);
    this->Axes->SetUserTransform(transform);

  } else if (this->SliceOrientation == 2) {
    leftLowerText->append("Y:");
    // this->AnnotatedCube->GetAssembly()->SetOrientation(-90,0,0);
    this->HumanActor->SetOrientation(-90, 0, -180);
    vtkSmartPointer<vtkTransform> transform =
        vtkSmartPointer<vtkTransform>::New();
    transform->PostMultiply(); // this is the key line
    transform->RotateX(90.0);
    this->Axes->SetUserTransform(transform);
  }

  QString *SliceValue = new QString;
  SliceValue->setNum(this->ui->verticalScrollBar->value());
  leftLowerText->append(SliceValue);
  leftLowerText->append(" mm");
  this->ImgPositionAnnotation->SetText(1, leftLowerText->toLatin1().data());

  this->ViewRenderer->AddViewProp(this->ImgPositionAnnotation);
  // this->ViewRenderer->GetRenderWindow()->Render();

  this->SliceLoc = SliceLoc;
  delete SliceValue;
  delete leftLowerText;
}

void ImageViewer2D::on_toolButtonAxial_clicked() {
  // Enable scrollbar
  this->ui->verticalScrollBar->setEnabled(true);
  this->ViewRenderer->RemoveAllViewProps();
  this->SliceOrientation = 0;
  this->ui->verticalScrollBar->setMinimum(this->Bounds[4]);
  this->ui->verticalScrollBar->setMaximum(this->Bounds[5]);
  // To make scrolling move in the direction of Foot-to-Head
  this->ui->verticalScrollBar->setInvertedAppearance(true);
  this->ShowImageAndDose(double(this->ui->verticalScrollBar->value()));
  this->setWindowTitle("Axial");
  //    if(this->ui->toolButtonCursor->isChecked())
  //    {
  //        this->ShowCursor(this->ImageSlice->GetBounds());
  //    }

  if (this->ui->toolButtonProbe->isChecked()) {
    this->ui->toolButtonProbe->setChecked(false);
    this->lineCallbackDose->doseProfiler->close();
  }

  this->DisplayROIs(this->SliceLoc, this->SliceOrientation);
  this->DisplayBeams(this->SliceLoc, this->SliceOrientation);
  this->ViewRenderer
      ->ResetCamera(); // This positions the view in the centre of the screen
  this->ViewRenderer->GetRenderWindow()->Render();
  this->AxesWidget->GetInteractor()->GetRenderWindow()->Render();
}

void ImageViewer2D::on_toolButtonSagittal_clicked() {
  // Enable scrollbar
  this->ui->verticalScrollBar->setEnabled(true);
  this->ViewRenderer->RemoveAllViewProps();
  this->SliceOrientation = 1;
  this->ui->verticalScrollBar->setMinimum(this->Bounds[0]);
  this->ui->verticalScrollBar->setMaximum(this->Bounds[1]);
  this->ui->verticalScrollBar->setInvertedAppearance(false);
  this->ShowImageAndDose(double(this->ui->verticalScrollBar->value()));
  this->setWindowTitle("Sagittal");
  //    if(this->ui->toolButtonCursor->isChecked())
  //    {
  //        this->ShowCursor(this->ImageSlice->GetBounds());
  //    }

  if (this->ui->toolButtonProbe->isChecked()) {
    this->ui->toolButtonProbe->setChecked(false);
    this->lineCallbackDose->doseProfiler->close();
  }

  this->DisplayROIs(this->SliceLoc, this->SliceOrientation);
  this->DisplayBeams(this->SliceLoc, this->SliceOrientation);
  this->ViewRenderer
      ->ResetCamera(); // This position the view in the centre of the screen
  this->ViewRenderer->GetRenderWindow()->Render();
  this->AxesWidget->GetInteractor()->GetRenderWindow()->Render();
}

void ImageViewer2D::on_toolButtonCoronal_clicked() {
  // Enable scrollbar
  this->ui->verticalScrollBar->setEnabled(true);
  this->ViewRenderer->RemoveAllViewProps();
  this->SliceOrientation = 2;
  this->ui->verticalScrollBar->setMinimum(this->Bounds[2]);
  this->ui->verticalScrollBar->setMaximum(this->Bounds[3]);
  this->ui->verticalScrollBar->setInvertedAppearance(false);
  this->ShowImageAndDose(double(this->ui->verticalScrollBar->value()));
  this->setWindowTitle("Coronal");
  //    if(this->ui->toolButtonCursor->isChecked())
  //    {
  //        this->ShowCursor(this->ImageSlice->GetBounds());
  //    }

  if (this->ui->toolButtonProbe->isChecked()) {
    this->ui->toolButtonProbe->setChecked(false);
    this->lineCallbackDose->doseProfiler->close();
  }

  this->DisplayROIs(this->SliceLoc, this->SliceOrientation);
  this->DisplayBeams(this->SliceLoc, this->SliceOrientation);
  this->ViewRenderer
      ->ResetCamera(); // This position the view in the centre of the screen
  this->ViewRenderer->GetRenderWindow()->Render();
  this->AxesWidget->GetInteractor()->GetRenderWindow()->Render();
}

void ImageViewer2D::on_toolButtonScreenshot_clicked() {
  QFileDialog *FileDlg = new QFileDialog;
  QString FileName = FileDlg->getSaveFileName(this->ui->widget, "Save File",
                                              "untitled", "TIFF(*.tiff)");
  // qDebug()<<FileName;

  vtkSmartPointer<vtkWindowToImageFilter> WindowToImage =
      vtkSmartPointer<vtkWindowToImageFilter>::New();
  WindowToImage->SetInput(this->ui->widget->GetRenderWindow());
  WindowToImage->Update();

  vtkSmartPointer<vtkTIFFWriter> Writer = vtkSmartPointer<vtkTIFFWriter>::New();
  Writer->SetFileName(FileName.toLatin1());
  Writer->SetInputConnection(WindowToImage->GetOutputPort());
  Writer->Update();

  delete FileDlg;
}

void ImageViewer2D::on_verticalScrollBar_valueChanged(int value) {
  this->ViewRenderer->RemoveViewProp(this->ImageSlice);
  this->ViewRenderer->RemoveViewProp(this->DoseSlice);
  this->ShowImageAndDose(double(value));
  this->SliceLoc = value;
  this->DisplayROIs(double(value), this->SliceOrientation);
  this->DisplayBeams(double(value), this->SliceOrientation);
  this->DisplayPOIs(double(value), this->SliceOrientation);
  this->ViewRenderer->GetRenderWindow()->Render();

  if (this->ui->toolButtonProbe->isChecked()) {
    if (this->SliceOrientation == 0) // Axial
    {
      this->lineCallbackDose->trZ =
          static_cast<double>(this->ui->verticalScrollBar->value());
      this->lineWidget->InvokeEvent(vtkCommand::InteractionEvent, NULL);
    }

    else if (this->SliceOrientation == 1) // Sagittal
    {
      this->lineCallbackDose->trX =
          static_cast<double>(this->ui->verticalScrollBar->value());
      this->lineWidget->InvokeEvent(vtkCommand::InteractionEvent, NULL);
    }

    else if (this->SliceOrientation == 2) // Coronal
    {
      this->lineCallbackDose->trY =
          static_cast<double>(this->ui->verticalScrollBar->value());
      this->lineWidget->InvokeEvent(vtkCommand::InteractionEvent, NULL);
    }
  }
}

void ImageViewer2D::UpdateView() {
  this->SliceLoc = this->ui->verticalScrollBar->value();
  this->ViewRenderer->RemoveViewProp(this->ImageSlice);
  this->ViewRenderer->RemoveViewProp(this->DoseSlice);
  this->ShowImageAndDose(this->SliceLoc);
  this->DisplayROIs(this->SliceLoc, this->SliceOrientation);
  this->DisplayBeams(this->SliceLoc, this->SliceOrientation);
  this->DisplayPOIs(this->SliceLoc, this->SliceOrientation);
  this->ViewRenderer->GetRenderWindow()->Render();
}

void ImageViewer2D::MoveToLocation(double loc) {
  this->ui->verticalScrollBar->setValue(loc);
  this->ViewRenderer->RemoveActor(this->ImageSlice);
  this->ViewRenderer->RemoveActor(this->DoseSlice);
  this->ShowImageAndDose(double(loc));
  this->SliceLoc = loc;
  this->DisplayROIs(double(loc), this->SliceOrientation);
  this->DisplayBeams(double(loc), this->SliceOrientation);
  this->DisplayPOIs(double(loc), this->SliceOrientation);
  this->ViewRenderer->GetRenderWindow()->Render();
}

void ImageViewer2D::on_toolButtonContrast_clicked() {
  if (this->ui->toolButtonContrast->isChecked()) {
    this->ui->widget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(
        InteractorImage);
  }

  //    else if(!this->DoseSlice->GetPickable() &&
  //    !this->ui->toolButtonContrast->isChecked())
  //    {
  //        //Switch to image alignment interaction
  //        this->SetInteractorStyleToRegistration();
  //        //qDebug()<<"Reverting interactor style to registration";
  //    }

  else {
    this->ui->widget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(
        this->InteractorImage2D);
  }
}

// Connect ShowImage to PgUp & PgDown key presses for scrolling
void ImageViewer2D::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_PageUp) {
    if (this->SliceOrientation == 0) {
      this->ui->verticalScrollBar->setValue(
          this->ui->verticalScrollBar->value() + this->SliceStep);
    } else {
      this->ui->verticalScrollBar->setValue(
          this->ui->verticalScrollBar->value() - this->SliceStep);
    }

  } else if (event->key() == Qt::Key_PageDown) {
    if (this->SliceOrientation == 0) {
      this->ui->verticalScrollBar->setValue(
          this->ui->verticalScrollBar->value() - this->SliceStep);
    } else {
      this->ui->verticalScrollBar->setValue(
          this->ui->verticalScrollBar->value() + this->SliceStep);
    }
  }

  else if (event->key() == Qt::Key_Up) {
    if (this->SliceOrientation == 0) {
      this->ui->verticalScrollBar->setValue(
          this->ui->verticalScrollBar->value() + 1.0);
    } else {
      this->ui->verticalScrollBar->setValue(
          this->ui->verticalScrollBar->value() - 1.0);
    }
  }

  else if (event->key() == Qt::Key_Down) {
    if (this->SliceOrientation == 0) {
      this->ui->verticalScrollBar->setValue(
          this->ui->verticalScrollBar->value() - 1.0);
    } else {
      this->ui->verticalScrollBar->setValue(
          this->ui->verticalScrollBar->value() + 1.0);
    }
  }
}

void ImageViewer2D::on_actionMeasureDistance_triggered() {
  if (this->ui->actionMeasureDistance->isChecked()) {
    // Add distance widget
    this->DistanceWidget = vtkSmartPointer<vtkDistanceWidget>::New();
    this->DistanceWidget->SetInteractor(
        this->ViewRenderer->GetRenderWindow()->GetInteractor());
    this->DistanceWidget->CreateDefaultRepresentation();
    static_cast<vtkDistanceRepresentation2D *>(
        this->DistanceWidget->GetRepresentation())
        ->SetLabelFormat("%-#6.4g mm");
    vtkAxisActor2D *axis = static_cast<vtkDistanceRepresentation2D *>(
                               this->DistanceWidget->GetRepresentation())
                               ->GetAxis();
    axis->SetNumberOfMinorTicks(4.0);
    axis->SetTickLength(15);
    this->ViewRenderer->GetRenderWindow()->Render();
    this->DistanceWidget->On();
  }

  else {
    this->DistanceWidget->Off();
  }
}

void ImageViewer2D::on_actionMeasureAngle_triggered() {
  if (this->ui->actionMeasureAngle->isChecked()) {
    // Angle widget setup
    this->AngleWidget = vtkSmartPointer<vtkAngleWidget>::New();
    this->AngleWidget->SetInteractor(
        this->ui->widget->GetRenderWindow()->GetInteractor());
    this->AngleWidget->CreateDefaultRepresentation();
    static_cast<vtkAngleRepresentation2D *>(
        this->AngleWidget->GetRepresentation())
        ->SetLabelFormat("%-#3.3g Deg");
    static_cast<vtkAngleRepresentation2D *>(
        this->AngleWidget->GetRepresentation())
        ->GetRay1()
        ->GetProperty()
        ->SetColor(1, 0, 0);
    static_cast<vtkAngleRepresentation2D *>(
        this->AngleWidget->GetRepresentation())
        ->GetRay2()
        ->GetProperty()
        ->SetColor(0, 1, 0);
    static_cast<vtkAngleRepresentation2D *>(
        this->AngleWidget->GetRepresentation())
        ->GetArc()
        ->GetProperty()
        ->SetColor(0, 0, 1);
    this->ui->widget->GetRenderWindow()->Render();
    this->AngleWidget->On();
  }

  else {
    this->AngleWidget->Off();
  }
}

void ImageViewer2D::ShowCursor(double *Bounds) {
  // qDebug()<<"Running... ShowCursor";
  // Add cursor
  vtkSmartPointer<vtkCursor2D> cursor2D = vtkSmartPointer<vtkCursor2D>::New();
  cursor2D->SetModelBounds(Bounds);
  cursor2D->AllOn();
  cursor2D->TranslationModeOn();
  cursor2D->SetRadius(5.0);
  cursor2D->OutlineOn();
  cursor2D->Update();

  vtkSmartPointer<vtkPolyDataMapper> cursorMapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
  cursorMapper->SetInputConnection(cursor2D->GetOutputPort());
  cursorMapper->SetResolveCoincidentTopologyToPolygonOffset();

  this->CursorActor->GetProperty()->SetColor(1, 0, 0);
  this->CursorActor->SetMapper(cursorMapper);
  this->ViewRenderer->AddActor(this->CursorActor);
  this->ViewRenderer->GetRenderWindow()->Render();
}

void ImageViewer2D::on_actionReset_WL_WW_triggered() {
  // qDebug() << "Level: " << this->ImageSlice->GetProperty()->GetColorLevel();
  // qDebug() << "Width: " << this->ImageSlice->GetProperty()->GetColorWindow();

  // qDebug()<<"Resetting W/L";
  this->WindowLow = -400;
  this->WindowUp = 600;
  this->ViewRenderer->RemoveActor(this->ImageSlice);
  this->ViewRenderer->RemoveActor(this->DoseSlice);
  this->AdjustImageWLWW();
  this->ViewRenderer->AddActor(this->ImageSlice);
  this->ViewRenderer->AddActor(this->DoseSlice);
  //  if (this->DoseVisibility == 1) {
  //    this->ViewRenderer->AddActor(this->DoseSlice);
  //  }
  this->ViewRenderer->GetRenderWindow()->Render();
}

void ImageViewer2D::SetInteractorStyleToRegistration() {
  // Custom interaction to shift and rotate images and align
  // vtkSmartPointer<AlignImageInteractorStyle>actorStyle =
  // vtkSmartPointer<AlignImageInteractorStyle>::New();

  vtkSmartPointer<vtkInteractorStyleImage> actorStyle =
      vtkSmartPointer<vtkInteractorStyleImage>::New();
  this->ViewRenderer->GetRenderWindow()->GetInteractor()->SetInteractorStyle(
      actorStyle);
}

vtkSmartPointer<vtkActor> ImageViewer2D::CutROI(vtkPolyData *mesh,
                                                double sliceNo, double R,
                                                double G, double B,
                                                int Orientation) {
  // Create a plane to cut
  vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
  switch (Orientation) {
  case 0: // axial
    plane->SetOrigin(0, 0, sliceNo);
    plane->SetNormal(0, 0, 1);
    break;
  case 1: // sagittal
    plane->SetOrigin(sliceNo, 0, 0);
    plane->SetNormal(1, 0, 0);
    break;
  case 2: // coronal
    plane->SetOrigin(0, sliceNo, 1);
    plane->SetNormal(0, 1, 0);
    break;
  }

  // Create cutter
  vtkSmartPointer<vtkCutter> cutter = vtkSmartPointer<vtkCutter>::New();
  cutter->SetCutFunction(plane);
  cutter->SetInputData(mesh);
  cutter->Update();

  vtkSmartPointer<vtkPolyDataMapper> cutterMapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
  cutterMapper->SetResolveCoincidentTopologyToPolygonOffset();
  cutterMapper->SetInputConnection(cutter->GetOutputPort());
  cutterMapper->ScalarVisibilityOff();
  // cutterMapper->ImmediateModeRenderingOn();
  cutterMapper->Update();

  //  /*Turn on for filled contours*/
  //  vtkSmartPointer<vtkContourTriangulator> poly =
  //      vtkSmartPointer<vtkContourTriangulator>::New();
  //  poly->SetInputConnection(cutter->GetOutputPort());

  //  vtkSmartPointer<vtkDataSetMapper> cutterMapper =
  //      vtkSmartPointer<vtkDataSetMapper>::New();
  //  cutterMapper->SetInputConnection(poly->GetOutputPort());
  //  cutterMapper->ScalarVisibilityOff();
  //  cutterMapper->Update();

  // Create ROI actor
  vtkSmartPointer<vtkActor> cutROI = vtkSmartPointer<vtkActor>::New();
  cutROI->GetProperty()->SetColor(R, G, B);
  // qDebug()<<R << G<< B;
  cutROI->SetMapper(cutterMapper);
  cutROI->GetProperty()->SetDiffuse(0.3);
  cutROI->GetProperty()->SetSpecular(0.3);
  cutROI->GetProperty()->SetSpecularPower(5);
  cutROI->GetProperty()->SetInterpolationToPhong();
  cutROI->GetProperty()->SetLineWidth(1.5);
  cutROI->GetProperty()->SetAmbient(0.6);
  cutROI->DragableOff();

  // Axial
  if (Orientation == 0) {
    cutROI->SetPosition(0, 0, -sliceNo);

  }

  // Sagittal
  else if (Orientation == 1) {
    cutROI->RotateX(90);
    cutROI->RotateZ(270);
    cutROI->SetPosition(0, 0, sliceNo);

  }

  // Coronal
  else if (Orientation == 2) {
    cutROI->RotateX(90);
    cutROI->SetPosition(0, 0, -sliceNo);
  }

  return cutROI;
}

void ImageViewer2D::DisplayROIs(double SliceLoc, int Orientation) {
  // qDebug()<<this->MeshList.size()<<"Meshes";
  // Cut all actors at the given location with preferred orientation
  this->ContourActors->InitTraversal();
  for (vtkIdType i = 0; i < this->ContourActors->GetNumberOfItems(); i++) {
    this->ViewRenderer->RemoveViewProp(this->ContourActors->GetNextItem());
  }
  this->ContourActors->RemoveAllItems();

  if (this->ContourVisibility == 1) {
    for (unsigned int i = 0; i < this->MeshList.size(); i++) {
      double R = this->ROIColors[i][0] / 255.0;
      double G = this->ROIColors[i][1] / 255.0;
      double B = this->ROIColors[i][2] / 255.0;

      vtkSmartPointer<vtkActor> ROIActor = vtkSmartPointer<vtkActor>::New();
      // qDebug()<<R<<G<<B<<"CutROI color";
      ROIActor =
          this->CutROI(this->MeshList[i], SliceLoc, R, G, B, Orientation);
      this->ContourActors->AddItem(ROIActor);
      this->ViewRenderer->AddActor(this->ContourActors->GetLastItem());
    }
  }

  // this->ViewRenderer->GetRenderWindow()->Render();
}

void ImageViewer2D::DisplayBeams(double SliceLoc, int Orientation) {
  // qDebug()<<this->BeamList.size()<<"Beams";
  // Cut all actors at the given location with preferred orientation
  this->BeamActors->InitTraversal();
  for (vtkIdType i = 0; i < this->BeamActors->GetNumberOfItems(); i++) {
    this->ViewRenderer->RemoveViewProp(this->BeamActors->GetNextItem());
  }
  this->BeamActors->RemoveAllItems();

  if (this->BeamVisibility == 1) {
    for (int i = 0; i < this->BeamList.size(); i++) {
      //            //Zero index color is black, so start from 1
      //            double R=this->BeamColors->GetColor(i+1)[0]/255.0;
      //            double G=this->BeamColors->GetColor(i+1)[1]/255.0;
      //            double B=this->BeamColors->GetColor(i+1)[2]/255.0;

      vtkSmartPointer<vtkActor> BeamActor = vtkSmartPointer<vtkActor>::New();
      BeamActor =
          this->CutROI(this->BeamList[i], SliceLoc, 1.0, 0.5, 0.0, Orientation);
      BeamActor->GetProperty()->SetLineWidth(1.0);
      this->BeamActors->AddItem(BeamActor);
      this->ViewRenderer->AddActor(this->BeamActors->GetLastItem());
    }
  }

  // this->ViewRenderer->GetRenderWindow()->Render();
}

void ImageViewer2D::DisplayPOIs(double SliceLoc, int Orientation) {
  this->POIActors->InitTraversal();
  for (vtkIdType i = 0; i < this->POIActors->GetNumberOfItems(); i++) {
    this->ViewRenderer->RemoveViewProp(this->POIActors->GetNextItem());
  }
  this->POIActors->RemoveAllItems();

  if (this->POIVisibility == 1) {
    for (int i = 0; i < this->POIList.size(); i++) {
      vtkSmartPointer<vtkActor> POIActor = vtkSmartPointer<vtkActor>::New();
      POIActor =
          this->CutROI(this->POIList[i], SliceLoc, 1.0, 1.0, 0.0, Orientation);
      POIActor->GetProperty()->SetLineWidth(5.0);
      POIActor->GetProperty()->SetEdgeColor(1, 0, 0);
      this->POIActors->AddItem(POIActor);
      this->ViewRenderer->AddActor(this->POIActors->GetLastItem());
    }
  }

  // this->ViewRenderer->GetRenderWindow()->Render();
}

void ImageViewer2D::HideBeams() {
  this->BeamVisibility = 0;
  this->BeamActors->InitTraversal();
  for (vtkIdType i = 0; i < this->BeamActors->GetNumberOfItems(); i++) {
    this->ViewRenderer->RemoveViewProp(this->BeamActors->GetNextItem());
  }
  this->BeamActors->RemoveAllItems();
  this->ViewRenderer->GetRenderWindow()->Render();
}

void ImageViewer2D::HideContours() {
  this->ContourVisibility = 0;
  this->ContourActors->InitTraversal();
  for (vtkIdType i = 0; i < this->ContourActors->GetNumberOfItems(); i++) {
    this->ViewRenderer->RemoveViewProp(this->ContourActors->GetNextItem());
  }
  this->ContourActors->RemoveAllItems();
  this->ViewRenderer->GetRenderWindow()->Render();
}

void ImageViewer2D::HideDose() {
  this->DoseVisibility = 0;
  this->ViewRenderer->RemoveViewProp(this->DoseSlice);
  this->ViewRenderer->GetRenderWindow()->Render();
}

void ImageViewer2D::DisplayScalarBar() {
  this->ScalarBar->SetLookupTable(this->DoseLUT);
  this->ViewRenderer->AddActor(this->ScalarBar);
}

void ImageViewer2D::wheelEvent(QWheelEvent *event) {
  double angleDelta = event->angleDelta().y();
  if (!this->ui->toolButtonContrast->isChecked()) {
    if (angleDelta >= 0) {
      this->ui->verticalScrollBar->setValue(
          this->ui->verticalScrollBar->value() + 1);
    } else {
      this->ui->verticalScrollBar->setValue(
          this->ui->verticalScrollBar->value() - 1);
    }
  }
  event->accept();
}

void ImageViewer2D::on_toolButtonProbe_clicked() {
  if (this->RTDose->GetScalarRange()[1] > 1.0) {
    if (this->ui->toolButtonProbe->isChecked()) {
      this->lineWidget = vtkSmartPointer<vtkLineWidget2>::New();
      // Dose plotter
      this->lineCallbackDose = new vtkLineCallbackDose(this);
      this->lineCallbackDose->doseProfiler->setWindowTitle("Profile");

      this->lineWidget->SetInteractor(
          this->ViewRenderer->GetRenderWindow()->GetInteractor());
      this->lineWidget->CreateDefaultRepresentation();

      this->lineWidget->GetLineRepresentation()
          ->GetEndPointProperty()
          ->SetColor(0, 255, 0); // starting point green
      this->lineWidget->GetLineRepresentation()
          ->GetEndPoint2Property()
          ->SetColor(255, 0, 0); // starting point red

      static_cast<vtkLineRepresentation *>(
          this->lineWidget->GetRepresentation())
          ->SetLineColor(1, 1, 1);
      static_cast<vtkLineRepresentation *>(
          this->lineWidget->GetRepresentation())
          ->GetLineProperty()
          ->SetLineWidth(0.25);
      static_cast<vtkLineRepresentation *>(
          this->lineWidget->GetRepresentation())
          ->GetLineProperty()
          ->SetAmbient(0.0);
      static_cast<vtkLineRepresentation *>(
          this->lineWidget->GetRepresentation())
          ->SetResolution(1000);
      static_cast<vtkLineRepresentation *>(
          this->lineWidget->GetRepresentation())
          ->GetLineProperty()
          ->SetOpacity(1.0);

      // z=-5 so the widget always stays on top of the slice
      double lineBds[6] = {-50, 50, 0, 0, -5, -5}; // initial length to 10cm
      static_cast<vtkLineRepresentation *>(
          this->lineWidget->GetRepresentation())
          ->PlaceWidget(lineBds);

      if (this->SliceOrientation == 0) // Axial
      {
        this->lineCallbackDose->SliceOrientation = 0;
        this->lineCallbackDose->trZ =
            static_cast<double>(this->ui->verticalScrollBar->value());
      }

      else if (this->SliceOrientation == 1) // Sagittal
      {
        this->lineCallbackDose->SliceOrientation = 1;
        this->lineCallbackDose->trX =
            static_cast<double>(this->ui->verticalScrollBar->value());

      }

      else if (this->SliceOrientation == 2) // Coronal
      {
        this->lineCallbackDose->SliceOrientation = 2;
        this->lineCallbackDose->trY =
            static_cast<double>(this->ui->verticalScrollBar->value());
      }

      this->lineWidget->GetRepresentation()->SetHandleSize(5);
      this->lineCallbackDose->dose = this->RTDose;
      this->lineWidget->AddObserver(vtkCommand::InteractionEvent,
                                    this->lineCallbackDose);
      this->ViewRenderer->GetRenderWindow()->Render();
      this->lineWidget->On();

      this->lineCallbackDose->doseProfiler->show();

    }

    else {
      this->lineWidget->Off();
      this->lineCallbackDose->doseProfiler->close();
      this->ViewRenderer->GetRenderWindow()->Render(); // update renderer
    }
  } else {
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setText("Dose not available!");
    msgBox->setIcon(QMessageBox::Critical);
    msgBox->setWindowTitle("Error");
    msgBox->exec();
    delete msgBox;
    this->ui->toolButtonProbe->setChecked(false);
  }
}

void ImageViewer2D::on_toolButtonScalarBar_clicked() {
  if (this->ui->toolButtonScalarBar->isChecked()) {
    if (this->RTDose->GetScalarRange()[1] == 1.0) {
      QMessageBox *msgBox = new QMessageBox(this);
      msgBox->setText("Dose not available.");
      msgBox->setIcon(QMessageBox::Critical);
      msgBox->setWindowTitle("Error");
      msgBox->exec();
      delete msgBox;
      this->ui->toolButtonScalarBar->setChecked(false);
    } else {
      this->DisplayScalarBar();
    }

  }

  else {
    this->ViewRenderer->RemoveViewProp(this->ScalarBar);
  }

  this->UpdateView();
}

void ImageViewer2D::on_actionShowContours_triggered() {
  if (this->ui->actionShowContours->isChecked()) {
    this->ContourVisibility = 1;
    this->DisplayROIs(this->SliceLoc, this->SliceOrientation);
    this->ViewRenderer->GetRenderWindow()->Render();
  }

  else {
    this->ContourVisibility = 0;
    this->HideContours();
    this->ViewRenderer->GetRenderWindow()->Render();
  }
}

void ImageViewer2D::on_actionShowDose_triggered() {
  if (this->ui->actionShowDose->isChecked()) {
    this->DoseVisibility = 1;
    this->ViewRenderer->AddViewProp(this->DoseSlice);
    // this->ViewRenderer->GetRenderWindow()->Render();
    this->UpdateView();

  }

  else {
    this->DoseVisibility = 0;
    this->ViewRenderer->RemoveViewProp(this->DoseSlice);
    // this->ViewRenderer->GetRenderWindow()->Render();
    this->UpdateView();
  }
}

void ImageViewer2D::on_actionRubberBandZoom_triggered() {
  if (this->ui->actionRubberBandZoom->isChecked()) {
    vtkSmartPointer<vtkInteractorStyleRubberBandZoom> rubberStyleZoom =
        vtkSmartPointer<vtkInteractorStyleRubberBandZoom>::New();
    rubberStyleZoom->SetPickColor(255, 0, 0);
    this->ViewRenderer->GetRenderWindow()->GetInteractor()->SetInteractorStyle(
        rubberStyleZoom);
    this->ViewRenderer->GetRenderWindow()->GetInteractor()->Start();

  }

  else {
    this->ui->widget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(
        this->InteractorImage2D);
    this->ViewRenderer->ResetCamera();
    this->ViewRenderer->GetRenderWindow()->Render();
  }
}

void ImageViewer2D::on_actionShowBeams_triggered() {
  if (this->ui->actionShowBeams->isChecked()) {
    this->BeamVisibility = 1;
    this->DisplayBeams(this->SliceLoc, this->SliceOrientation);
    this->ViewRenderer->GetRenderWindow()->Render();
  }

  else {
    this->BeamVisibility = 0;
    this->HideBeams();
    this->ViewRenderer->GetRenderWindow()->Render();
  }
}

void ImageViewer2D::TriggerActionShowBeams() {
  this->ui->actionShowBeams->trigger();
}

void ImageViewer2D::TriggerActionShowDose() {
  this->ui->actionShowDose->trigger();
}

void ImageViewer2D::TriggerActionShowContours() {
  this->ui->actionShowContours->trigger();
}

void ImageViewer2D::TriggerReset_WL_WW() {
  this->ui->actionReset_WL_WW->trigger();
}

void ImageViewer2D::AdjustDoseRange(double min, double max) {
  this->DoseRange[0] = min;
  this->DoseRange[1] = max;

  // Remove old dose slcie
  this->ViewRenderer->RemoveViewProp(this->DoseSlice);

  // Slice dose
  int *extent = new int[6];
  double *spacing = new double[3];
  double *origin = new double[3];
  double *center = new double[3];
  this->ImageData->GetExtent(extent);
  this->ImageData->GetSpacing(spacing);
  this->ImageData->GetOrigin(origin);
  this->ImageData->GetCenter(center);

  // Matrices for axial, coronal, sagittal view orientations
  double *AxialElements = new double[16];
  AxialElements[0] = 1.0;
  AxialElements[1] = 0.0;
  AxialElements[2] = 0.0;
  AxialElements[3] = 0.0;
  AxialElements[4] = 0.0;
  AxialElements[5] = 1.0;
  AxialElements[6] = 0.0;
  AxialElements[7] = 0.0;
  AxialElements[8] = 0.0;
  AxialElements[9] = 0.0;
  AxialElements[10] = 1.0;
  AxialElements[11] = 0.0;
  AxialElements[12] = 0.0;
  AxialElements[13] = 0.0;
  AxialElements[14] = 0.0;
  AxialElements[15] = 1.0;

  double *SagittalElements = new double[16];
  SagittalElements[0] = 0.0;
  SagittalElements[1] = 0.0;
  SagittalElements[2] = -1.0;
  SagittalElements[3] = 0.0;
  SagittalElements[4] = 1.0;
  SagittalElements[5] = 0.0;
  SagittalElements[6] = 0.0;
  SagittalElements[7] = 0.0;
  SagittalElements[8] = 0.0;
  SagittalElements[9] = -1.0;
  SagittalElements[10] = 0.0;
  SagittalElements[11] = 0.0;
  SagittalElements[12] = 0.0;
  SagittalElements[13] = 0.0;
  SagittalElements[14] = 0.0;
  SagittalElements[15] = 1.0;

  double *CoronalElements = new double[16];
  CoronalElements[0] = 1.0;
  CoronalElements[1] = 0.0;
  CoronalElements[2] = 0.0;
  CoronalElements[3] = 0.0;
  CoronalElements[4] = 0.0;
  CoronalElements[5] = 0.0;
  CoronalElements[6] = 1.0;
  CoronalElements[7] = 0.0;
  CoronalElements[8] = 0.0;
  CoronalElements[9] = -1.0;
  CoronalElements[10] = 0.0;
  CoronalElements[11] = 0.0;
  CoronalElements[12] = 0.0;
  CoronalElements[13] = 0.0;
  CoronalElements[14] = 0.0;
  CoronalElements[15] = 1.0;

  // Set the slice orientation
  vtkSmartPointer<vtkMatrix4x4> resliceAxes =
      vtkSmartPointer<vtkMatrix4x4>::New();
  if (this->SliceOrientation == 0) {
    resliceAxes->DeepCopy(AxialElements);

  }

  else if (this->SliceOrientation == 1) {
    resliceAxes->DeepCopy(SagittalElements);
  }

  else if (this->SliceOrientation == 2) {
    resliceAxes->DeepCopy(CoronalElements);
  }

  switch (this->SliceOrientation) {
  case 0:
    resliceAxes->SetElement(0, 3, 0);
    resliceAxes->SetElement(1, 3, 0);
    resliceAxes->SetElement(2, 3, this->SliceLoc);
    break;

  case 1:
    resliceAxes->SetElement(0, 3, this->SliceLoc);
    resliceAxes->SetElement(1, 3, 0);
    resliceAxes->SetElement(2, 3, 0);
    break;

  case 2:
    resliceAxes->SetElement(0, 3, 0);
    resliceAxes->SetElement(1, 3, this->SliceLoc);
    resliceAxes->SetElement(2, 3, 0);
    break;
  }

  // Extract a dose slice in the desired orientation
  vtkSmartPointer<vtkImageReslice> resliceDose =
      vtkSmartPointer<vtkImageReslice>::New();
  resliceDose->SetInputData(this->RTDose);
  resliceDose->SetOutputDimensionality(2);
  resliceDose->SetResliceAxes(resliceAxes);
  resliceDose->SetInterpolationModeToCubic();
  resliceDose->Update();

  // qDebug()<<this->RTDose->GetScalarRange()[0]<<":Min"<<this->RTDose->GetScalarRange()[1]<<":Max";

  // Dose color wash.......................................
  // Create a greyscale lookup table
  this->DoseLUT->SetRange(this->DoseRange);
  this->DoseLUT->SetValueRange(0.5, 1.0); // from black to white
  this->DoseLUT->SetSaturationRange(1.0, 1.0);
  this->DoseLUT->SetHueRange(0.667, 0.0);
  this->DoseLUT->SetAlpha(0.7);
  this->DoseLUT->SetRampToLinear();
  this->DoseLUT->SetNumberOfColors(256);
  this->DoseLUT->Build();
  this->DoseLUT->SetTableValue(0.0, 0, 0, 0,
                               0); // Makes zero values totally transparent
  this->DoseLUT->Build();

  // Map the image through the lookup table
  vtkSmartPointer<vtkImageMapToColors> colorDose =
      vtkSmartPointer<vtkImageMapToColors>::New();
  colorDose->SetLookupTable(this->DoseLUT);
  colorDose->SetInputConnection(resliceDose->GetOutputPort());
  colorDose->Update();

  // Set the dose image
  this->DoseSlice->SetInputData(colorDose->GetOutput());
  this->DoseSlice->PickableOff();

  // Toggle interpolation
  this->DoseSlice->InterpolateOn();

  if (this->RTDose->GetScalarRange()[1] > 1.0) {
    this->DoseScalarBar->SetLookupTable(this->DoseLUT);
  }

  delete[] extent;
  delete[] spacing;
  delete[] origin;
  delete[] center;

  delete[] AxialElements;
  delete[] SagittalElements;
  delete[] CoronalElements;

  if (this->DoseVisibility == 1) {
    this->ViewRenderer->AddViewProp(this->DoseSlice);
  }

  if (this->ScalarBarVisibility == 1) {
    this->ViewRenderer->AddViewProp(this->DoseScalarBar);
  }

  this->ViewRenderer->GetRenderWindow()->Render();
}

void ImageViewer2D::on_actionShow_Image_Extent_triggered() {
  if (this->ui->actionShow_Image_Extent->isChecked()) {
    this->ShowCursor(this->ImageSlice->GetBounds());

  }

  else {
    this->ViewRenderer->RemoveActor(this->CursorActor);
    this->ViewRenderer->GetRenderWindow()->Render();
  }
}

void ImageViewer2D::AdjustImageWLWW() {
  int *extent = new int[6];
  double *spacing = new double[3];
  double *origin = new double[3];
  double *center = new double[3];
  this->ImageData->GetExtent(extent);
  this->ImageData->GetSpacing(spacing);
  this->ImageData->GetOrigin(origin);
  this->ImageData->GetCenter(center);

  // Matrices for axial, coronal, sagittal view orientations
  double *AxialElements = new double[16];
  AxialElements[0] = 1.0;
  AxialElements[1] = 0.0;
  AxialElements[2] = 0.0;
  AxialElements[3] = 0.0;
  AxialElements[4] = 0.0;
  AxialElements[5] = 1.0;
  AxialElements[6] = 0.0;
  AxialElements[7] = 0.0;
  AxialElements[8] = 0.0;
  AxialElements[9] = 0.0;
  AxialElements[10] = 1.0;
  AxialElements[11] = 0.0;
  AxialElements[12] = 0.0;
  AxialElements[13] = 0.0;
  AxialElements[14] = 0.0;
  AxialElements[15] = 1.0;

  double *SagittalElements = new double[16];
  SagittalElements[0] = 0.0;
  SagittalElements[1] = 0.0;
  SagittalElements[2] = -1.0;
  SagittalElements[3] = 0.0;
  SagittalElements[4] = 1.0;
  SagittalElements[5] = 0.0;
  SagittalElements[6] = 0.0;
  SagittalElements[7] = 0.0;
  SagittalElements[8] = 0.0;
  SagittalElements[9] = -1.0;
  SagittalElements[10] = 0.0;
  SagittalElements[11] = 0.0;
  SagittalElements[12] = 0.0;
  SagittalElements[13] = 0.0;
  SagittalElements[14] = 0.0;
  SagittalElements[15] = 1.0;

  double *CoronalElements = new double[16];
  CoronalElements[0] = 1.0;
  CoronalElements[1] = 0.0;
  CoronalElements[2] = 0.0;
  CoronalElements[3] = 0.0;
  CoronalElements[4] = 0.0;
  CoronalElements[5] = 0.0;
  CoronalElements[6] = 1.0;
  CoronalElements[7] = 0.0;
  CoronalElements[8] = 0.0;
  CoronalElements[9] = -1.0;
  CoronalElements[10] = 0.0;
  CoronalElements[11] = 0.0;
  CoronalElements[12] = 0.0;
  CoronalElements[13] = 0.0;
  CoronalElements[14] = 0.0;
  CoronalElements[15] = 1.0;

  // Set the slice orientation
  vtkSmartPointer<vtkMatrix4x4> resliceAxes =
      vtkSmartPointer<vtkMatrix4x4>::New();
  if (this->SliceOrientation == 0) {
    resliceAxes->DeepCopy(AxialElements);

  }

  else if (this->SliceOrientation == 1) {
    resliceAxes->DeepCopy(SagittalElements);
  }

  else if (this->SliceOrientation == 2) {
    resliceAxes->DeepCopy(CoronalElements);
  }

  switch (this->SliceOrientation) {
  case 0:
    resliceAxes->SetElement(0, 3, 0);
    resliceAxes->SetElement(1, 3, 0);
    resliceAxes->SetElement(2, 3, this->SliceLoc);
    break;

  case 1:
    resliceAxes->SetElement(0, 3, this->SliceLoc);
    resliceAxes->SetElement(1, 3, 0);
    resliceAxes->SetElement(2, 3, 0);
    break;

  case 2:
    resliceAxes->SetElement(0, 3, 0);
    resliceAxes->SetElement(1, 3, this->SliceLoc);
    resliceAxes->SetElement(2, 3, 0);
    break;
  }

  // Extract a slice in the desired orientation
  // Inialialize ImageReslice
  this->ImageReslice->SetResliceAxes(resliceAxes);
  this->ImageReslice->Update();

  // Create a greyscale lookup table
  vtkSmartPointer<vtkLookupTable> table =
      vtkSmartPointer<vtkLookupTable>::New();
  table->SetRange(this->WindowLow, this->WindowUp); // image intensity range
  table->SetValueRange(0.0, 1.0);                   // from black to white
  table->SetSaturationRange(0, 0);                  // no color saturation
  table->SetRampToSCurve();
  table->Build();

  // Map the image through the lookup table
  vtkSmartPointer<vtkImageMapToColors> color =
      vtkSmartPointer<vtkImageMapToColors>::New();
  color->SetLookupTable(table);
  color->SetInputConnection(this->ImageReslice->GetOutputPort());
  color->Update();

  this->ImageSlice = vtkSmartPointer<vtkImageActor>::New();
  this->ImageSlice->GetMapper()->SetInputConnection(color->GetOutputPort());
  this->ImageSlice->SetOpacity(0.95);
  this->ImageSlice->InterpolateOn();
  //*****************End of slicing image****************

  //  this->ViewRenderer->AddViewProp(this->ImageSlice);
  //  this->ViewRenderer->GetRenderWindow()->Render();

  delete[] extent;
  delete[] spacing;
  delete[] origin;
  delete[] center;

  delete[] AxialElements;
  delete[] SagittalElements;
  delete[] CoronalElements;
}
