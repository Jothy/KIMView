#include "meshreader.h"

#define vtkRenderingCore_AUTOINIT 3(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingOpenGL2)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL2)

#include <gdcmReader.h>
#include<gdcmFile.h>
#include<gdcmSequenceOfItems.h>
#include<gdcmDataSet.h>
#include<gdcmTag.h>
#include<gdcmItem.h>
#include<gdcmNestedModuleEntries.h>
#include<gdcmDataElement.h>
#include<gdcmAttribute.h>

#include<vtkSmartPointer.h>
#include<vtkAppendPolyData.h>
#include<vtkCellArray.h>
#include<vtkPoints.h>
#include<vtkPolyData.h>
#include<vtkImageData.h>
#include<vtkImageCast.h>
#include<vtkLinearExtrusionFilter.h>
#include<vtkPolyDataToImageStencil.h>
#include<vtkImageStencil.h>
#include<vtkDiscreteMarchingCubes.h>
#include<vtkWindowedSincPolyDataFilter.h>
#include<vtkDecimatePro.h>
#include<vtkProperty.h>
#include<vtkActorCollection.h>
#include<vtkCutter.h>
#include<vtkPlane.h>
#include<vtkTriangleFilter.h>
#include<vtkSmoothPolyDataFilter.h>
#include<vtkProperty.h>
#include<vtkMassProperties.h>
#include<vtkDataSetSurfaceFilter.h>
#include<vtkPolyDataNormals.h>
#include<vtkCleanPolyData.h>
#include<vtkFillHolesFilter.h>


#include<QFileDialog>
#include<QDebug>
#include<QMessageBox>
#include<QTime>
#include<QProgressDialog>
#include<QCoreApplication>
#include<QString>
#include<QList>
#include<QIcon>

#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include<vtkImageResample.h>
#include<vtkImageChangeInformation.h>
#include<vtkImageAccumulate.h>
#include<vtkImageExtractComponents.h>
#include<vtkImageGaussianSmooth.h>
#include<vtkStripper.h>
#include<vtkClipPolyData.h>
#include<vtkPlane.h>


meshReader::meshReader(QWidget* parent)
{
    this->ROIActors=vtkSmartPointer<vtkActorCollection>::New();
    this->parent=parent;
}


meshReader::~meshReader()
{
    this->ROINo.clear();
    this->ROINames.clear();
    this->ROITypes.clear();
    this->meshes.clear();
    this->structFileName.clear();
    this->structSetLabel.clear();

    //qDebug()<<"Mesh reader destructed";
}

void meshReader::getStructFileName()
{
QString structFile=QFileDialog::getOpenFileName(this->parent,"Open RT Structures");
this->structFileName=structFile;
//qDebug()<<"RT struct file name:"<<structFile;

}

void meshReader::getROIMeshes(vtkImageData *imgData,float zSpacing,float targetReduction,QList<int> selectionList,QWidget* parent)
{

    gdcm::Reader RTreader;
    if(this->structFileName!=nullptr)//Check whether file is selected or not
    {
        RTreader.SetFileName(this->structFileName.toLatin1().data());

        QProgressDialog *pd=new QProgressDialog("Reading RT structs...","Abort",0,100,parent=this->parent);
        pd->setModal(true);
        pd->setRange(0,100);
        pd->setAutoClose(true);
        pd->setWindowTitle("Processing Structs...");
        //QIcon msgIcon;
        //msgIcon.addFile(QString::fromUtf8(":/RC/Icons2/Polygon.png"), QSize(), QIcon::Normal, QIcon::Off);
        //pd->setWindowIcon(msgIcon);
        pd->show();
        QCoreApplication::processEvents();

        //QTime *t;
        //qDebug()<<t->currentTime()<<"T1";

        //show error msg if file can't be read.
        if( !RTreader.Read() )
        {
            QMessageBox msgBox;
            msgBox.setText("Can't' read the file!");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
        }

        else
        {
            const gdcm::DataSet& ds = RTreader.GetFile().GetDataSet();
            //    qDebug()<<t->currentTime()<<"T2";

            gdcm::Tag tssroisq(0x3006,0x0020);//Structure set ROI sequence
            if( !ds.FindDataElement( tssroisq ) )
            {
                //qDebug() <<"Problem locating 0x3006,0x0020 - Is this a valid RT Struct file?";
                QMessageBox msgBox;
                msgBox.setText("This is not a RT structure file!");
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.exec();

            }
            gdcm::Tag troicsq(0x3006,0x0039);//ROI contour sequence
            if( !ds.FindDataElement(troicsq ))
            {
                //qDebug() << "Problem locating 0x3006,0x0039 - Is this a valid RT Struct file?";
                QMessageBox msgBox;
                msgBox.setText("This is not a RT structure file!");
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.exec();
            }

            gdcm::Tag troiobsq(0x3006,0x0080);//ROI observation sequence


            const gdcm::DataElement &roicsq = ds.GetDataElement( troicsq );
            gdcm::SmartPointer<gdcm::SequenceOfItems> sqi = roicsq.GetValueAsSQ();

            const gdcm::DataElement &ssroisq = ds.GetDataElement( tssroisq );
            gdcm::SmartPointer<gdcm::SequenceOfItems> ssqi = ssroisq.GetValueAsSQ();
            this->NumberOfROIs=sqi->GetNumberOfItems();
            //      qDebug()<< "Number of structures found:" <<  this->NumberOfROIs;

            const gdcm::DataElement &roiobsq = ds.GetDataElement( troiobsq );
            gdcm::SmartPointer<gdcm::SequenceOfItems> obsq = roiobsq.GetValueAsSQ(); //observation sequence
            //    qDebug()<< obsq->GetNumberOfItems()<<"Types";


            gdcm::Attribute<0x3006,0x0002>structLabel;
            structLabel.SetFromDataElement(ds.GetDataElement(structLabel.GetTag()));
            //qDebug()<<structLabel.GetValue()<<"Struct set label";
            this->structSetLabel=structLabel.GetValue();
            //    qDebug()<<t->currentTime()<<"Start";

            int selectionSize=selectionList.size();
            //qDebug()<<selectionSize<<"select size";

            for (int j=1;j<=selectionSize;j++)// 6 -shows Lung in dicompyler test data
            {
                int i=selectionList[j-1];
                //Get the ROI Nos.
                gdcm::Item & item=sqi->GetItem(i);
                gdcm::Attribute<0x3006,0x0084> roinumber; //ROI No. tag in ROI Contour Sequence
                const gdcm::DataSet& nestedds = item.GetNestedDataSet();
                roinumber.SetFromDataElement( nestedds.GetDataElement( roinumber.GetTag() ) );
                //qDebug()<<"ROI No.:"<<roinumber.GetValue();
                this->ROINo.append(roinumber.GetValue());

                //Get ROIs  color
                gdcm::Attribute<0x3006,0x002a> roiColor;//ROI color tag
                roiColor.SetFromDataElement( nestedds.GetDataElement( roiColor.GetTag() ));
                double R=roiColor.GetValues()[0];
                double G=roiColor.GetValues()[1];
                double B=roiColor.GetValues()[2];
                //qDebug()<<"ROi color:"<<R<<G<<B;
                this->ROIColors[j-1][0]=R; this->ROIColors[j-1][1]=G; this->ROIColors[j-1][2]=B;//C++ array starts from "0".

                //Get ROI Names
                gdcm::Item & sitem=ssqi->GetItem(i);
                gdcm::Attribute<0x3006,0x0026> roiName; //ROI Name tag in Structure Set ROI Sequence
                const gdcm::DataSet& snestedds = sitem.GetNestedDataSet();
                roiName.SetFromDataElement(snestedds.GetDataElement( roiName.GetTag()));
                //qDebug()<<roiName.GetValue()<<"ROI Name";
                this->ROINames.append(QString(roiName.GetValue()));
                pd->setLabelText(QString(roiName.GetValue()));
                QCoreApplication::processEvents();

                //Get ROI Type
                gdcm::Item & typeitem=obsq->GetItem(i);
                gdcm::Attribute<0x3006,0x00a4> roiType; //ROI Type tag in ROI observation  Sequence
                const gdcm::DataSet& obnestedds = typeitem.GetNestedDataSet();
                roiType.SetFromDataElement(obnestedds.GetDataElement( roiType.GetTag()));
                //            qDebug()<<roiType.GetValue()<<"ROI Type";
                this->ROITypes.append(QString(roiType.GetValue()));
                QCoreApplication::processEvents();


                //Get slice contours for this ROI
                gdcm::Tag contourSeq(0x3006,0x0040) ;
                if (!nestedds.FindDataElement(contourSeq))//checks whether contours are available for this structure
                {
                    //qDebug()<<"No contours available";//If not available inserts an empty mesh
                    vtkSmartPointer<vtkActor>actor =
                            vtkSmartPointer<vtkActor>::New();
                    vtkSmartPointer<vtkPolyDataMapper>mapper =
                            vtkSmartPointer<vtkPolyDataMapper>::New();
                    this->ROIActors->AddItem(actor);
                    this->meshes.push_back(mapper->GetInput());
                    this->ROITypes.append("N/A");

                }

                else
                {
                    const gdcm::DataElement &contourSeqData=nestedds.GetDataElement(contourSeq);
                    gdcm::SmartPointer<gdcm::SequenceOfItems> contourSeqItems = contourSeqData.GetValueAsSQ();
                    unsigned int contoursInROI=contourSeqItems->GetNumberOfItems();
                    //qDebug()<<"Contours in current ROI:"<<contoursInROI;


                    vtkSmartPointer<vtkAppendPolyData>appendFilter =
                            vtkSmartPointer<vtkAppendPolyData>::New();
                    for(int x=1;x<=contoursInROI;x++)
                    {
                        gdcm::Item &currentROI=contourSeqItems->GetItem(x);
                        gdcm::Tag contourDataTag(0x3006,0x0050);
                        const gdcm::DataSet& nestedds2=currentROI.GetNestedDataSet();
                        if(! nestedds2.FindDataElement(contourDataTag))
                        {
                            // qDebug()<<"No contour points found";
                        }
                        else
                        {
                            const gdcm::DataElement & currentSlice = nestedds2.GetDataElement( contourDataTag);
                            gdcm::Attribute<0x3006,0x0050> at;
                            at.SetFromDataElement(currentSlice);
                            const double* pts = at.GetValues();
                            unsigned int npts = at.GetNumberOfValues() / 3;
                            // unsigned int nxyzpts=npts*3;                     //Total no. of points including x,y,z
                            //  qDebug()<<"No. of points in the current contour:"<<npts;

                            vtkSmartPointer<vtkPoints>points =
                                    vtkSmartPointer<vtkPoints>::New();
                            vtkSmartPointer<vtkCellArray> cells =
                                    vtkSmartPointer<vtkCellArray>::New();
                            points->SetNumberOfPoints(npts+1);
                            cells->InsertNextCell(npts+1);

                            for(int p=0;p<npts;p++)
                            {
                                int x=p*3+0;int y=p*3+1;int z=p*3+2;
                                double Xp=pts[x];double Yp=pts[y];double Zp=pts[z];//x,y,z values of ith contour point
                                points->SetPoint( p, Xp, Yp,Zp );
                                cells->InsertCellPoint( p );
                            }
                            cells->InsertCellPoint(npts);
                            points->SetPoint(npts,pts[0],pts[1],pts[2]);

                            vtkSmartPointer<vtkPolyData>polydataContour=
                                    vtkSmartPointer<vtkPolyData>::New();
                            polydataContour->Initialize();
                            polydataContour->SetLines(cells);
                            polydataContour->SetPoints(points);

                            vtkSmartPointer<vtkCleanPolyData> cleanContour =
                                    vtkSmartPointer<vtkCleanPolyData>::New();
                            cleanContour->SetInputData(polydataContour);
                            //               cleanContour->SetAbsoluteTolerance(1);
                            cleanContour->Update();


                            appendFilter->AddInputData(cleanContour->GetOutput());
                        }
                    }
                    appendFilter->Update();


                    vtkSmartPointer<vtkCleanPolyData> cleanContours =
                            vtkSmartPointer<vtkCleanPolyData>::New();
                    cleanContours->SetInputData(appendFilter->GetOutput());
                    //cleanContours->SetAbsoluteTolerance(1);
                    //cleanContours->SetPointMerging(1);
                    cleanContours->Update();
                    //qDebug()<<"Done";


                    double *bounds=cleanContours->GetOutput()->GetBounds();
                    //qDebug()<<bounds[0]<<bounds[1]<<bounds[2]<<bounds[3]<<bounds[4]<<bounds[5]<<"Polydata bounds";
                    int structType=QString::compare(QString(roiType.GetValue()),"EXTERNAL",Qt::CaseInsensitive);//equal to zero if external

                    vtkSmartPointer<vtkImageData>binary_image=
                            vtkSmartPointer<vtkImageData>::New();
                    binary_image->AllocateScalars(VTK_UNSIGNED_CHAR,1);
                    //qDebug()<<structType<<"Struct type";

                    if (structType==0)//Coarser sampling(x,y=2mm &z=zSpacing) for Body to reduce execution time in vtkImageStencil
                    {
                        vtkSmartPointer<vtkImageResample> resampler=
                                vtkSmartPointer<vtkImageResample>::New();
                        resampler->SetInputData(imgData);
                        resampler->SetDimensionality(3);
                        resampler->SetAxisOutputSpacing(0,3);
                        resampler->SetAxisOutputSpacing(1,3);
                        resampler->SetAxisOutputSpacing(2,imgData->GetSpacing()[2]*1);
                        resampler->InterpolateOn();
                        resampler->Update();

                        vtkSmartPointer<vtkImageChangeInformation>InfoChanger =
                                vtkSmartPointer<vtkImageChangeInformation>::New();
                        InfoChanger->SetInputData( resampler->GetOutput());
                        InfoChanger->SetOutputSpacing(3,3,imgData->GetSpacing()[2]*1);
                        InfoChanger->Update();
                        //v16->GetOutput()->ReleaseData();

                        double  *samp_origin=InfoChanger->GetOutput()->GetOrigin();
                        double  *spacing=InfoChanger->GetOutput()->GetSpacing();
                        binary_image->SetSpacing(spacing);
                        //Use the smallest mask in which the mesh fits
                        // Add two voxels on each side to make sure the mesh fits
                        //Put the origin on a voxel to avoid small skips
                        binary_image->SetOrigin(floor((bounds[0]-samp_origin[0])/spacing[0]-2)*spacing[0]+samp_origin[0],
                                                floor((bounds[2]-samp_origin[1])/spacing[1]-2)*spacing[1]+samp_origin[1],
                                                floor((bounds[4]-samp_origin[2])/spacing[2]-2)*spacing[2]+samp_origin[2]);
                        double * origin=binary_image->GetOrigin();
                        binary_image->SetExtent(0,ceil((bounds[1]-origin[0])/spacing[0]+4),
                                                0,ceil((bounds[3]-origin[1])/spacing[1]+4),
                                                0,ceil((bounds[5]-origin[2])/spacing[2])+4);
                        binary_image->AllocateScalars(VTK_SHORT,1);
                        //Set image  voxel values to zero
                        memset(binary_image->GetScalarPointer(),0,binary_image->GetDimensions()[0]*binary_image->GetDimensions()[1]*binary_image->GetDimensions()[2]*sizeof(short));
                        // qDebug()<<"PROCESSING BODY";
                    }

                    else
                    {

                        double  *samp_origin=imgData->GetOrigin();
                        double  *spacing=imgData->GetSpacing();
                        binary_image->SetSpacing(spacing);
                        //Use the smallest mask in which the mesh fits
                        // Add two voxels on each side to make sure the mesh fits
                        //Put the origin on a voxel to avoid small skips
                        binary_image->SetOrigin(floor((bounds[0]-samp_origin[0])/spacing[0]-2)*spacing[0]+samp_origin[0],
                                floor((bounds[2]-samp_origin[1])/spacing[1]-2)*spacing[1]+samp_origin[1],
                                floor((bounds[4]-samp_origin[2])/spacing[2]-2)*spacing[2]+samp_origin[2]);
                        double * origin=binary_image->GetOrigin();
                        binary_image->SetExtent(0,ceil((bounds[1]-origin[0])/spacing[0]+4),
                                0,ceil((bounds[3]-origin[1])/spacing[1]+4),
                                0,ceil((bounds[5]-origin[2])/spacing[2])+4);
                        binary_image->AllocateScalars(VTK_SHORT,1);
                        //Set image  voxel values to zero
                        memset(binary_image->GetScalarPointer(),0,binary_image->GetDimensions()[0]*binary_image->GetDimensions()[1]*binary_image->GetDimensions()[2]*sizeof(short));
                        //qDebug()<<"PROCESSING ORGAN";

                    }


                    vtkSmartPointer<vtkPolyDataToImageStencil>sts=
                            vtkSmartPointer<vtkPolyDataToImageStencil>::New();
                    sts->SetTolerance(0);//very important
                    sts->SetInformationInput(binary_image);
                    sts->SetInputData(cleanContours->GetOutput());
                    sts->Update();


                    //qDebug()<<t->currentTime()<<"Stencil start";
                    vtkSmartPointer<vtkImageStencil>stencil=
                            vtkSmartPointer<vtkImageStencil>::New();
                    stencil->SetStencilData(sts->GetOutput());
                    stencil->SetInputData(binary_image);
                    stencil->ReverseStencilOn();
                    stencil->Update();
                    //qDebug()<<t->currentTime()<<"Stencil stop";

                    int stencilDims[6];
                    double stencilSpacing[3];
                    stencil->GetOutput()->GetDimensions(stencilDims);
                    stencil->GetOutput()->GetSpacing(stencilSpacing);


                    vtkSmartPointer<vtkDiscreteMarchingCubes>marching =
                            vtkSmartPointer<vtkDiscreteMarchingCubes>::New();
                    marching->SetInputData(stencil->GetOutput());
                    marching->SetValue(0.0,1.0);
                    marching->Update();

                    vtkSmartPointer<vtkCleanPolyData>cleanContour =
                            vtkSmartPointer<vtkCleanPolyData>::New();
                    cleanContour->SetInputConnection(marching->GetOutputPort());                    
                    cleanContour->Update();                   

                    //Triangulte before passing it through VTKDecimatePro, so the efficiency of decimateion increases
                    vtkSmartPointer<vtkTriangleFilter>triangleFilter=
                            vtkSmartPointer<vtkTriangleFilter>::New();
                    triangleFilter->SetInputConnection(cleanContour->GetOutputPort());
                    triangleFilter->Update();

                    vtkSmartPointer<vtkDecimatePro>decimate =
                            vtkSmartPointer<vtkDecimatePro>::New();
                    decimate->SetInputConnection(triangleFilter->GetOutputPort());
                    decimate->PreserveTopologyOn();//Important to preserve the original shape
                    decimate->SetTargetReduction(targetReduction);//Higher the number, lesser the no. of polygons
                    decimate->Update();

                    vtkSmartPointer<vtkWindowedSincPolyDataFilter>smoother =
                            vtkSmartPointer<vtkWindowedSincPolyDataFilter>::New();
                    smoother->SetInputConnection(decimate->GetOutputPort());
                    smoother->SetNumberOfIterations(15);//original 15
                    smoother->BoundarySmoothingOn();
                    smoother->FeatureEdgeSmoothingOff();
                    smoother->SetFeatureAngle(120);
                    smoother->SetPassBand(0.001);// originally 0.001
                    smoother->Update();

                    //For better appearance
                    vtkSmartPointer<vtkPolyDataNormals>pdNormals=
                            vtkSmartPointer<vtkPolyDataNormals>::New();
                    pdNormals->SetInputConnection(smoother->GetOutputPort());
                    pdNormals->FlipNormalsOn();
                    pdNormals->ReleaseDataFlagOn();

                    vtkSmartPointer<vtkPolyDataMapper>mapper =
                            vtkSmartPointer<vtkPolyDataMapper>::New();
                    mapper->SetInputConnection(pdNormals->GetOutputPort());
                    mapper->ScalarVisibilityOff();
                    mapper->ReleaseDataFlagOff();
                    mapper->ImmediateModeRenderingOn();
                    mapper->Update();

                    vtkSmartPointer<vtkActor>actor =
                            vtkSmartPointer<vtkActor>::New();
                    actor->SetMapper(mapper);
                    actor->GetProperty()->SetColor(R/255,G/255,B/255);
                    actor->GetProperty()->SetDiffuse(0.5);
                    actor->GetProperty()->SetSpecular(0.5);
                    actor->GetProperty()->SetSpecularPower(10);
                    actor->GetProperty()->SetInterpolationToPhong();
                    actor->GetProperty()->SetLineWidth(1.0);
                    actor->GetProperty()->SetAmbient(0.3);
                    actor->PickableOff();


                    if (structType==0)//Set the opacity of the body surface to 0.75
                    {
                        actor->GetProperty()->SetOpacity(0.5);//Hide body
                        actor->GetProperty()->SetColor(0.75,0.75,0.75);

                    }

                    this->ROIActors->AddItem(actor);

                    this->meshes.push_back(smoother->GetOutput());
                    //Set progress value
                    int progress=double(j)/double(selectionSize)*100;
                    //qDebug()<<progress<<"progress value";
                    pd->setValue(progress);
                    QCoreApplication::processEvents();

                }

                QCoreApplication::processEvents();

            }
            QCoreApplication::processEvents();
        }
        //qDebug()<<"Stop";
        pd->close();
        delete pd;
    }
}


