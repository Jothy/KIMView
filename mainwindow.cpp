#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<vtkSmartPointer.h>
#include<vtkRenderer.h>
#include<vtkPolyDataMapper.h>
#include<vtkRenderWindow.h>
#include<vtkProperty.h>
#include<vtkFileOutputWindow.h>
#include<vtkDICOMImageReader.h>
#include<vtkOutlineFilter.h>
#include<vtkPolyData.h>
#include<vtkActorCollection.h>
#include<vtkGDCMImageReader.h>
#include<vtkCamera.h>
#include<vtkRenderWindowInteractor.h>
#include<vtkInteractorStyleTrackballCamera.h>
#include<vtkCamera.h>
#include<vtkCommand.h>
#include<vtkTransformPolyDataFilter.h>
#include<vtkTransform.h>

#include "itkVersion.h"
#include "itkImage.h"
#include "itkMinimumMaximumImageFilter.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkNumericSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include <itksys/SystemTools.hxx>
#include "itkImageToVTKImageFilter.h"
#include "itkSmartPointer.h"
#include "itkCommand.h"
#include "itkMetaDataObject.h"
#include "itkCastImageFilter.h"
#include<itkMetaDataDictionary.h>

#include<gdcmTag.h>
#include<gdcmItem.h>
#include<gdcmNestedModuleEntries.h>
#include<gdcmDataElement.h>
#include<gdcmAttribute.h>

#include<QDebug>
#include<QTreeWidgetItem>
#include<QIcon>
#include<QDir>
#include<QFileDialog>
#include<QProgressDialog>
#include<QList>
#include<QInputDialog>
#include<QMdiSubWindow>
#include<QVTKWidget.h>
#include<QElapsedTimer>
#include<QMessageBox>
#include<QTime>
#include<QDockWidget>
#include<QSettings>

#include"imageviewer2d.h"
#include"rtstructreaderdialog.h"
#include"meshreader.h"
#include"createobjects.h"
#include"rangesliderdialog.h"
#include"ipconfigdialog.h"

#include<iostream>
#include<dvhdialog.h>
#include<omp.h>
#include<stdlib.h>

//Testing
#include<QTimer>
#include<vtkConeSource.h>
#include<vtkPolarAxesActor.h>
#include<vtkArcSource.h>
#include<vtkArrowSource.h>
#include<vtkGlyph3D.h>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->loadSettings();

    this->listener=new UDPListener;

    //Avoid error window popping
    vtkSmartPointer<vtkFileOutputWindow> fileOutputWindow =
            vtkSmartPointer<vtkFileOutputWindow>::New();
    fileOutputWindow->SetFileName("ErrorLog.txt" );
    vtkOutputWindow* outputWindow = vtkOutputWindow::GetInstance();
    if (outputWindow)
    {
        outputWindow->SetInstance(fileOutputWindow);
    }

    this->BeamAngles.clear();

    this->ContextMenus=new QActionGroup(this);
    this->ContextMenus->setEnabled(true);
    this->ContextMenus->setExclusive(false);

    this->ContextMenus->addAction(this->ui->actionBEV);
    this->ContextMenus->addAction(this->ui->action3DView);

    this->RTDose=vtkSmartPointer<vtkImageData>::New();

    this->ui->treeWidget->expandAll();
    this->ui->statusBar->showMessage("No data available to display");

    this->CTImage=vtkSmartPointer<vtkImageData>::New();


    this->MeshActors=vtkSmartPointer<vtkActorCollection>::New();
    this->BeamActors=vtkSmartPointer<vtkActorCollection>::New();

    //Setup BEV widget
    this->BEVViewer=new BEVWidget(this->ui->mdiAreaView,this->ContextMenus);
    //this->ui->mdiAreaView->addSubWindow(this->BEVViewer,Qt::WindowMinMaxButtonsHint|Qt::WindowTitleHint|Qt::FramelessWindowHint);//For frameless window
    this->ui->mdiAreaView->addSubWindow(this->BEVViewer,Qt::WindowMaximizeButtonHint|Qt::WindowTitleHint);
    this->BEVViewer->setWindowTitle("Model");


    //By default plan information dock widget is hidden
    this->ui->dockWidget_2->setVisible(false);
    this->ui->mdiAreaView->tileSubWindows();




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionCT_triggered()
{
    const unsigned int InputDimension = 3;
    typedef signed short PixelType;
    typedef itk::Image<PixelType,InputDimension>
            InputImageType;
    typedef itk::ImageSeriesReader< InputImageType >
            ReaderType;
    typedef itk::GDCMImageIO
            ImageIOType;
    typedef itk::GDCMSeriesFileNames
            InputNamesGeneratorType;
    ImageIOType::Pointer gdcmIO= ImageIOType::New();
    ReaderType::Pointer reader= ReaderType::New();
    std::string val1;

    try
    {
        QString imageDirName=QFileDialog::getExistingDirectory(this,"Open CT Folder","D:\\DICOM Test Patients");

        //Start reading DICOM CT data with Phase info
        /********************************************************************************/

        InputNamesGeneratorType::Pointer inputNames=
                InputNamesGeneratorType::New();
        inputNames->SetInputDirectory(imageDirName.toLatin1().data());
        gdcmIO->SetLoadPrivateTags(true);
        gdcmIO->SetLoadSequences(true);

        const ReaderType::FileNamesContainer & filenames =
                inputNames->GetInputFileNames();

        reader->SetImageIO(gdcmIO);
        reader->SetFileNames(filenames);
        reader->Update();

        //qDebug()<<"Reading done!";
        using DictionaryType = itk::MetaDataDictionary;
        typedef itk::MetaDataObject< std::string > MetaDataStringType;
        const DictionaryType & dictionary = gdcmIO->GetMetaDataDictionary();

//        //Read Patient Position tag
//        std::string PatientPosition = "0018|5100";
//        auto tagItr1 = dictionary.Find(PatientPosition);
//        MetaDataStringType::ConstPointer PatientPositionValue = dynamic_cast<const MetaDataStringType *>(tagItr1->second.GetPointer() );
//        val1 = PatientPositionValue->GetMetaDataObjectValue();
//        //qDebug()<<val1.c_str()<<" "<<val1.compare("HFS")<<" Comaprarison";
//        QString curIOP;
//        curIOP=QString::fromStdString(val1);



        //Read Patient Orientation tag, this seems more relibale
        std::string PatientOrientation = "0020|0037";
        auto tagItr = dictionary.Find(PatientOrientation);
        MetaDataStringType::ConstPointer PatientOrientationValue = dynamic_cast<const MetaDataStringType *>(tagItr->second.GetPointer() );
        std::string val2 = PatientOrientationValue->GetMetaDataObjectValue();
        qDebug()<<"DICOM Cosines: "<<val2.c_str();

        QString curIOP;
        curIOP=QString::fromStdString(val2);


        //QString supportedIOP="HFS ";//DICOM seems to be hvaving a space after as "HFS "
        QString supportedIOP="1\\0.0\\0.0\\0.0\\1\\0.0 ";//DICOM seems to be hvaving a space at the end "
        //qDebug()<<curIOP<<"**********"<<supportedIOP;


        //1==match,-1=no match
        if(curIOP==supportedIOP)
        {

            typedef itk::ImageToVTKImageFilter<InputImageType> ConnectorType;
            ConnectorType::Pointer Converter = ConnectorType::New();
            Converter->SetInput(reader->GetOutput());
            Converter->Update();
            //qDebug()<<"Conversion done!";

            this->CTImage->DeepCopy(Converter->GetOutput());

            //Display the data
            this->SagittalViewer=new ImageViewer2D(this->ui->mdiAreaView,this->ContextMenus);
            this->SagittalViewer->SetImageData(this->CTImage);
            this->SagittalViewer->SetSliceOrientation(1);
            this->SagittalViewer->SetUpView();
            this->ui->mdiAreaView->addSubWindow(this->SagittalViewer,Qt::WindowMaximizeButtonHint|Qt::WindowTitleHint);//add to make borderless window Qt::FramelessWindowHint
            this->SagittalViewer->setWindowTitle("Sagittal");
            this->SagittalViewer->show();

            this->CoronalViewer=new ImageViewer2D(this->ui->mdiAreaView,this->ContextMenus);
            this->CoronalViewer->SetImageData(this->CTImage);
            this->CoronalViewer->SetSliceOrientation(2);
            this->CoronalViewer->SetUpView();
            this->ui->mdiAreaView->addSubWindow(this->CoronalViewer,Qt::WindowMaximizeButtonHint|Qt::WindowTitleHint);
            this->CoronalViewer->setWindowTitle("Coronal");
            this->CoronalViewer->show();

            this->AxialViewer=new ImageViewer2D(this->ui->mdiAreaView,this->ContextMenus);
            this->AxialViewer->SetImageData(this->CTImage);
            this->AxialViewer->SetSliceOrientation(0);
            this->AxialViewer->SetUpView();
            this->ui->mdiAreaView->addSubWindow(this->AxialViewer,Qt::WindowMaximizeButtonHint|Qt::WindowTitleHint);
            this->AxialViewer->setWindowTitle("Axial");
            this->AxialViewer->show();

            this->ui->mdiAreaView->tileSubWindows();
            this->ui->statusBar->showMessage("CT imported sucessfully");

            QTreeWidgetItem *wItem = new QTreeWidgetItem((QTreeWidget*)nullptr, QStringList(QString("CT")));
            wItem->setCheckState(0,Qt::Checked);
            QIcon icon;
            icon.addFile(QString::fromUtf8(":/Icons/CT.png"),QSize(24,24), QIcon::Normal, QIcon::Off);
            wItem->setIcon(0,icon);
            this->ui->treeWidget->topLevelItem(0)->addChild(wItem);

            //Set default DoseVOI for RPL calculation
            double bds[6];
            this->CTImage->GetBounds(bds);

            //Enable actions
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


        }
        else
        {
            QMessageBox messageBox;
            messageBox.critical(this,"Error","Unsupported image orientation.\nOnly HFS is supported in this version.");
            messageBox.setFixedSize(500,200);
        }

    }
    catch (itk::ExceptionObject &excp)
    {
        std::cerr << "Exception thrown while reading the series" << std::endl;
        std::cerr << excp << std::endl;
    }

}

void MainWindow::on_actionStructures_triggered()
{
    if(this->CTImage->GetDimensions()[0]>0)
    {
        //Read ROIs
        RTStructReaderDialog *meshReaderDlg=new RTStructReaderDialog(this);
        meshReaderDlg->exec();


        if(meshReaderDlg->ROINames.size()>0)//Check any ROI exist or not
        {
            QList<int>selectedStructsList=meshReaderDlg->selectedItems;
            //qDebug()<<selectedStructsList[0]<<"ROI";

            meshReader* RTStructReader=new meshReader(this);
            //RTStructReader->getStructFileName();
            RTStructReader->structFileName=meshReaderDlg->structFileName;
            QCoreApplication::processEvents();
            RTStructReader->getROIMeshes(this->CTImage,this->CTImage->GetSpacing()[2],this->TargetReduction,meshReaderDlg->selectedItems,this);//Reads ROI name as well as structs
            QCoreApplication::processEvents();
            this->MeshList=RTStructReader->meshes;
            this->MeshActors=RTStructReader->ROIActors;
            this->ROIVisibleFlag=1;//structs imported


            for(int i=0;i<meshReaderDlg->selectedItems.size();i++)
            {
                this->ROIColors[i][0]=RTStructReader->ROIColors[i][0];
                this->ROIColors[i][1]=RTStructReader->ROIColors[i][1];
                this->ROIColors[i][2]=RTStructReader->ROIColors[i][2];
            }

            this->ROINum=meshReaderDlg->selectedItems.size();
            this->ROINames=RTStructReader->ROINames;
            this->ROITypes=RTStructReader->ROITypes;
            this->ROINo=RTStructReader->ROINo;

            QList<QTreeWidgetItem *> items;
            for (int i=0;i<meshReaderDlg->selectedItems.size();++i)
            {
                QTreeWidgetItem *wItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(RTStructReader->ROINames[i])));
                wItem->setCheckState(0,Qt::Checked);
                QIcon icon;
                icon.addFile(QString::fromUtf8(":/Icons/Polygon.png"),QSize(16,16),QIcon::Normal,QIcon::Off);
                wItem->setIcon(0,icon);
                wItem->setBackgroundColor(0,QColor(RTStructReader->ROIColors[i][0],
                                          RTStructReader->ROIColors[i][1],RTStructReader->ROIColors[i][2]));
                items.append(wItem);
            }
            //Add ROIs to RTSS item
            this->ui->treeWidget->topLevelItem(1)->addChildren(items);
            this->ui->treeWidget->expandAll();
            delete RTStructReader;

        }
        delete meshReaderDlg;

        //Dispaly ROIs
        this->AxialViewer->MeshList=this->MeshList;
        this->AxialViewer->ROIColors=this->ROIColors;
        this->AxialViewer->show();
        this->AxialViewer->ContourVisibility=1;
        this->AxialViewer->DisplayROIs(this->AxialViewer->SliceLoc,this->AxialViewer->SliceOrientation);
        this->AxialViewer->UpdateView();


        this->SagittalViewer->MeshList=this->MeshList;
        this->SagittalViewer->ROIColors=this->ROIColors;
        this->SagittalViewer->show();
        this->SagittalViewer->ContourVisibility=1;
        this->SagittalViewer->DisplayROIs(this->SagittalViewer->SliceLoc,this->SagittalViewer->SliceOrientation);
        this->SagittalViewer->UpdateView();

        this->CoronalViewer->MeshList=this->MeshList;
        this->CoronalViewer->ROIColors=this->ROIColors;
        this->CoronalViewer->show();
        this->CoronalViewer->ContourVisibility=1;
        this->CoronalViewer->DisplayROIs(this->CoronalViewer->SliceLoc,this->CoronalViewer->SliceOrientation);
        this->CoronalViewer->UpdateView();

        this->ui->action3DView->trigger();
    }
    else
    {
        QMessageBox messageBox;
        messageBox.critical(this,"Error","Please load a CT image before loading structures");
        messageBox.setFixedSize(500,200);
    }


}

void MainWindow::on_actionDose_triggered()
{
    if(this->CTImage->GetDimensions()[0]>0)
    {

        QString DoseFile=QFileDialog::getOpenFileName(this,"Open RT Dose");
        if(DoseFile!=nullptr)
        {
            //qDebug()<<doseFile;
            vtkSmartPointer <vtkGDCMImageReader>DoseReader=
                    vtkSmartPointer<vtkGDCMImageReader>::New();
            DoseReader->SetFileName(DoseFile.toLatin1());
            DoseReader->FileLowerLeftOn();//otherwise flips the image
            DoseReader->SetDataScalarTypeToDouble();
            DoseReader->Update();
            this->RTDose->DeepCopy(DoseReader->GetOutput());
            //qDebug()<<this->RTDose->GetScalarRange()[0]<<":Min"<<this->RTDose->GetScalarRange()[1]<<":Max";
        }
        else
        {
            QMessageBox messageBox;
            messageBox.critical(this,"Error","An error has occured !");
            messageBox.setFixedSize(500,200);
            messageBox.show();
        }

        this->AxialViewer->SetRTDose(this->RTDose);
        this->SagittalViewer->SetRTDose(this->RTDose);
        this->CoronalViewer->SetRTDose(this->RTDose);
        this->BEVViewer->RTDose=this->RTDose;

        this->AxialViewer->DoseVisibility=true;
        this->AxialViewer->UpdateView();
        this->SagittalViewer->DoseVisibility=true;
        this->SagittalViewer->UpdateView();
        this->CoronalViewer->DoseVisibility=true;
        this->CoronalViewer->UpdateView();

        this->ui->statusBar->showMessage("Dose imported sucessfully");

        double *DoseOrg=this->RTDose->GetOrigin();
        //qDebug()<<DoseOrg[0]<<DoseOrg[1]<<DoseOrg[2]<<"Dose Origin";

        double *ImgOrg=this->CTImage->GetOrigin();
        //qDebug()<<ImgOrg[0]<<ImgOrg[1]<<ImgOrg[2]<<"Image Origin";
    }
    else
    {
        QMessageBox messageBox;
        messageBox.critical(this,"Error","Please load a CT image before loading dose");
        messageBox.setFixedSize(500,200);
    }




}

void MainWindow::on_actionGo_To_Isocentre_triggered()
{

    QList<QMdiSubWindow *>SubWindows = this->ui->mdiAreaView->subWindowList();
    if(SubWindows[1])
    {
        ImageViewer2D *Viewer = qobject_cast<ImageViewer2D*>(SubWindows[1]->widget());
        if(Viewer->SliceOrientation==0)
        {
            Viewer->MoveToLocation(this->Isocentre[2]);

        }

        else if(Viewer->SliceOrientation==1)
        {
            Viewer->MoveToLocation(this->Isocentre[0]);

        }


        else if(Viewer->SliceOrientation==2)
        {
            Viewer->MoveToLocation(this->Isocentre[1]);

        }
    }

    if(SubWindows[2])
    {
        ImageViewer2D *Viewer = qobject_cast<ImageViewer2D*>(SubWindows[2]->widget());
        if(Viewer->SliceOrientation==0)
        {
            Viewer->MoveToLocation(this->Isocentre[2]);

        }

        else if(Viewer->SliceOrientation==1)
        {
            Viewer->MoveToLocation(this->Isocentre[0]);

        }


        else if(Viewer->SliceOrientation==2)
        {
            Viewer->MoveToLocation(this->Isocentre[1]);

        }

    }

    if(SubWindows[3])
    {
        ImageViewer2D *Viewer = qobject_cast<ImageViewer2D*>(SubWindows[3]->widget());
        if(Viewer->SliceOrientation==0)
        {
            Viewer->MoveToLocation(this->Isocentre[2]);

        }

        else if(Viewer->SliceOrientation==1)
        {
            Viewer->MoveToLocation(this->Isocentre[0]);

        }


        else if(Viewer->SliceOrientation==2)
        {
            Viewer->MoveToLocation(this->Isocentre[1]);

        }

    }
}

void MainWindow::on_actionBEV_triggered()
{
    this->ui->tableWidget->setFocus();

    //qDebug()<<"No. of beams:"<<this->ui->tableWidget->rowCount();

    if(!(this->ui->tableWidget->rowCount()==0))
    {

        //By default show the first beam if none selected
        int SelectedBeam=0;
        SelectedBeam=this->ui->tableWidget->currentRow();
        double gantryAngle=this->ui->tableWidget->item(SelectedBeam,2)->text().toDouble();
        //qDebug()<<gantryAngle<<":Selected Beam Angle";

        //Display the data
        this->BEVViewer->MeshActors=this->MeshActors;
        this->BEVViewer->ROIColors=this->ROIColors;
        this->BEVViewer->Isocentre=this->Isocentre;
        this->BEVViewer->GantryAngle=gantryAngle;        
        this->BEVViewer->DisplayBEV();
        this->BEVViewer->show();

    }

    else
    {
        QMessageBox messageBox;
        messageBox.critical(this,"Error","No beam exist. Please add a beam to view BEV.");
        messageBox.setFixedSize(500,200);
    }

    //this->ui->mdiAreaView->tileSubWindows();

}

void MainWindow::on_action3DView_triggered()
{
    this->ui->tableWidget->setFocus();
    //Display the data
    this->BEVViewer->MeshActors=this->MeshActors;
    this->BEVViewer->BeamActors=this->BeamActors;
    this->BEVViewer->ROIColors=this->ROIColors;
    this->BEVViewer->DisplayMeshes();
    this->BEVViewer->DisplayBeams();

    this->InteractorTrackball=vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    this->BEVViewer->ModelRenderer->GetRenderWindow()->GetInteractor()->SetInteractorStyle(this->InteractorTrackball);
    this->BEVViewer->show();
    this->BEVViewer->ModelRenderer->GetRenderWindow()->Render();


}

void MainWindow::on_actionClose_Patient_triggered()
{
    QApplication::closeAllWindows();
}

void MainWindow::on_actionReset_Zoom_triggered()
{

    this->AxialViewer->ViewRenderer->ResetCamera();
    this->AxialViewer->ViewRenderer->GetRenderWindow()->Render();

    this->SagittalViewer->ViewRenderer->ResetCamera();
    this->SagittalViewer->ViewRenderer->GetRenderWindow()->Render();

    this->CoronalViewer->ViewRenderer->ResetCamera();
    this->CoronalViewer->ViewRenderer->GetRenderWindow()->Render();

}

void MainWindow::on_actionShowBeams_triggered()
{
    this->AxialViewer->TriggerActionShowBeams();
    this->SagittalViewer->TriggerActionShowBeams();
    this->CoronalViewer->TriggerActionShowBeams();
}

void MainWindow::on_actionShowDose_triggered()
{
    this->AxialViewer->TriggerActionShowDose();
    this->SagittalViewer->TriggerActionShowDose();
    this->CoronalViewer->TriggerActionShowDose();

    if(this->AxialViewer->DoseVisibility==1)
    {
        this->BEVViewer->IsodoseSurface->VisibilityOn();
        //qDebug()<<"Dose on";

    }
    else
    {
        this->BEVViewer->IsodoseSurface->VisibilityOff();
         //qDebug()<<"Dose off";
    }
    this->BEVViewer->ModelRenderer->GetRenderWindow()->Render();

}

void MainWindow::on_actionShowContours_triggered()
{
    this->AxialViewer->TriggerActionShowContours();
    this->SagittalViewer->TriggerActionShowContours();
    this->CoronalViewer->TriggerActionShowContours();

}

void MainWindow::on_actionZoom_In_All_triggered()
{
    //qDebug()<<"Zooming in...";
    this->AxialViewer->ViewRenderer->GetActiveCamera()->Zoom(1.1);
    this->AxialViewer->ViewRenderer->GetRenderWindow()->Render();


    this->SagittalViewer->ViewRenderer->GetActiveCamera()->Zoom(1.1);
    this->SagittalViewer->ViewRenderer->GetRenderWindow()->Render();

    this->CoronalViewer->ViewRenderer->GetActiveCamera()->Zoom(1.1);
    this->CoronalViewer->ViewRenderer->GetRenderWindow()->Render();
}

void MainWindow::on_actionZoom_Out_All_triggered()
{
    //qDebug()<<"Zooming out...";
    this->AxialViewer->ViewRenderer->GetActiveCamera()->Zoom(0.9);
    this->AxialViewer->ViewRenderer->GetRenderWindow()->Render();


    this->SagittalViewer->ViewRenderer->GetActiveCamera()->Zoom(0.9);
    this->SagittalViewer->ViewRenderer->GetRenderWindow()->Render();

    this->CoronalViewer->ViewRenderer->GetActiveCamera()->Zoom(0.9);
    this->CoronalViewer->ViewRenderer->GetRenderWindow()->Render();
}

void MainWindow::on_actionInformation_triggered()
{
    if(this->ui->actionInformation->isChecked())
    {
        this->ui->dockWidget->setVisible(true);
    }

    else
    {
        this->ui->dockWidget->setVisible(false);
    }

    this->ui->mdiAreaView->tileSubWindows();
}

void MainWindow::on_actionPlan_Information_triggered()
{
    if(this->ui->actionPlan_Information->isChecked())
    {
        this->ui->dockWidget_2->setVisible(true);
    }

    else
    {
        this->ui->dockWidget_2->setVisible(false);
    }

    this->ui->mdiAreaView->tileSubWindows();
}

void MainWindow::on_actionCalc_DVH_triggered()
{


    //qDebug()<<this->rtDoseCube->GetScalarRange()[1];
        if(this->RTDose->GetScalarRange()[1]==1)
        {
            QMessageBox* msgBox=new QMessageBox(this);
            msgBox->setText("Dose not available.");
            msgBox->setIcon(QMessageBox::Critical);
            msgBox->setWindowTitle("Error");
            msgBox->exec();
            delete msgBox;
        }

        else
        {

            DVHDialog *myDVHDialog=new DVHDialog(this);
            myDVHDialog->doseMatrix=this->RTDose;
            myDVHDialog->maxDose=this->RTDose->GetScalarRange()[1];
            myDVHDialog->meshes=this->MeshList;
            myDVHDialog->setROIColors(this->ROIColors);
            myDVHDialog->setStructureNames(this->ROINames);
            myDVHDialog->ROIType=this->ROITypes;
            myDVHDialog->ROINo=this->ROINo;

            myDVHDialog->exec();
            delete myDVHDialog;
        }




}

void MainWindow::on_actionAdjust_Range_triggered()
{
    if(this->RTDose->GetScalarRange()[1]==1.0)
    {
        QMessageBox* msgBox=new QMessageBox(this);
        msgBox->setText("Dose not available.");
        msgBox->setIcon(QMessageBox::Critical);
        msgBox->setWindowTitle("Error");
        msgBox->exec();
        delete msgBox;
    }
    else
    {

        RangeSliderDialog *ranger=new RangeSliderDialog(this);
        ranger->minDose=this->RTDose->GetScalarRange()[0];
        ranger->maxDose=this->RTDose->GetScalarRange()[1];
        //Need to set viewers first before SetDoseRange, or else crashes
        ranger->AxialViewer=this->AxialViewer;
        ranger->SagittalViewer=this->SagittalViewer;
        ranger->CoronalViewer=this->CoronalViewer;
        ranger->ModelViewer=this->BEVViewer;
        ranger->SetDoseRange();
        ranger->show();
    }
}

void MainWindow::on_actionReset_WL_WW_triggered()
{
    this->AxialViewer->TriggerReset_WL_WW();
    this->SagittalViewer->TriggerReset_WL_WW();
    this->CoronalViewer->TriggerReset_WL_WW();
}

void MainWindow::on_actionRender_Bones_triggered()
{

}

void MainWindow::on_actionHello_UDP_triggered()
{
    if(this->ui->actionHello_UDP->isChecked()==true)
    {
        this->listener->TrackingTarget->DeepCopy(this->MeshList[0]);
        this->listener->AxialViewer=this->AxialViewer;
        this->listener->SagittalViewer=this->SagittalViewer;
        this->listener->CoronalViewer=this->CoronalViewer;
        this->listener->BEVViewer=this->BEVViewer;
        this->listener->StartListening();
        this->ui->statusBar->showMessage("Listening to UPD sender...");
     }

    else
    {
        this->listener->StopListening();
        this->ui->statusBar->clearMessage();
        QApplication::processEvents();

    }
}

void MainWindow::on_actionMove_ROI_triggered()
{

    //double XYZ[3]={-5,10,20};
    QString text = QInputDialog::getText(this, tr("Enter shifts (XYZ)"),
                                             tr("X,Y,Z:"), QLineEdit::Normal,
                                             "0,0,0");
    double x=text.split(',')[0].toDouble();
    double y=text.split(',')[1].toDouble();
    double z=text.split(',')[2].toDouble();

    QElapsedTimer timer;
    timer.start();


   qDebug() <<"Rendering took" << timer.elapsed() << "milliseconds";
   QMessageBox messageBox;
   messageBox.information(this,"Error",QString::number(timer.elapsed()));
   messageBox.show();



}

void MainWindow::on_actionRotate_ROI_triggered()
{
//    //Create a cone
//    vtkSmartPointer<vtkConeSource> coneSource1 =
//            vtkSmartPointer<vtkConeSource>::New();
//    coneSource1->SetHeight(10.0);
//    coneSource1->Update();


//    vtkSmartPointer<vtkTransform> transform =
//            vtkSmartPointer<vtkTransform>::New();
//    //transform->RotateWXYZ(double angle, double x, double y, double z);
//    transform->RotateWXYZ(90,0.5,0.5,0.5);

//    vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter =
//            vtkSmartPointer<vtkTransformPolyDataFilter>::New();
//    transformFilter->SetTransform(transform);
//    transformFilter->SetInputConnection(coneSource1->GetOutputPort());
//    transformFilter->Update();

//    //Create a mapper and actor
//    vtkSmartPointer<vtkPolyDataMapper> mapper1 =
//            vtkSmartPointer<vtkPolyDataMapper>::New();
//    mapper1->SetInputConnection(coneSource1->GetOutputPort());

//    vtkSmartPointer<vtkActor> actor1 =
//            vtkSmartPointer<vtkActor>::New();
//    actor1->SetMapper(mapper1);
//    actor1->GetProperty()->SetColor(1,0,0);

//    //TRansformed actor
//    //Create a mapper and actor
//    vtkSmartPointer<vtkPolyDataMapper> mapper2 =
//            vtkSmartPointer<vtkPolyDataMapper>::New();
//    mapper2->SetInputConnection(transformFilter->GetOutputPort());

//    vtkSmartPointer<vtkActor> actor2 =
//            vtkSmartPointer<vtkActor>::New();
//    actor2->SetMapper(mapper2);
//    actor2->GetProperty()->SetColor(0,1,0);

//    //Create a renderer, render window, and interactor
//    vtkSmartPointer<vtkRenderer> renderer =
//            vtkSmartPointer<vtkRenderer>::New();
//    vtkSmartPointer<vtkRenderWindow> renderWindow =
//            vtkSmartPointer<vtkRenderWindow>::New();
//    renderWindow->AddRenderer(renderer);
//    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor=
//            vtkSmartPointer<vtkRenderWindowInteractor>::New();
//    renderWindowInteractor->SetRenderWindow(renderWindow);

//    //Add the actors to the scene
//    renderer->AddActor(actor1);
//    renderer->AddActor(actor2);
//    renderer->SetBackground(0,0,0); // Background color dark red

//    //Render and interact
//    renderWindow->Render();
//    renderWindowInteractor->Start();



}

void MainWindow::on_actionAdd_Arc_triggered()
{
    double radius=400;
    double xCord1=radius*cos(vtkMath::RadiansFromDegrees(0.0));
    double yCord1=radius*sin(vtkMath::RadiansFromDegrees(0.0));
    double xCord2=radius*cos(vtkMath::RadiansFromDegrees(359.9));
    double yCord2=radius*sin(vtkMath::RadiansFromDegrees(359.9));

    vtkSmartPointer<vtkArcSource>arcSource=
            vtkSmartPointer<vtkArcSource>::New();
    //arcSource->SetAngle(90);
    arcSource->SetResolution(360);
    arcSource->SetPoint1(xCord1,yCord1,0);
    arcSource->SetPoint2(xCord2,yCord2,0);
    arcSource->NegativeOn();
    arcSource->UseNormalAndAngleOff();
    arcSource->Update();


    // Visualize
    vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(arcSource->GetOutputPort());

    vtkSmartPointer<vtkActor> arcActor =
            vtkSmartPointer<vtkActor>::New();
    arcActor->SetMapper(mapper);
    arcActor->GetProperty()->SetColor(1,1,0);
    arcActor->GetProperty()->SetLineWidth(2.0);
    arcActor->SetPosition(-12,126,-32);
    arcActor->RotateZ(-90);


    //Add arrow
    //Create an arrow.
    vtkSmartPointer<vtkArrowSource>arrowSource =
            vtkSmartPointer<vtkArrowSource>::New();
    arrowSource->SetShaftRadius(50);
    arrowSource->SetTipLength(125);

    vtkSmartPointer<vtkPolyDataMapper>arrowMapper=
            vtkSmartPointer<vtkPolyDataMapper>::New();
    arrowMapper->SetInputData(arrowSource->GetOutput());

    vtkSmartPointer<vtkActor>arrowActor=
            vtkSmartPointer<vtkActor>::New();
    arrowActor->SetMapper(arrowMapper);
    arrowActor->GetProperty()->SetColor(1,0,0);
    arrowActor->SetPosition(0,0,0);

    vtkSmartPointer<vtkActor>actor=
            vtkSmartPointer<vtkActor>::New();
    actor=arcActor;
    actor->SetPosition(-12,126,0);

    this->BEVViewer->ModelRenderer->AddViewProp(arcActor);
    this->BEVViewer->ModelRenderer->AddViewProp(arrowActor);
    this->BEVViewer->show();
    this->BEVViewer->ModelRenderer->GetRenderWindow()->Render();

    this->AxialViewer->ViewRenderer->AddActor(actor);
    this->AxialViewer->ViewRenderer->GetRenderWindow()->Render();



}

void MainWindow::on_actionSend_UDP_triggered()
{
//    this->listener->TrackingTarget->DeepCopy(this->MeshList[0]);
//    this->listener->AxialViewer=this->AxialViewer;
//    this->listener->SagittalViewer=this->SagittalViewer;
//    this->listener->CoronalViewer=this->CoronalViewer;
//    this->listener->BEVViewer=this->BEVViewer;
    this->listener->StartListening();
    QApplication::processEvents();

}


void MainWindow::on_actionAbout_QT_triggered()
{
    QMessageBox::aboutQt(this);
    QSettings settings;
    qDebug()<<settings.value("KIMPort").toString()<<" :KIMIP";

}



void MainWindow::on_actionIP_COnfiguration_triggered()
{

    IPConfigDialog *IPDialog=new IPConfigDialog(this);
    IPDialog->exec();
    delete  IPDialog;
}


void MainWindow::loadSettings()
{
   QSettings settings;
   settings.setValue("KIMIP","127.0.0.1");
   settings.setValue("KIMPort",52364);
   settings.setValue("KIMViewPort",45617);



}
























