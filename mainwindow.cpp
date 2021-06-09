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

#include "mainwindow.h"

#include <QVTKWidget.h>
#include <gdcmAttribute.h>
#include <gdcmDataElement.h>
#include <gdcmItem.h>
#include <gdcmNestedModuleEntries.h>
#include <gdcmTag.h>
#include <itkCastImageFilter.h>
#include <itkCommand.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImage.h>
#include <itkImageSeriesReader.h>
#include <itkImageToVTKImageFilter.h>
#include <itkMetaDataDictionary.h>
#include <itkMetaDataObject.h>
#include <itkMinimumMaximumImageFilter.h>
#include <itkNumericSeriesFileNames.h>
#include <itkSmartPointer.h>
#include <itkVersion.h>
#include <omp.h>
#include <stdlib.h>
#include <vtkActorCollection.h>
#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkConeSource.h>
#include <vtkDICOMImageReader.h>
#include <vtkFileOutputWindow.h>
#include <vtkImageShiftScale.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkOutlineFilter.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTriangleFilter.h>
#include <vtkXMLImageDataWriter.h>

#include <QCloseEvent>
#include <QDebug>
#include <QDir>
#include <QDockWidget>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QFileSystemWatcher>
#include <QIcon>
#include <QInputDialog>
#include <QList>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSettings>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTime>
#include <QTimer>
#include <QTreeWidgetItem>
#include <QUdpSocket>
#include <iostream>
#include <itksys/SystemTools.hxx>

#include "aboutdialog.h"
#include "createobjects.h"
#include "dvhdialog.h"
#include "imageviewer2d.h"
#include "ipconfigdialog.h"
#include "meshreader.h"
#include "planreader.h"
#include "rangesliderdialog.h"
#include "rtstructreaderdialog.h"
#include "udplistener.h"
#include "ui_mainwindow.h"
#include "wlwwdialog.h"

// Testing
#include <itkCenteredEuler3DTransform.h>
#include <itkRayCastInterpolateImageFunction.h>
#include <itkResampleImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkVTKImageToImageFilter.h>
#include <vtkArcSource.h>
#include <vtkArrowSource.h>
#include <vtkAssembly.h>
#include <vtkConeSource.h>
#include <vtkGlyph3D.h>
#include <vtkImageMapToColors.h>
#include <vtkLineSource.h>
#include <vtkLookupTable.h>
#include <vtkMinimalStandardRandomSequence.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolarAxesActor.h>
#include <vtkTransform.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  // Make tabel widget read only
  this->ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

  // Avoid error window popping
  vtkSmartPointer<vtkFileOutputWindow> fileOutputWindow =
      vtkSmartPointer<vtkFileOutputWindow>::New();
  fileOutputWindow->SetFileName("ErrorLog.txt");
  vtkOutputWindow *outputWindow = vtkOutputWindow::GetInstance();
  if (outputWindow) {
    outputWindow->SetInstance(fileOutputWindow);
  }

  this->BeamAngles.clear();

  this->ContextMenus = new QActionGroup(this);
  this->ContextMenus->setEnabled(true);
  this->ContextMenus->setExclusive(false);

  this->ContextMenus->addAction(this->ui->actionBEV);
  this->ContextMenus->addAction(this->ui->action3DView);

  this->RTDose = vtkSmartPointer<vtkImageData>::New();

  this->ui->treeWidget->expandAll();
  this->ui->statusBar->showMessage("No data available to display");

  this->CTImage = vtkSmartPointer<vtkImageData>::New();

  this->MeshActors = vtkSmartPointer<vtkActorCollection>::New();
  this->BeamActors = vtkSmartPointer<vtkActorCollection>::New();

  // Setup BEV widget
  this->BEVViewer = new BEVWidget(this->ui->mdiAreaView, this->ContextMenus);
  // this->ui->mdiAreaView->addSubWindow(this->BEVViewer,Qt::WindowMinMaxButtonsHint|Qt::WindowTitleHint|Qt::FramelessWindowHint);//For
  // frameless window
  this->ui->mdiAreaView->addSubWindow(
      this->BEVViewer, Qt::WindowMaximizeButtonHint | Qt::WindowTitleHint);
  this->BEVViewer->setWindowTitle("Model");

  // By default plan information dock widget is hidden
  this->ui->dockWidget_2->setVisible(false);
  this->ui->mdiAreaView->tileSubWindows();

  // Setup file watcher
  this->fileWatcher.addPath(
      "D:\\Projects\\build-KIMView-Desktop_Qt_5_15_1_MSVC2019_64bit-"
      "Release\\Doses");

  connect(&this->fileWatcher, SIGNAL(directoryChanged(QString)), this,
          SLOT(updateDose(QString)));

  // this->arc1 = vtkSmartPointer<vtkAssembly>::New();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionCT_triggered() {
  const unsigned int InputDimension = 3;
  typedef signed short PixelType;
  typedef itk::Image<PixelType, InputDimension> InputImageType;
  typedef itk::ImageSeriesReader<InputImageType> ReaderType;
  typedef itk::GDCMImageIO ImageIOType;
  typedef itk::GDCMSeriesFileNames InputNamesGeneratorType;
  ImageIOType::Pointer gdcmIO = ImageIOType::New();
  ReaderType::Pointer reader = ReaderType::New();

  try {
    QString imageDirName = QFileDialog::getExistingDirectory(
        this, "Open CT Folder", "D:\\DICOM Test Patients");

    // Start reading DICOM CT data with Phase info
    /********************************************************************************/

    InputNamesGeneratorType::Pointer inputNames =
        InputNamesGeneratorType::New();
    inputNames->SetInputDirectory(imageDirName.toLatin1().data());
    gdcmIO->SetLoadPrivateTags(true);
    gdcmIO->SetLoadSequences(true);

    const ReaderType::FileNamesContainer &filenames =
        inputNames->GetInputFileNames();
    reader->SetImageIO(gdcmIO);
    reader->SetFileNames(filenames);
    reader->Update();

    // qDebug()<<"Reading done!";
    using DictionaryType = itk::MetaDataDictionary;
    typedef itk::MetaDataObject<std::string> MetaDataStringType;
    const DictionaryType &dictionary = gdcmIO->GetMetaDataDictionary();

    // Read Patient Position tag
    std::string PatientPosition = "0018|5100";
    auto tagItr1 = dictionary.Find(PatientPosition);
    MetaDataStringType::ConstPointer PatientPositionValue =
        dynamic_cast<const MetaDataStringType *>(tagItr1->second.GetPointer());
    std::string val1;
    val1 = PatientPositionValue->GetMetaDataObjectValue();
    // qDebug()<<val1.c_str()<<" "<<val1.compare("HFS")<<" Comaprarison";
    QString curIOP;
    curIOP = QString::fromStdString(val1);

    //        //Read Patient Orientation tag, this seems more relibale
    //        std::string PatientOrientation = "0020|0037";
    //        auto tagItr = dictionary.Find(PatientOrientation);
    //        MetaDataStringType::ConstPointer PatientOrientationValue =
    //        dynamic_cast<const MetaDataStringType
    //        *>(tagItr->second.GetPointer() ); std::string val2 =
    //        PatientOrientationValue->GetMetaDataObjectValue();
    //        qDebug()<<"DICOM Cosines: "<<val2.c_str();

    //        QString curIOP;
    //        curIOP=QString::fromStdString(val2);

    QString supportedIOP = "HFS ";
    // DICOM seems to be hvaving a space after as "HFS "
    // QString supportedIOP="1\\0.0\\0.0\\0.0\\1\\0.0 ";//DICOM seems to be
    // hvaving a space at the end "
    // qDebug()<<curIOP<<"**********"<<supportedIOP;

    // Read patient's name tag
    std::string PatientName = "0010|0010";
    auto tagItr2 = dictionary.Find(PatientName);
    MetaDataStringType::ConstPointer PatientNameValue =
        dynamic_cast<const MetaDataStringType *>(tagItr2->second.GetPointer());
    std::string val2;
    val2 = PatientNameValue->GetMetaDataObjectValue();
    QString PatientNameStr;
    PatientInfo["PatientName"] = QString::fromStdString(val2);

    // Read patient's ID
    std::string PatientID = "0010|0020";
    auto tagItr3 = dictionary.Find(PatientID);
    MetaDataStringType::ConstPointer PatientIDValue =
        dynamic_cast<const MetaDataStringType *>(tagItr3->second.GetPointer());
    std::string val3;
    val3 = PatientIDValue->GetMetaDataObjectValue();
    QString PatientIDStr;
    PatientInfo["PatientID"] = QString::fromStdString(val3);

    // 1==match,-1=no match
    if (curIOP == supportedIOP) {
      typedef itk::ImageToVTKImageFilter<InputImageType> ConnectorType;
      ConnectorType::Pointer Converter = ConnectorType::New();
      Converter->SetInput(reader->GetOutput());
      Converter->Update();
      // qDebug()<<"Conversion done!";

      this->CTImage->DeepCopy(Converter->GetOutput());

      // Set patients name and ID to treeWidget
      QString NameIDStr;
      NameIDStr.append(PatientInfo["PatientName"]);
      NameIDStr.append(" | ");
      NameIDStr.append(PatientInfo["PatientID"]);
      this->ui->treeWidget->setHeaderLabel(NameIDStr);
      this->setWindowTitle(NameIDStr);

      // Hide information tree widget
      this->ui->actionInformation->trigger();

      // Display the data
      this->SagittalViewer =
          new ImageViewer2D(this->ui->mdiAreaView, this->ContextMenus);
      this->SagittalViewer->SetImageData(this->CTImage);
      this->SagittalViewer->SetSliceOrientation(1);
      this->SagittalViewer->SetUpView();
      this->ui->mdiAreaView->addSubWindow(
          this->SagittalViewer,
          Qt::WindowMaximizeButtonHint | Qt::WindowTitleHint);
      // add to make borderless window Qt::FramelessWindowHint
      this->SagittalViewer->setWindowTitle("Sagittal");
      this->SagittalViewer->show();

      this->CoronalViewer =
          new ImageViewer2D(this->ui->mdiAreaView, this->ContextMenus);
      this->CoronalViewer->SetImageData(this->CTImage);
      this->CoronalViewer->SetSliceOrientation(2);
      this->CoronalViewer->SetUpView();
      this->ui->mdiAreaView->addSubWindow(
          this->CoronalViewer,
          Qt::WindowMaximizeButtonHint | Qt::WindowTitleHint);
      this->CoronalViewer->setWindowTitle("Coronal");
      this->CoronalViewer->show();

      this->AxialViewer =
          new ImageViewer2D(this->ui->mdiAreaView, this->ContextMenus);
      this->AxialViewer->SetImageData(this->CTImage);
      this->AxialViewer->SetSliceOrientation(0);
      this->AxialViewer->SetUpView();
      this->ui->mdiAreaView->addSubWindow(
          this->AxialViewer,
          Qt::WindowMaximizeButtonHint | Qt::WindowTitleHint);
      this->AxialViewer->setWindowTitle("Axial");
      this->AxialViewer->show();

      this->ui->mdiAreaView->tileSubWindows();
      this->ui->statusBar->showMessage("CT imported sucessfully");

      // In somecases CT are centered, this fixes the problem
      this->ui->actionReset_Zoom->trigger();

      QTreeWidgetItem *wItem = new QTreeWidgetItem((QTreeWidget *)nullptr,
                                                   QStringList(QString("CT")));
      wItem->setCheckState(0, Qt::Checked);
      QIcon icon;
      icon.addFile(QString::fromUtf8(":/Icons/CT.png"), QSize(12, 12),
                   QIcon::Normal, QIcon::Off);
      wItem->setIcon(0, icon);
      this->ui->treeWidget->topLevelItem(0)->addChild(wItem);

      // Set default DoseVOI for RPL calculation
      double bds[6];
      this->CTImage->GetBounds(bds);

      // Enable actions
      this->ui->actionReset_WL_WW->setEnabled(true);
      this->ui->actionGo_To_Isocentre->setEnabled(true);
      this->ui->actionReset_Zoom->setEnabled(true);
      this->ui->actionShowBeams->setEnabled(true);
      this->ui->actionShowContours->setEnabled(true);
      this->ui->actionShowDose->setEnabled(true);
      this->ui->actionZoom_In_All->setEnabled(true);
      this->ui->actionZoom_Out_All->setEnabled(true);
      this->ui->actionBEV->setEnabled(true);
      this->ui->action3DView->setEnabled(true);
      this->ui->actionZoom_In_All->setEnabled(true);
      this->ui->actionZoom_Out_All->setEnabled(true);
      this->ui->actionReset_Zoom->setEnabled(true);
      this->ui->actionReset_WL_WW->setEnabled(true);
      this->ui->actionCalc_DVH->setEnabled(true);
      this->ui->actionStart->setEnabled(true);
      this->ui->actionStop->setEnabled(true);
      this->ui->actionStructures->setEnabled(true);
      this->ui->actionDose->setEnabled(true);
      this->ui->actionAdjust_Range->setEnabled(true);
      this->ui->actionWL_WW->setEnabled(true);
      this->ui->actionPlan->setEnabled(true);
      this->ui->actionArcsView->setEnabled(true);

    } else {
      QMessageBox messageBox;
      messageBox.critical(this, "Error",
                          "Unsupported image orientation.\nOnly HFS is "
                          "supported in this version.");
      messageBox.setFixedSize(500, 200);
    }

  } catch (itk::ExceptionObject &excp) {
    std::cerr << "Exception thrown while reading the series" << std::endl;
    std::cerr << excp << std::endl;
  }
}

void MainWindow::on_actionStructures_triggered() {
  if (this->CTImage->GetDimensions()[0] > 0) {
    // Read ROIs
    RTStructReaderDialog *meshReaderDlg = new RTStructReaderDialog(this);
    meshReaderDlg->exec();

    if (meshReaderDlg->ROINames.size() > 0)  // Check any ROI exist or not
    {
      QList<int> selectedStructsList = meshReaderDlg->selectedItems;
      // qDebug()<<selectedStructsList[0]<<"ROI";

      meshReader *RTStructReader = new meshReader(this);
      // RTStructReader->getStructFileName();
      RTStructReader->structFileName = meshReaderDlg->structFileName;
      QCoreApplication::processEvents();
      RTStructReader->getROIMeshes(
          this->CTImage, this->CTImage->GetSpacing()[2], this->TargetReduction,
          meshReaderDlg->selectedItems,
          this);  // Reads ROI name as well as structs
      QCoreApplication::processEvents();
      this->MeshList = RTStructReader->meshes;
      this->MeshActors = RTStructReader->ROIActors;
      this->ROIVisibleFlag = 1;  // structs imported

      for (int i = 0; i < meshReaderDlg->selectedItems.size(); i++) {
        this->ROIColors[i][0] = RTStructReader->ROIColors[i][0];
        this->ROIColors[i][1] = RTStructReader->ROIColors[i][1];
        this->ROIColors[i][2] = RTStructReader->ROIColors[i][2];
      }

      this->ROINum = meshReaderDlg->selectedItems.size();
      this->ROINames = RTStructReader->ROINames;
      this->ROITypes = RTStructReader->ROITypes;
      this->ROINo = RTStructReader->ROINo;

      QList<QTreeWidgetItem *> items;
      for (int i = 0; i < meshReaderDlg->selectedItems.size(); ++i) {
        QTreeWidgetItem *wItem = new QTreeWidgetItem(
            (QTreeWidget *)0,
            QStringList(QString(RTStructReader->ROINames[i])));
        wItem->setCheckState(0, Qt::Checked);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Icons/Polygon.png"), QSize(12, 12),
                     QIcon::Normal, QIcon::Off);
        wItem->setIcon(0, icon);
        wItem->setBackgroundColor(0, QColor(RTStructReader->ROIColors[i][0],
                                            RTStructReader->ROIColors[i][1],
                                            RTStructReader->ROIColors[i][2]));
        items.append(wItem);
      }
      // Add ROIs to RTSS item
      this->ui->treeWidget->topLevelItem(1)->addChildren(items);
      this->ui->treeWidget->expandAll();

      this->ui->treeWidget->topLevelItem(1)->setText(
          0, QString(RTStructReader->structSetLabel));

      delete RTStructReader;
    }
    delete meshReaderDlg;

    // Dispaly ROIs
    this->AxialViewer->MeshList = this->MeshList;
    this->AxialViewer->ROIColors = this->ROIColors;
    this->AxialViewer->show();
    this->AxialViewer->ContourVisibility = 1;
    this->AxialViewer->DisplayROIs(this->AxialViewer->SliceLoc,
                                   this->AxialViewer->SliceOrientation);
    this->AxialViewer->UpdateView();

    this->SagittalViewer->MeshList = this->MeshList;
    this->SagittalViewer->ROIColors = this->ROIColors;
    this->SagittalViewer->show();
    this->SagittalViewer->ContourVisibility = 1;
    this->SagittalViewer->DisplayROIs(this->SagittalViewer->SliceLoc,
                                      this->SagittalViewer->SliceOrientation);
    this->SagittalViewer->UpdateView();

    this->CoronalViewer->MeshList = this->MeshList;
    this->CoronalViewer->ROIColors = this->ROIColors;
    this->CoronalViewer->show();
    this->CoronalViewer->ContourVisibility = 1;
    this->CoronalViewer->DisplayROIs(this->CoronalViewer->SliceLoc,
                                     this->CoronalViewer->SliceOrientation);
    this->CoronalViewer->UpdateView();

    this->ui->action3DView->trigger();
  } else {
    QMessageBox messageBox;
    messageBox.critical(this, "Error",
                        "Please load a CT image before loading structures");
    messageBox.setFixedSize(500, 200);
  }
}

void MainWindow::on_actionDose_triggered() {
  if (this->CTImage->GetDimensions()[0] > 0) {
    QString DoseFile = QFileDialog::getOpenFileName(this, "Open RT Dose");

    if (DoseFile != nullptr) {
      // qDebug()<<doseFile;
      const unsigned int InputDimension = 3;
      typedef double PixelType;
      typedef itk::Image<PixelType, InputDimension> InputImageType;
      typedef itk::ImageSeriesReader<InputImageType> ReaderType;
      typedef itk::GDCMImageIO ImageIOType;
      ImageIOType::Pointer gdcmIO = ImageIOType::New();
      ReaderType::Pointer reader = ReaderType::New();

      reader->SetImageIO(gdcmIO);
      reader->SetFileName(DoseFile.toStdString());
      reader->Update();

      // Multiply the raw values of the image data with dose grid scaling
      // factor;
      using DictionaryType = itk::MetaDataDictionary;
      typedef itk::MetaDataObject<std::string> MetaDataStringType;
      const DictionaryType &dictionary = gdcmIO->GetMetaDataDictionary();

      std::string doseGridScaling = "3004|000e";
      auto doseGridScalingFactor = dictionary.Find(doseGridScaling);
      MetaDataStringType::ConstPointer doseGridScalingVal =
          dynamic_cast<const MetaDataStringType *>(
              doseGridScalingFactor->second.GetPointer());
      std::string doseGridScalingStr;
      doseGridScalingStr = doseGridScalingVal->GetMetaDataObjectValue();
      // qDebug() << std::stod(doseGridScalingStr) << "Dose grid scaling ";

      typedef itk::ImageToVTKImageFilter<InputImageType> ConnectorType;
      ConnectorType::Pointer Converter = ConnectorType::New();
      Converter->SetInput(reader->GetOutput());
      Converter->Update();

      vtkSmartPointer<vtkImageShiftScale> scaler =
          vtkSmartPointer<vtkImageShiftScale>::New();
      scaler->SetInputData(Converter->GetOutput());
      scaler->SetScale(std::stod(doseGridScalingStr));
      // Important to get accurate dose values
      scaler->SetOutputScalarTypeToDouble();
      scaler->Update();

      this->RTDose->DeepCopy(scaler->GetOutput());
      // qDebug()<<this->RTDose->GetScalarRange()[0]<<":Min"<<this->RTDose->GetScalarRange()[1]<<":Max";
    } else {
      QMessageBox messageBox;
      messageBox.critical(this, "Error", "An error has occured !");
      messageBox.setFixedSize(500, 200);
      messageBox.show();
    }

    this->AxialViewer->SetRTDose(this->RTDose);
    this->SagittalViewer->SetRTDose(this->RTDose);
    this->CoronalViewer->SetRTDose(this->RTDose);
    this->BEVViewer->RTDose = this->RTDose;

    this->AxialViewer->DoseVisibility = true;
    this->AxialViewer->UpdateView();
    this->SagittalViewer->DoseVisibility = true;
    this->SagittalViewer->UpdateView();
    this->CoronalViewer->DoseVisibility = true;
    this->CoronalViewer->UpdateView();

    this->ui->statusBar->showMessage("Dose imported sucessfully");

  } else {
    QMessageBox messageBox;
    messageBox.critical(this, "Error",
                        "Please load a CT image before loading dose");
    messageBox.setFixedSize(500, 200);
  }
}

void MainWindow::on_actionGo_To_Isocentre_triggered() {
  qDebug() << "Iso: " << this->Isocentre[0] << this->Isocentre[1]
           << this->Isocentre[2];

  QList<QMdiSubWindow *> SubWindows = this->ui->mdiAreaView->subWindowList();
  if (SubWindows[1]) {
    ImageViewer2D *Viewer1 =
        qobject_cast<ImageViewer2D *>(SubWindows[1]->widget());
    if (Viewer1->SliceOrientation == 0) {
      Viewer1->MoveToLocation(this->Isocentre[2]);

    }

    else if (Viewer1->SliceOrientation == 1) {
      Viewer1->MoveToLocation(this->Isocentre[0]);

    }

    else if (Viewer1->SliceOrientation == 2) {
      Viewer1->MoveToLocation(this->Isocentre[1]);
    }
  }

  if (SubWindows[2]) {
    ImageViewer2D *Viewer2 =
        qobject_cast<ImageViewer2D *>(SubWindows[2]->widget());
    if (Viewer2->SliceOrientation == 0) {
      Viewer2->MoveToLocation(this->Isocentre[2]);

    }

    else if (Viewer2->SliceOrientation == 1) {
      Viewer2->MoveToLocation(this->Isocentre[0]);

    }

    else if (Viewer2->SliceOrientation == 2) {
      Viewer2->MoveToLocation(this->Isocentre[1]);
    }
  }

  if (SubWindows[3]) {
    ImageViewer2D *Viewer3 =
        qobject_cast<ImageViewer2D *>(SubWindows[3]->widget());
    if (Viewer3->SliceOrientation == 0) {
      Viewer3->MoveToLocation(this->Isocentre[2]);

    }

    else if (Viewer3->SliceOrientation == 1) {
      Viewer3->MoveToLocation(this->Isocentre[0]);

    }

    else if (Viewer3->SliceOrientation == 2) {
      Viewer3->MoveToLocation(this->Isocentre[1]);
    }
  }
}

void MainWindow::on_actionBEV_triggered() {
  this->ui->tableWidget->setFocus();

  // qDebug()<<"No. of beams:"<<this->ui->tableWidget->rowCount();

  if (!(this->ui->tableWidget->rowCount() == 0)) {
    // By default show the first beam if none selected
    int SelectedBeam = 0;
    SelectedBeam = this->ui->tableWidget->currentRow();
    double gantryAngle =
        this->ui->tableWidget->item(SelectedBeam, 5)->text().toDouble();
    // qDebug()<<gantryAngle<<":Selected Beam Angle";

    // Display the data
    this->BEVViewer->MeshActors = this->MeshActors;
    this->BEVViewer->ROIColors = this->ROIColors;
    this->BEVViewer->Isocentre = this->Isocentre;
    this->BEVViewer->GantryAngle = gantryAngle;
    this->BEVViewer->DisplayBEV();
    this->BEVViewer->show();

  }

  else {
    QMessageBox messageBox;
    messageBox.critical(this, "Error",
                        "No beam exist. Please add a beam to view BEV.");
    messageBox.setFixedSize(500, 200);
  }

  // this->ui->mdiAreaView->tileSubWindows();
}

void MainWindow::on_action3DView_triggered() {
  this->ui->tableWidget->setFocus();
  // Display the data
  this->BEVViewer->MeshActors = this->MeshActors;
  this->BEVViewer->BeamActors = this->BeamActors;
  this->BEVViewer->ROIColors = this->ROIColors;
  this->BEVViewer->ROITypes = this->ROITypes;
  this->BEVViewer->DisplayMeshes();
  this->BEVViewer->DisplayBeams();

  this->InteractorTrackball =
      vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
  this->BEVViewer->ModelRenderer->GetRenderWindow()
      ->GetInteractor()
      ->SetInteractorStyle(this->InteractorTrackball);
  this->BEVViewer->show();
  this->BEVViewer->ModelRenderer->GetRenderWindow()->Render();
}

void MainWindow::on_actionClose_Patient_triggered() {
  QApplication::closeAllWindows();
}

void MainWindow::on_actionReset_Zoom_triggered() {
  this->AxialViewer->ViewRenderer->ResetCamera();
  this->AxialViewer->ViewRenderer->GetRenderWindow()->Render();

  this->SagittalViewer->ViewRenderer->ResetCamera();
  this->SagittalViewer->ViewRenderer->GetRenderWindow()->Render();

  this->CoronalViewer->ViewRenderer->ResetCamera();
  this->CoronalViewer->ViewRenderer->GetRenderWindow()->Render();
}

void MainWindow::on_actionShowBeams_triggered() {
  this->AxialViewer->TriggerActionShowBeams();
  this->SagittalViewer->TriggerActionShowBeams();
  this->CoronalViewer->TriggerActionShowBeams();
}

void MainWindow::on_actionShowDose_triggered() {
  this->AxialViewer->TriggerActionShowDose();
  this->SagittalViewer->TriggerActionShowDose();
  this->CoronalViewer->TriggerActionShowDose();

  if (this->AxialViewer->DoseVisibility == 1) {
    this->BEVViewer->IsodoseSurface->VisibilityOn();
    // qDebug()<<"Dose on";

  } else {
    this->BEVViewer->IsodoseSurface->VisibilityOff();
    // qDebug()<<"Dose off";
  }
  this->BEVViewer->ModelRenderer->GetRenderWindow()->Render();
}

void MainWindow::on_actionShowContours_triggered() {
  this->AxialViewer->TriggerActionShowContours();
  this->SagittalViewer->TriggerActionShowContours();
  this->CoronalViewer->TriggerActionShowContours();
}

void MainWindow::on_actionZoom_In_All_triggered() {
  // qDebug()<<"Zooming in...";
  this->AxialViewer->ViewRenderer->GetActiveCamera()->Zoom(1.1);
  this->AxialViewer->ViewRenderer->GetRenderWindow()->Render();

  this->SagittalViewer->ViewRenderer->GetActiveCamera()->Zoom(1.1);
  this->SagittalViewer->ViewRenderer->GetRenderWindow()->Render();

  this->CoronalViewer->ViewRenderer->GetActiveCamera()->Zoom(1.1);
  this->CoronalViewer->ViewRenderer->GetRenderWindow()->Render();
}

void MainWindow::on_actionZoom_Out_All_triggered() {
  // qDebug()<<"Zooming out...";
  this->AxialViewer->ViewRenderer->GetActiveCamera()->Zoom(0.9);
  this->AxialViewer->ViewRenderer->GetRenderWindow()->Render();

  this->SagittalViewer->ViewRenderer->GetActiveCamera()->Zoom(0.9);
  this->SagittalViewer->ViewRenderer->GetRenderWindow()->Render();

  this->CoronalViewer->ViewRenderer->GetActiveCamera()->Zoom(0.9);
  this->CoronalViewer->ViewRenderer->GetRenderWindow()->Render();
}

void MainWindow::on_actionInformation_triggered() {
  if (this->ui->actionInformation->isChecked()) {
    this->ui->dockWidget->setVisible(true);
  }

  else {
    this->ui->dockWidget->setVisible(false);
  }

  this->ui->mdiAreaView->tileSubWindows();
}

void MainWindow::on_actionPlan_Information_triggered() {
  if (this->ui->actionPlan_Information->isChecked()) {
    this->ui->dockWidget_2->setVisible(true);
  }

  else {
    this->ui->dockWidget_2->setVisible(false);
  }

  this->ui->mdiAreaView->tileSubWindows();
}

void MainWindow::on_actionCalc_DVH_triggered() {
  // qDebug()<<this->rtDoseCube->GetScalarRange()[1];
  if (this->RTDose->GetScalarRange()[1] == 1) {
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setText("Dose not available.");
    msgBox->setIcon(QMessageBox::Critical);
    msgBox->setWindowTitle("Error");
    msgBox->exec();
    delete msgBox;
  }

  else {
    DVHDialog *myDVHDialog = new DVHDialog(this);
    myDVHDialog->doseMatrix = this->RTDose;
    myDVHDialog->maxDose = this->RTDose->GetScalarRange()[1];
    myDVHDialog->meshes = this->MeshList;
    myDVHDialog->setROIColors(this->ROIColors);
    myDVHDialog->setStructureNames(this->ROINames);
    myDVHDialog->ROIType = this->ROITypes;
    myDVHDialog->ROINo = this->ROINo;

    myDVHDialog->exec();
    delete myDVHDialog;
  }
}

void MainWindow::on_actionAdjust_Range_triggered() {
  if (this->RTDose->GetScalarRange()[1] == 1.0) {
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setText("Dose not available.");
    msgBox->setIcon(QMessageBox::Critical);
    msgBox->setWindowTitle("Error");
    msgBox->exec();
    delete msgBox;
  } else {
    RangeSliderDialog *ranger = new RangeSliderDialog(this);
    ranger->minDose = this->RTDose->GetScalarRange()[0];
    ranger->maxDose = this->RTDose->GetScalarRange()[1];
    // Need to set viewers first before SetDoseRange, or else crashes
    ranger->AxialViewer = this->AxialViewer;
    ranger->SagittalViewer = this->SagittalViewer;
    ranger->CoronalViewer = this->CoronalViewer;
    ranger->ModelViewer = this->BEVViewer;
    ranger->SetDoseRange();
    ranger->show();
  }
}

void MainWindow::on_actionReset_WL_WW_triggered() {
  this->AxialViewer->TriggerReset_WL_WW();
  this->SagittalViewer->TriggerReset_WL_WW();
  this->CoronalViewer->TriggerReset_WL_WW();
}

void MainWindow::on_actionRender_Bones_triggered() {}

void MainWindow::on_actionHello_UDP_triggered() {}

void MainWindow::on_actionMove_ROI_triggered() {
  // DRR begins
  vtkSmartPointer<vtkImageData> imagevtk = vtkSmartPointer<vtkImageData>::New();
  imagevtk = this->CTImage;

  const unsigned int Dimension = 3;
  typedef short InputPixelType;
  typedef unsigned char OutputPixelType;
  typedef itk::Image<InputPixelType, 3> InputImageType;
  typedef itk::Image<OutputPixelType, 3> OutputImageType;

  float rx = 90.0;
  float ry = 0.0;
  float rz = 0.0;  // 0 deg gantry
  float tx = 0.0;
  float ty = 0.0;
  float tz = 0.0;
  float cx = 0.0;
  float cy = 0.0;
  float cz = 0.0;
  float sid = 1000.0;  // 150 cm
  float sx = 2.0;
  float sy = 2.0;
  int dx = 501;
  int dy = 501;
  float o2Dx = 0;
  float o2Dy = 0;
  double threshold = 0;

  // Convert vtk image to itk image
  typedef itk::Image<short, 3> InputImageType1;
  typedef itk::VTKImageToImageFilter<InputImageType1> VTKImageToImageTypeInput;
  VTKImageToImageTypeInput::Pointer converterInput =
      VTKImageToImageTypeInput::New();
  converterInput->SetInput(imagevtk);
  converterInput->Update();
  qDebug() << "Conversion done";

  // image=converterInput->GetOutput();

  typedef itk::ResampleImageFilter<InputImageType, InputImageType> FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(converterInput->GetOutput());
  filter->SetDefaultPixelValue(0);

  typedef itk::CenteredEuler3DTransform<double> TransformType;
  TransformType::Pointer transform = TransformType::New();
  transform->SetComputeZYX(true);
  TransformType::OutputVectorType translation;

  translation[0] = tx;
  translation[1] = ty;
  translation[2] = tz;

  // constant for converting degrees into radians
  const double dtr = (vcl_atan(1.0) * 4.0) / 180.0;
  qDebug() << "Conversion done";
  transform->SetTranslation(translation);
  transform->SetRotation(dtr * rx, dtr * ry, dtr * rz);

  InputImageType::PointType imOrigin = converterInput->GetOutput()->GetOrigin();
  InputImageType::SpacingType imRes = converterInput->GetOutput()->GetSpacing();

  typedef InputImageType::RegionType InputImageRegionType;
  typedef InputImageRegionType::SizeType InputImageSizeType;

  InputImageRegionType imRegion =
      converterInput->GetOutput()->GetBufferedRegion();
  InputImageSizeType imSize = imRegion.GetSize();

  imOrigin[0] += imRes[0] * static_cast<double>(imSize[0]) / 2.0;
  imOrigin[1] += imRes[1] * static_cast<double>(imSize[1]) / 2.0;
  imOrigin[2] += imRes[2] * static_cast<double>(imSize[2]) / 2.0;

  TransformType::InputPointType center;
  center[0] = cx + imOrigin[0];
  center[1] = cy + imOrigin[1];
  center[2] = cz + imOrigin[2];

  transform->SetCenter(center);

  typedef itk::RayCastInterpolateImageFunction<InputImageType, double>
      InterpolatorType;
  InterpolatorType::Pointer interpolator = InterpolatorType::New();
  interpolator->SetTransform(transform);
  interpolator->SetThreshold(threshold);

  InterpolatorType::InputPointType focalpoint;

  focalpoint[0] = imOrigin[0];
  focalpoint[1] = imOrigin[1];
  focalpoint[2] = imOrigin[2] - sid / 2.;

  interpolator->SetFocalPoint(focalpoint);

  filter->SetInterpolator(interpolator);
  filter->SetTransform(transform);

  InputImageType::SizeType size;
  size[0] = dx;  // number of pixels along X of the 2D DRR image
  size[1] = dy;  // number of pixels along Y of the 2D DRR image
  size[2] = 1;   // only one slice

  filter->SetSize(size);

  InputImageType::SpacingType spacing;
  spacing[0] = sx;   // pixel spacing along X of the 2D DRR image [mm]
  spacing[1] = sy;   // pixel spacing along Y of the 2D DRR image [mm]
  spacing[2] = 2.0;  // slice thickness of the 2D DRR image [mm]
  filter->SetOutputSpacing(spacing);

  double origin[Dimension];
  origin[0] = imOrigin[0] + o2Dx - sx * ((double)dx - 1.) / 2.;
  origin[1] = imOrigin[1] + o2Dy - sy * ((double)dy - 1.) / 2.;
  origin[2] = imOrigin[2] + sid / 2.;
  filter->SetOutputOrigin(origin);

  typedef itk::RescaleIntensityImageFilter<InputImageType, OutputImageType>
      RescaleFilterType;
  RescaleFilterType::Pointer rescaler = RescaleFilterType::New();
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  rescaler->SetInput(filter->GetOutput());
  qDebug() << "Conversion done";

  typedef itk::Image<unsigned char, 3> OutImageType;
  typedef itk::ImageToVTKImageFilter<OutImageType> ImageToVTKImageType;
  ImageToVTKImageType::Pointer converter2 = ImageToVTKImageType::New();
  converter2->SetInput(rescaler->GetOutput());
  converter2->Update();
  qDebug() << "Converted back to VTK Image";

  // qDebug()<<converter2->GetOutput()->GetScalarRange()[0]<<converter2->GetOutput()->GetScalarRange()[1];

  // Create a greyscale lookup table
  vtkSmartPointer<vtkLookupTable> table =
      vtkSmartPointer<vtkLookupTable>::New();
  table->SetRange(0, 370);              // image intensity range
  table->SetValueRange(0, 1.0);         // from black to white
  table->SetSaturationRange(0.0, 0.0);  // no color saturation
  table->SetRampToLinear();
  table->Build();

  // Map the image through the lookup table
  vtkSmartPointer<vtkImageMapToColors> color =
      vtkSmartPointer<vtkImageMapToColors>::New();
  color->SetLookupTable(table);
  color->SetInputData(converter2->GetOutput());
  color->Update();

  vtkSmartPointer<vtkImageActor> imgact = vtkSmartPointer<vtkImageActor>::New();
  imgact->SetInputData(color->GetOutput());
  // imgact->SetUserTransform(this->userTransform);
  imgact->RotateX(90);
  imgact->RotateY(-180);
  double iso[3] = {-12.0, 124, -30};
  imgact->SetPosition(iso);
  imgact->SetOpacity(0.8);

  this->BEVViewer->ModelRenderer->AddActor(imgact);
  this->BEVViewer->ModelRenderer->Render();
}

void MainWindow::on_actionRotate_ROI_triggered() {
  // qDebug() << this->AxialViewer->ImageSlice->GetInput()->GetScalarRange()[0];
  // qDebug() << this->AxialViewer->ImageSlice->GetInput()->GetScalarRange()[1];
}

void MainWindow::on_actionAdd_Arc_triggered() {
  CreateObjects *beams = new CreateObjects;

  vtkSmartPointer<vtkActor> beam = vtkSmartPointer<vtkActor>::New();
  beam = beams->createBeam(-44, 42, -42, 44, 700, 1300, 180.1, 45.40, 0.0,
                           this->Isocentre);

  //  vtkSmartPointer<vtkRenderer> renderer =
  //  vtkSmartPointer<vtkRenderer>::New(); vtkSmartPointer<vtkRenderWindow>
  //  renderWindow =
  //      vtkSmartPointer<vtkRenderWindow>::New();
  //  renderWindow->AddRenderer(renderer);
  //  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
  //      vtkSmartPointer<vtkRenderWindowInteractor>::New();
  //  renderWindowInteractor->SetRenderWindow(renderWindow);

  this->BEVViewer->ModelRenderer->AddActor(beam);
  this->BEVViewer->ModelRenderer->GetRenderWindow()->Render();
}

void MainWindow::on_actionSend_UDP_triggered() {
  const unsigned int InputDimension = 3;
  typedef double PixelType;
  typedef itk::Image<PixelType, InputDimension> InputImageType;
  typedef itk::ImageSeriesReader<InputImageType> ReaderType;
  typedef itk::GDCMImageIO ImageIOType;
  ImageIOType::Pointer gdcmIO = ImageIOType::New();
  ReaderType::Pointer reader = ReaderType::New();

  QString DoseFile = QFileDialog::getOpenFileName(this, "Open RT Dose");

  // Start reading DICOM CT data with Phase info
  /********************************************************************************/

  reader->SetImageIO(gdcmIO);
  reader->SetFileName(DoseFile.toStdString());
  reader->Update();

  typedef itk::ImageToVTKImageFilter<InputImageType> ConnectorType;
  ConnectorType::Pointer Converter = ConnectorType::New();
  Converter->SetInput(reader->GetOutput());
  Converter->Update();
  // qDebug()<<"Conversion done!";

  this->RTDose->DeepCopy(Converter->GetOutput());

  this->AxialViewer->SetRTDose(this->RTDose);
  this->SagittalViewer->SetRTDose(this->RTDose);
  this->CoronalViewer->SetRTDose(this->RTDose);
  this->BEVViewer->RTDose = this->RTDose;

  this->AxialViewer->DoseVisibility = true;
  this->AxialViewer->UpdateView();
  this->SagittalViewer->DoseVisibility = true;
  this->SagittalViewer->UpdateView();
  this->CoronalViewer->DoseVisibility = true;
  this->CoronalViewer->UpdateView();

  this->ui->statusBar->showMessage("Dose imported sucessfully");
}

void MainWindow::on_actionAbout_QT_triggered() { QMessageBox::aboutQt(this); }

void MainWindow::on_actionIP_COnfiguration_triggered() {
  IPConfigDialog *IPDialog = new IPConfigDialog(this);
  IPDialog->exec();
  delete IPDialog;
}

void MainWindow::on_actionStart_triggered() {
  this->ui->statusBar->showMessage("Listening to KIM");

  this->listener = new UDPListener(this);
  this->listener->parent = this;

  this->listener->ROINames = this->ROINames;
  this->listener->ROIColors = this->ROIColors;
  this->listener->MeshList = this->MeshList;

  this->listener->AxialViewer = this->AxialViewer;
  this->listener->SagittalViewer = this->SagittalViewer;
  this->listener->CoronalViewer = this->CoronalViewer;
  this->listener->BEVViewer = this->BEVViewer;

  this->listener->StartListening();
  this->ListeningState = true;

  // QApplication::processEvents();
}

void MainWindow::on_actionStop_triggered() {
  this->ui->statusBar->clearMessage();
  if (this->ListeningState == true) {
    this->listener->StopListening();
    delete this->listener;
    this->ListeningState = false;
  }
}

void MainWindow::closeEvent(QCloseEvent *event) {
  QMessageBox::StandardButton resBtn = QMessageBox::question(
      this, "KIMView", tr("Are you sure you want to close?\n"),
      QMessageBox::No | QMessageBox::Yes,
      QMessageBox::No);  // Set default focus to NO
  if (resBtn == QMessageBox::No) {
    event->ignore();
  } else {
    if (this->ListeningState == true) {
      this->listener->StopListening();
      delete this->listener;
    }

    event->accept();
  }
}

void MainWindow::on_actionWL_WW_triggered() {
  WLWWDialog *WLWlDlg = new WLWWDialog(this);
  WLWlDlg->AxialViewer = this->AxialViewer;
  WLWlDlg->SagittalViewer = this->SagittalViewer;
  WLWlDlg->CoronalViewer = this->CoronalViewer;
  WLWlDlg->show();
}

void MainWindow::on_actionAbout_triggered() {
  AboutDialog *abtDlg = new AboutDialog(this);
  abtDlg->exec();
}

void MainWindow::on_actionPlan_triggered() {
  try {
    PlanReader *myPlanReader = new PlanReader();
    myPlanReader->readRTPlan();
    // qDebug() << "Frs: " << myPlanReader->fractionsPlanned;
    // qDebug() << "No. of beams: " << myPlanReader->numOfBeams;
    // qDebug() << "Plan name: " << myPlanReader->planDetailStruct[0].beamType;
    // qDebug() << myPlanReader->planDetailStruct[0].beamAngle;
    // qDebug() << myPlanReader->planDetailStruct[0].beamStopAngle;

    this->ui->tableWidget->setRowCount(0);

    this->ui->treeWidget->topLevelItem(2)->setText(
        0, QString(myPlanReader->planLabel));

    this->ui->tableWidget->setRowCount(myPlanReader->numOfBeams);

    for (int i = 0; i < myPlanReader->numOfBeams; i++) {
      // Add a treeWidegtItem for each beam
      QTreeWidgetItem *wItem = new QTreeWidgetItem(
          (QTreeWidget *)nullptr,
          QStringList(QString(myPlanReader->planDetailStruct[i].beamName)));
      wItem->setCheckState(0, Qt::Checked);
      QIcon icon;
      icon.addFile(QString::fromUtf8(":/Icons/Beam.png"), QSize(12, 12),
                   QIcon::Normal, QIcon::Off);
      wItem->setIcon(0, icon);
      this->ui->treeWidget->topLevelItem(2)->addChild(wItem);
      this->ui->treeWidget->expandAll();

      QTableWidgetItem *item1 = new QTableWidgetItem;
      item1->setText(
          QString::number(myPlanReader->planDetailStruct[i].beamNum));
      item1->setTextAlignment(Qt::AlignCenter);
      this->ui->tableWidget->setItem(i, 0, item1);

      QTableWidgetItem *item2 = new QTableWidgetItem;
      item2->setText(myPlanReader->planDetailStruct[i].beamName.toLatin1());
      item2->setTextAlignment(Qt::AlignCenter);
      this->ui->tableWidget->setItem(i, 1, item2);

      QTableWidgetItem *item3 = new QTableWidgetItem;
      item3->setText(tr(myPlanReader->planDetailStruct[i].mcName.toLatin1()));
      item3->setTextAlignment(Qt::AlignCenter);
      this->ui->tableWidget->setItem(i, 2, item3);

      QTableWidgetItem *item4 = new QTableWidgetItem;
      item4->setText(tr(myPlanReader->planDetailStruct[i].beamType.toLatin1()));
      item4->setTextAlignment(Qt::AlignCenter);
      this->ui->tableWidget->setItem(i, 3, item4);

      QTableWidgetItem *item5 = new QTableWidgetItem;
      item5->setText(
          QString::number(myPlanReader->planDetailStruct[i].beamEnergy));
      item5->setTextAlignment(Qt::AlignCenter);
      this->ui->tableWidget->setItem(i, 4, item5);

      QTableWidgetItem *item6 = new QTableWidgetItem;
      item6->setText(
          QString::number(myPlanReader->planDetailStruct[i].beamAngle, 'f', 1));
      item6->setTextAlignment(Qt::AlignCenter);
      this->ui->tableWidget->setItem(i, 5, item6);

      QTableWidgetItem *item7 = new QTableWidgetItem;
      item7->setText(QString::number(
          myPlanReader->planDetailStruct[i].beamStopAngle, 'f', 1));
      item7->setTextAlignment(Qt::AlignCenter);
      this->ui->tableWidget->setItem(i, 6, item7);

      QTableWidgetItem *item8 = new QTableWidgetItem;
      item8->setText(
          tr(myPlanReader->planDetailStruct[i].arcDirection.toLatin1()));
      item8->setTextAlignment(Qt::AlignCenter);
      this->ui->tableWidget->setItem(i, 7, item8);

      QTableWidgetItem *item9 = new QTableWidgetItem;
      item9->setText(
          QString::number(myPlanReader->planDetailStruct[i].collAngle, 'f', 1));
      item9->setTextAlignment(Qt::AlignCenter);
      this->ui->tableWidget->setItem(i, 8, item9);

      QTableWidgetItem *item10 = new QTableWidgetItem;
      item10->setText(QString::number(
          myPlanReader->planDetailStruct[i].couchAngle, 'f', 1));
      item10->setTextAlignment(Qt::AlignCenter);
      this->ui->tableWidget->setItem(i, 9, item10);

      QTableWidgetItem *item11 = new QTableWidgetItem;
      item11->setText(
          QString::number(myPlanReader->planDetailStruct[i].fieldX1, 'f', 1));
      item11->setTextAlignment(Qt::AlignCenter);
      this->ui->tableWidget->setItem(i, 10, item11);

      QTableWidgetItem *item12 = new QTableWidgetItem;
      item12->setText(
          QString::number(myPlanReader->planDetailStruct[i].fieldX2, 'f', 1));
      item12->setTextAlignment(Qt::AlignCenter);
      this->ui->tableWidget->setItem(i, 11, item12);

      QTableWidgetItem *item13 = new QTableWidgetItem;
      item13->setText(
          QString::number(myPlanReader->planDetailStruct[i].fieldY1, 'f', 1));
      item13->setTextAlignment(Qt::AlignCenter);
      this->ui->tableWidget->setItem(i, 12, item13);

      QTableWidgetItem *item14 = new QTableWidgetItem;
      item14->setText(
          QString::number(myPlanReader->planDetailStruct[i].fieldY2, 'f', 1));
      item14->setTextAlignment(Qt::AlignCenter);
      this->ui->tableWidget->setItem(i, 13, item14);

      QTableWidgetItem *item15 = new QTableWidgetItem;
      item15->setText(
          QString::number(myPlanReader->planDetailStruct[i].icX, 'f', 1));
      item15->setTextAlignment(Qt::AlignCenter);
      this->ui->tableWidget->setItem(i, 14, item15);

      QTableWidgetItem *item16 = new QTableWidgetItem;
      item16->setText(
          QString::number(myPlanReader->planDetailStruct[i].icY, 'f', 1));
      item16->setTextAlignment(Qt::AlignCenter);
      this->ui->tableWidget->setItem(i, 15, item16);

      QTableWidgetItem *item17 = new QTableWidgetItem;
      item17->setText(
          QString::number(myPlanReader->planDetailStruct[i].icZ, 'f', 1));
      item17->setTextAlignment(Qt::AlignCenter);
      this->ui->tableWidget->setItem(i, 16, item17);

      QTableWidgetItem *item18 = new QTableWidgetItem;
      item18->setText(
          QString::number(myPlanReader->planDetailStruct[i].ssd, 'f', 1));
      item18->setTextAlignment(Qt::AlignCenter);
      this->ui->tableWidget->setItem(i, 17, item18);

      QTableWidgetItem *item19 = new QTableWidgetItem;
      item19->setText(
          QString::number(myPlanReader->planDetailStruct[i].mu, 'f', 1));
      item19->setTextAlignment(Qt::AlignCenter);
      this->ui->tableWidget->setItem(i, 18, item19);
    }

    // Update isocentre values in Mainwindow class
    this->Isocentre[0] = myPlanReader->planDetailStruct[0].icX * 10;  // cm to
    this->Isocentre[1] =
        myPlanReader->planDetailStruct[0].icY * 10;  // cm to mm
    this->Isocentre[2] =
        myPlanReader->planDetailStruct[0].icZ * 10;  // cm to mm

    // Add isocentre POI
    // Create a sphere source and  add it to the mesh list
    vtkSmartPointer<vtkSphereSource> SphereSrc =
        vtkSmartPointer<vtkSphereSource>::New();
    SphereSrc->SetRadius(
        1.0);  // 1mm so that it just appears on one single slice
    SphereSrc->SetThetaResolution(100.0);
    SphereSrc->SetPhiResolution(100.0);
    SphereSrc->SetCenter(this->Isocentre);
    SphereSrc->Update();

    vtkSmartPointer<vtkTriangleFilter> Triangler =
        vtkSmartPointer<vtkTriangleFilter>::New();
    Triangler->SetInputConnection(SphereSrc->GetOutputPort());
    Triangler->Update();

    this->POIList.push_back(Triangler->GetOutput());

    QTreeWidgetItem *IsoItem = new QTreeWidgetItem(
        (QTreeWidget *)nullptr, QStringList(QString("Isocentre")));
    IsoItem->setCheckState(0, Qt::Checked);
    QIcon IsoIcon;
    IsoIcon.addFile(QString::fromUtf8(":/RC/Icons/POI.png"), QSize(10, 10),
                    QIcon::Normal, QIcon::Off);
    IsoItem->setIcon(0, IsoIcon);
    IsoItem->setBackgroundColor(0, QColor(255, 255, 0));
    this->ui->treeWidget->topLevelItem(4)->addChild(IsoItem);

    this->AxialViewer->POIVisibility = true;
    this->AxialViewer->POIList = this->POIList;
    this->SagittalViewer->POIVisibility = true;
    this->SagittalViewer->POIList = this->POIList;
    this->CoronalViewer->POIVisibility = true;
    this->CoronalViewer->POIList = this->POIList;

    delete myPlanReader;

    // Clear any existing arcs
    this->arcList.clear();

    // Add arcs if it's VMAT
    QString DeliveryType = this->ui->tableWidget->item(0, 3)->text();
    // qDebug() << DeliveryType << " :Type";
    // DICOM seems to have a space after "DYNAMIC "
    if (DeliveryType == "DYNAMIC ") {
      CreateObjects *arcCreator = new CreateObjects;

      for (int i = 0; i < this->ui->tableWidget->rowCount(); i++) {
        double gantryStart =
            this->ui->tableWidget->item(i, 5)->text().toDouble();
        double gantryStop =
            this->ui->tableWidget->item(i, 6)->text().toDouble();
        QString dir = this->ui->tableWidget->item(i, 7)->text();
        // qDebug() << gantryStart << gantryStop << dir;

        double radius = 250.0 + (25 * i);

        vtkSmartPointer<vtkAssembly> arc = vtkSmartPointer<vtkAssembly>::New();
        arc = arcCreator->createArc(radius, gantryStart, gantryStop, dir,
                                    this->Isocentre);
        this->arcList.push_back(arc);

        this->BEVViewer->ModelRenderer->AddActor(this->arcList[i]);
        this->AxialViewer->ViewRenderer->AddActor(this->arcList[i]);
      }
      delete arcCreator;
    }

    // Add beams
    CreateObjects *beamCreator = new CreateObjects;

    for (int i = 0; i < this->ui->tableWidget->rowCount(); i++) {
      double gantryStart = this->ui->tableWidget->item(i, 5)->text().toDouble();
      double coll = this->ui->tableWidget->item(i, 8)->text().toDouble();
      double x1 = this->ui->tableWidget->item(i,
                                              10)
                      ->text()
                      .toDouble() *
                  10;  // cm to mm
      double x2 = this->ui->tableWidget->item(i, 11)->text().toDouble() *
                  10;  // cm to mm
      double y1 = this->ui->tableWidget->item(i, 12)->text().toDouble() *
                  10;  // cm to mm
      double y2 = this->ui->tableWidget->item(i, 13)->text().toDouble() *
                  10;  // cm to mm

      vtkSmartPointer<vtkActor> beam = vtkSmartPointer<vtkActor>::New();
      beam = beamCreator->createBeam(x1, x2, y1, y2, 700, 1300, gantryStart,
                                     coll, 0.0, this->Isocentre);
      this->BEVViewer->ModelRenderer->AddActor(beam);
    }

    delete beamCreator;

    this->AxialViewer->ViewRenderer->ResetCamera();
    this->BEVViewer->ModelRenderer->GetRenderWindow()->Render();
    this->AxialViewer->ViewRenderer->GetRenderWindow()->Render();

    // Move planes to isocentre
    this->ui->actionGo_To_Isocentre->trigger();
    if (myPlanReader->numOfBeams == 0) {
      throw "Error reading plan";
    }

  } catch (...) {
    QMessageBox messageBox;
    messageBox.critical(this, "Error", "Problem reading plan");
    messageBox.setFixedSize(500, 200);
  }
}

double MainWindow::CalcSSD(double *Iso, double GantryAngle,
                           vtkDataSet *BodyMesh) {
  double SSD = 0.0;
  //    //qDebug()<<this->Isocentre[0]<<this->Isocentre[1]<<this->Isocentre[2];
  //    double SrcPosForZeroGantry[3] = {this->Isocentre[0],
  //                                     this->Isocentre[1] - 1000.0,
  //                                     this->Isocentre[2]}; //src position
  //                                     in LPS
  //    double SrcPos[3];
  //    std::vector<double> SrcPosCurrentGantry;

  //    FSPBDoseCalc *Transformer = new FSPBDoseCalc;

  //    //Transform src point according to gantry angle and convert to double
  //    array from std::vector SrcPosCurrentGantry =
  //    Transformer->TransformPointXYZ(SrcPosForZeroGantry,
  //                                                         this->Isocentre,
  //                                                         0.0,
  //                                                         GantryAngle,
  //                                                         0.0);
  //    SrcPos[0] = SrcPosCurrentGantry[0];
  //    SrcPos[1] = SrcPosCurrentGantry[1];
  //    SrcPos[2] = SrcPosCurrentGantry[2];

  //    vtkSmartPointer<vtkOBBTree> OBBTree =
  //    vtkSmartPointer<vtkOBBTree>::New(); OBBTree->SetDataSet(BodyMesh);
  //    OBBTree->BuildLocator();

  //    vtkSmartPointer<vtkPoints> Points = vtkSmartPointer<vtkPoints>::New();
  //    vtkSmartPointer<vtkIdList> IDs = vtkSmartPointer<vtkIdList>::New();

  //    int Intersections = OBBTree->IntersectWithLine(SrcPos, Iso, Points,
  //    IDs);
  //    //    qDebug()<<Intersections<<":Intersections";
  //    //    qDebug()<<Points->GetNumberOfPoints()<<"Points";
  //    //    qDebug()<<"Intersection
  //    point1:"<<Points->GetPoint(0)[0]<<Points->GetPoint(0)[1]<<Points->GetPoint(0)[2];
  //    //    qDebug()<<"Intersection
  //    point2:"<<Points->GetPoint(1)[0]<<Points->GetPoint(1)[1]<<Points->GetPoint(1)[2];

  //    //Intersection point on Body
  //    double Intersection[3] = {Points->GetPoint(0)[0],
  //                              Points->GetPoint(0)[1],
  //                              Points->GetPoint(0)[2]};

  //    //Calculate SSD
  //    double SSD = std::sqrt(std::pow((SrcPos[0] - Intersection[0]), 2)
  //                           + std::pow((SrcPos[2] - Intersection[2]), 2)
  //                           + std::pow((SrcPos[1] - Intersection[1]), 2));
  //    //qDebug()<<"SSD:"<<SSD;
  return SSD;
}

void MainWindow::updateDose(const QString &path) {
  qDebug() << "Dose updated"
           << "****" << path;

  //    //Get current dose thresholds
  //    double minDose=this->AxialViewer->DoseRange[0];
  //    double maxDose=this->AxialViewer->DoseRange[1];

  //    // Read and update dose
  //    vtkSmartPointer<vtkGDCMImageReader> DoseReader =
  //        vtkSmartPointer<vtkGDCMImageReader>::New();
  //    DoseReader->SetFileName("D:\\Projects\\build-KIMView-Desktop_Qt_5_15_1_MSVC2019_64bit-Release\\Doses\\RD.dcm");
  //    DoseReader->FileLowerLeftOn(); // otherwise flips the image
  //    DoseReader->SetDataScalarTypeToDouble();
  //    DoseReader->Update();
  //    this->RTDose->DeepCopy(DoseReader->GetOutput());
  //    //qDebug()<<this->RTDose->GetScalarRange()[0]<<
  //    ""<<this->RTDose->GetScalarRange()[1]<<"Dynamic dose";

  //    this->AxialViewer->SetRTDose(this->RTDose);
  //    this->SagittalViewer->SetRTDose(this->RTDose);
  //    this->CoronalViewer->SetRTDose(this->RTDose);
  //    this->BEVViewer->RTDose = this->RTDose;

  //    this->AxialViewer->ViewRenderer->RemoveActor(this->AxialViewer->DoseSlice);
  //    this->AxialViewer->AdjustDoseRange(minDose,maxDose);
  //    this->SagittalViewer->ViewRenderer->RemoveActor(this->SagittalViewer->DoseSlice);
  //    this->SagittalViewer->AdjustDoseRange(minDose,maxDose);
  //    this->CoronalViewer->ViewRenderer->RemoveActor(this->CoronalViewer->DoseSlice);
  //    this->CoronalViewer->AdjustDoseRange(minDose,maxDose);
}

void MainWindow::on_actionArcsView_triggered() {
  // qDebug() << this->arcList.size();
  if (this->arcList.size() > 0) {
    if (this->ui->actionArcsView->isChecked()) {
      this->arcVisibility = 1;
      for (int i = 0; i < this->arcList.size(); i++) {
        this->arcList[i]->SetVisibility(true);
      }

    }

    else {
      this->arcVisibility = 0;
      for (int i = 0; i < this->arcList.size(); i++) {
        this->arcList[i]->SetVisibility(false);
      }
    }
    this->AxialViewer->ViewRenderer->GetRenderWindow()->Render();
    this->BEVViewer->ModelRenderer->GetRenderWindow()->Render();
  }
}

void MainWindow::on_actionUpdate_Dose_triggered() {
  QString DoseFile = QFileDialog::getOpenFileName(this, "Open RT Dose");

  if (DoseFile != nullptr) {
    // qDebug()<<doseFile;
    const unsigned int InputDimension = 3;
    typedef double PixelType;
    typedef itk::Image<PixelType, InputDimension> InputImageType;
    typedef itk::ImageSeriesReader<InputImageType> ReaderType;
    typedef itk::GDCMImageIO ImageIOType;
    ImageIOType::Pointer gdcmIO = ImageIOType::New();
    ReaderType::Pointer reader = ReaderType::New();

    reader->SetImageIO(gdcmIO);
    reader->SetFileName(DoseFile.toStdString());
    reader->Update();

    // qDebug()<<"Reading done!";
    using DictionaryType = itk::MetaDataDictionary;
    typedef itk::MetaDataObject<std::string> MetaDataStringType;
    const DictionaryType &dictionary = gdcmIO->GetMetaDataDictionary();

    std::string doseGridScaling = "3004|000e";
    auto doseGridScalingFactor = dictionary.Find(doseGridScaling);
    MetaDataStringType::ConstPointer doseGridScalingVal =
        dynamic_cast<const MetaDataStringType *>(
            doseGridScalingFactor->second.GetPointer());
    std::string doseGridScalingStr;
    doseGridScalingStr = doseGridScalingVal->GetMetaDataObjectValue();
    // qDebug() << std::stod(doseGridScalingStr) << "Dose grid scaling ";

    typedef itk::ImageToVTKImageFilter<InputImageType> ConnectorType;
    ConnectorType::Pointer Converter = ConnectorType::New();
    Converter->SetInput(reader->GetOutput());
    Converter->Update();

    vtkSmartPointer<vtkImageShiftScale> scaler =
        vtkSmartPointer<vtkImageShiftScale>::New();
    scaler->SetInputData(Converter->GetOutput());
    scaler->SetScale(std::stod(doseGridScalingStr));
    // Important to get accurate dose values
    scaler->SetOutputScalarTypeToDouble();
    scaler->Update();

    this->RTDose->DeepCopy(scaler->GetOutput());

    qDebug() << this->RTDose->GetScalarRange()[0]
             << this->RTDose->GetScalarRange()[1];
  }
}
