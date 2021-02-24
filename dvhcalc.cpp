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

#include "dvhcalc.h"

#include<vtkPolyData.h>
#include<vtkImageData.h>
#include<vtkPolyDataToImageStencil.h>
#include<vtkImageAccumulate.h>
#include<vtkImageStencil.h>
#include<vtkLinearExtrusionFilter.h>
#include<vtkSmartPointer.h>
#include<vtkAppendPolyData.h>
#include<vtkPlane.h>
#include<vtkCutter.h>
#include<vtkMassProperties.h>
#include<vtkImageResample.h>
#include<vtkImageChangeInformation.h>
#include<vtkImageIterator.h>
#include<vtkMath.h>
#include<vtkImageExport.h>
#include<vtkTriangleFilter.h>
#include<vtkStripper.h>
#include<vtkImageReslice.h>

#include<QDebug>
#include<QInputDialog>
#include <QObject>
#include<QStringList>
#include<QString>
#include<QList>
#include<QTime>
#include<QRunnable>

#include<algorithm>
#include<numeric>
#include<iterator>
#include<omp.h>



DVHCalc::DVHCalc()
{
//    this->resampledImage=vtkSmartPointer<vtkImageData>::New();
}


DVHCalc::~DVHCalc()
{
    this->indicesX.clear();
    this->indicesY.clear();
    this->indicesZ.clear();
    this->doseValues.clear();
    this->doseBins.clear();
    this->volBins.clear();
    this->cumVolume.clear();
}


std::vector<double> DVHCalc::calcStructDVH(vtkPolyData *structure,vtkImageData *doseGrid)
{
    double bounds[6];
    structure->GetBounds(bounds);
    double doseBounds[6];
    doseGrid->GetBounds(doseBounds);
    int doseDims[3];
    doseGrid->GetDimensions(doseDims);
    double doseSpc[3];
    doseGrid->GetSpacing(doseSpc);
    //qDebug()<<bounds[0]<<bounds[1]<<bounds[2]<<bounds[3]<<bounds[4]<<bounds[5]<<"Bounds";
    //qDebug()<<doseBounds[0]<<doseBounds[1]<<doseBounds[2]<<doseBounds[3]<<doseBounds[4]<<doseBounds[5]<<"DoseBounds";
    this->getStructBoundsInMatrixCoords(doseBounds,bounds,doseSpc[0],doseSpc[1],doseSpc[2],doseDims[0],doseDims[1],doseDims[2]);

    vtkSmartPointer<vtkPolyDataToImageStencil>sts2=
            vtkSmartPointer<vtkPolyDataToImageStencil>::New();
    sts2->SetTolerance(0);//very important
    sts2->SetInformationInput(doseGrid);
    sts2->SetOutputSpacing(doseGrid->GetSpacing());
    sts2->SetOutputOrigin(doseGrid->GetOrigin());
    sts2->SetInputData(structure);
    sts2->Update();

    vtkSmartPointer<vtkImageStencil>stencil2=
            vtkSmartPointer<vtkImageStencil>::New();
    stencil2->SetStencilData(sts2->GetOutput());
    stencil2->SetInputData(doseGrid);
    stencil2->ReverseStencilOff();
    stencil2->SetBackgroundValue(0);    
    stencil2->Update();

   vtkSmartPointer<vtkImageAccumulate>ia2=
           vtkSmartPointer<vtkImageAccumulate>::New();
   ia2->SetInputConnection(stencil2->GetOutputPort());
   ia2->IgnoreZeroOn();  
   ia2->Update();
   long vc2=ia2->GetVoxelCount();
   double maxDose=ia2->GetMax()[0];//1st component's value
   double minDose=ia2->GetMin()[0];
   double meanDose=ia2->GetMean()[0];
   double std=ia2->GetStandardDeviation()[0];
   //qDebug()<<maxDose<<minDose<<"Max&Min";
   //qDebug()<<vc2<<"voxel count";

   int stencilDims[6];
   double stencilSpacing[3];
   stencil2->GetOutput()->GetDimensions(stencilDims);
   stencil2->GetOutput()->GetSpacing(stencilSpacing);
   double voxelVol=(stencilSpacing[0]*stencilSpacing[1]*stencilSpacing[2])/1000;//in cc
   // qDebug()<<stencilSpacing[0]<<stencilSpacing[1]<<stencilSpacing[2]<<"stencil spc";
   //qDebug()<<stencil2->GetOutput()->GetDimensions()[0]<<stencil2->GetOutput()->GetDimensions()[1]
   //<<stencil2->GetOutput()->GetDimensions()[2];
   //qDebug()<<vc2<<"voxel count";
   double vol=(vc2 *voxelVol);//in cc
   std::vector<double> result;
   result.push_back(vol);//0
   result.push_back(maxDose);//1
   result.push_back(meanDose);//2
   result.push_back(minDose);//3
   result.push_back(std);//4
   result.push_back(voxelVol);//5   
   //qDebug()<<vol<<"Volume from DVH";
   this->getIndicesOfOnes(stencil2->GetOutput());
   this->getDoseValues(this->indicesX,this->indicesY,this->indicesZ,doseGrid);  
   return result;

   }


//Returns the indices of the voxels inside the given structure
void DVHCalc::getIndicesOfOnes(vtkImageData *imgData)
{    
    int dims[6];
    dims[0]=this->structBoundsInMatrixCoords[0];
    dims[1]=this->structBoundsInMatrixCoords[1];
    dims[2]=this->structBoundsInMatrixCoords[2];
    dims[3]=this->structBoundsInMatrixCoords[3];
    dims[4]=this->structBoundsInMatrixCoords[4];
    dims[5]=this->structBoundsInMatrixCoords[5];

    for(int x=dims[0];x<dims[1];x++)
    {
        for(int y=dims[2];y<dims[3];y++)
        {

            for (int z=dims[4];z<dims[5];z++)
            {
                double* pix=static_cast<double*>(imgData->GetScalarPointer(x,y,z));
                //double* pix=(double*)imgData->GetScalarPointer(x,y,z);
                //if(imgData->GetScalarComponentAsFloat(x,y,z,0)>0)
                if(pix[0]>0)
                {
                    this->indicesX.push_back(x);
                    this->indicesY.push_back(y);
                    this->indicesZ.push_back(z);
                    //qDebug()<<x<<y<<z<<pix[0];
                }

            }
        }
    }
//qDebug()<<this->indicesX.size()<<"Index size";
}


void DVHCalc::getDoseValues(std::vector<int> indicesX,std::vector<int> indicesY,std::vector<int> indicesZ,vtkImageData* doseGrid)
{    
    double* pixel;
    this->doseValues.clear();

    //#pragma omp parallel
    //x,y,z all have the same size, so just get the size of one
    for (int i=0;i<indicesX.size();i++)
    {
        pixel=static_cast<double*>(doseGrid->GetScalarPointer(indicesX[i],indicesY[i],indicesZ[i]));
         //pixel=(double*)doseGrid->GetScalarPointer(indicesX[i],indicesY[i],indicesZ[i]);
        this->doseValues.push_back(pixel[0]);
    }
}


std::vector<float> DVHCalc::getClonogensAsVector(vtkImageData *clonogenGrid)
{
    std::vector<float>clonogens;
    int indexSize=this->indicesX.size();//x,y,z all have the same size, so just get the size of one
    int clonogenExt[6];
    clonogenGrid->GetExtent(clonogenExt);
    float* pixel;


    for (int i=0;i<indexSize;i++)
    {
//        //If pixel index outside dose grid set the value to zero (can be some lower threshold too)
//        //This avoids program crash in simulating the errros from gauss error distribution where the
//        //pixels could be outside the dosegrid
//        if(this->indicesX[i]<0 || this->indicesY[i]<0 || this->indicesZ[i]<0)
//        {
//            clonogens.push_back(0.0);
//            qDebug()<<"Pixel outside dose grid (-ve)";
//        }

//        else if(this->indicesX[i]>clonogenExt[1] ||this-> indicesY[i]>clonogenExt[3] || this->indicesZ[i]>clonogenExt[5])
//        {
//            clonogens.push_back(0.0);
//            qDebug()<<"Pixel outside dose grid(+ve)";
//        }
//        else
//        {
            pixel=static_cast<float*>(clonogenGrid->GetScalarPointer(this->indicesX[i],this->indicesY[i],this->indicesZ[i]));
            clonogens.push_back(pixel[0]);
            //qDebug()<<pixel[0]<<"Pixel[0]"<<i;
//        }

    }

    return clonogens;

}


std::vector<double> DVHCalc::getDoseValuesAsVector(std::vector<int> indicesX,std::vector<int> indicesY,std::vector<int> indicesZ, vtkImageData *doseGrid)
{
//   QTime*t;
//   qDebug()<<t->currentTime()<<"Get dose values-start";
   std::vector<double>doseValues;
   int indexSize=indicesX.size();//x,y,z all have the same size, so just get the size of one
   int doseExt[6];
   doseGrid->GetExtent(doseExt);
   double* pixel;
   //qDebug()<<doseExt[1]<<doseExt[3]<<doseExt[5]<<"Dose Exts";


   for (int i=0;i<indexSize;i++)
   {
       //If pixel index outside dose grid set the value to zero (can be some lower threshold too)
       //This avoids program crash in simulating the errros from gauss error distribution where the
       //pixels could be outside the dosegrid
       if(indicesX[i]<0 || indicesY[i]<0 || indicesZ[i]<0)
       {
           doseValues.push_back(0.0);
           //qDebug()<<"Pixel outside dose grid (-ve)";
       }

       else if(indicesX[i]>doseExt[1] || indicesY[i]>doseExt[3] || indicesZ[i]>doseExt[5])
       {
           doseValues.push_back(0.0);
           //qDebug()<<"Pixel outside dose grid(+ve)";
       }
       else
       {
           pixel=static_cast<double*>(doseGrid->GetScalarPointer(indicesX[i],indicesY[i],indicesZ[i]));
           //pixel=(double*)doseGrid->GetScalarPointer(indicesX[i],indicesY[i],indicesZ[i]);
           doseValues.push_back(pixel[0]);
       }

   }

   return doseValues;

}


void DVHCalc::resampleImage(vtkImageData*imgData,double spx,double spy,double spz,int interpolationFlag)
{
    vtkSmartPointer<vtkImageResample> resampler=
            vtkSmartPointer<vtkImageResample>::New();
    resampler->SetInputData(imgData);
    resampler->SetDimensionality(3);
    resampler->SetAxisOutputSpacing(0,spx);
    resampler->SetAxisOutputSpacing(1,spy);
    resampler->SetAxisOutputSpacing(2,spz);    
    if(interpolationFlag==0)
    {
        resampler->SetInterpolationModeToLinear();
    }
    else if(interpolationFlag==1)
    {
        resampler->SetInterpolationModeToCubic();
    }
    else if(interpolationFlag==2)
    {
        resampler->SetInterpolationModeToNearestNeighbor();
    }    
    resampler->Update();    
    //qDebug()<<resampler->GetOutput()->GetScalarTypeAsString()<<"Resampled dose type";

    //Required to update image data information
   vtkSmartPointer<vtkImageChangeInformation>v16 =
           vtkSmartPointer<vtkImageChangeInformation>::New();
    v16->SetInputConnection(resampler->GetOutputPort());
    v16->SetOutputSpacing(spx,spy,spz);
    v16->Update();   
    this->resampledImage= v16->GetOutput();
//    qDebug()<<this->resampledImage->GetDimensions()[0]<<this->resampledImage->GetDimensions()[1]<<
//    this->resampledImage->GetDimensions()[2]<<"Resampled dose dims";
   }


void DVHCalc::diffToCumulative(std::vector<double> doseBins,std::vector<double>volBins)
{
    double totalVol=std::accumulate(volBins.begin(),volBins.end(),0.0);

    double* cumVol=new double[volBins.size()];


    std::fill_n(cumVol,volBins.size(),0);//Initialize array to zero, otherwise filled with garbage values
    cumVol[0]=volBins[0];

    for (int i=1;i<volBins.size();++i)
    {       
        cumVol[i]=cumVol[i-1]+volBins[i];

    }

    //Convert to percentage volume
    for (int i=0;i<volBins.size();++i)
    {
        this->cumVolume.push_back((totalVol-cumVol[i])/totalVol*100);
        //qDebug()<<(totalVol-cumVol[i])/totalVol*100<<"cum vol";

    } 

    delete[] cumVol;

}


 void DVHCalc::histogramData(int bins,std::vector<double>data,double voxelVolume,bool dvhType)
{
     double* frequency=new double[bins];
     std::fill_n(frequency,bins,0.0);//Initialize array to zero, otherwise filled with garbage values
     int dataSize=data.size();
     //qDebug()<<dataSize<<"size";
     double doseMax=*std::max_element(data.begin(),data.end());//pointer is important
     double doseMin=*std::min_element(data.begin(),data.end());//pointer is important
     double doseInterval=doseMax-doseMin;
     double eachBinSize=doseInterval/bins;
     //qDebug()<<doseMin<<doseMax<<doseInterval<<eachBinSize<<voxelVolume<<"Histogram values";

     double min,max,binCentre;
     for (int i=0;i<bins;++i)
    {
        min=(i*eachBinSize)+doseMin;
        max=(i*eachBinSize)+(eachBinSize+doseMin);
        binCentre=min+(max-min)/2.0;
//        qDebug()<<min<<max;
        for (int k=0;k<dataSize;k++)
        {
            //to avoid rounding off errors,(sometimes the max dose will be 0.0001 less than the actual max dose
            //and voxels in a more uniform dose distribution will be avoided)
            if(i==(bins-1))//last bin
            {
                max=doseMax;
            }
            if (data[k]>=min && data[k]<=max)
            {
                frequency[i]++;
            }

        }

        this->doseBins.push_back(binCentre);
        //qDebug()<<binCentre<<"Bin Center"<<i<<bins;
        if(dvhType==0)//Cumulative
        {
            this->volBins.push_back(frequency[i]*voxelVolume);
        }
        else if(dvhType==1)//Differential
        {
            this->volBins.push_back(frequency[i]);
            //qDebug()<<frequency[i]<<"Frequency";
        }

    }

//    //For testing
//    for (int i=0;i<bins;i++)
//    {
//        qDebug()<<this->doseBins[i]<<this->volBins[i];
//    }

}


 void DVHCalc::getStructBoundsInMatrixCoords(double imgBounds[6],double structBounds[6],double xSpc,double ySpc,double zSpc,int xDims ,int yDims,int zDims)
 {

double x1=imgBounds[0]-structBounds[0];
int xStart=std::abs(vtkMath::Round(x1/xSpc));
int xEnd1=std::abs(vtkMath::Round(imgBounds[1]))-(std::abs(vtkMath::Round(structBounds[1])));
int xEnd=xDims-(std::abs(vtkMath::Round(xEnd1/xSpc)));
//qDebug()<<xStart<<xEnd<<"X1-X2";

double y1=imgBounds[2]-structBounds[2];
int yStart=std::abs(vtkMath::Round((y1/ySpc)));
int yEnd1=std::abs(vtkMath::Round(imgBounds[3]))-(std::abs(vtkMath::Round(structBounds[3])));
int yEnd=yDims-(std::abs(vtkMath::Round(yEnd1/ySpc)));
//qDebug()<<yStart<<yEnd<<"Y1-Y2";

double z1=imgBounds[4]-structBounds[4];
int zStart=std::abs(vtkMath::Round(z1/zSpc));
int zEnd1=std::abs(vtkMath::Round(imgBounds[5]))-(std::abs(vtkMath::Round(structBounds[5])));
int zEnd=zDims-(std::abs(vtkMath::Round(zEnd1/zSpc)));
//qDebug()<<zStart<<zEnd<<"Z1-Z2";

//Add one voxel on either side for safety
this->structBoundsInMatrixCoords[0]=xStart-0;
this->structBoundsInMatrixCoords[1]=xEnd+0;
this->structBoundsInMatrixCoords[2]=yStart-0;
this->structBoundsInMatrixCoords[3]=yEnd+0;
this->structBoundsInMatrixCoords[4]=zStart-0;
this->structBoundsInMatrixCoords[5]=zEnd+0;

 }


 void DVHCalc::histogramData2(double doseBinWidth,std::vector<double>data,double voxelVolume,bool dvhType)
{    //doseBinWidth in Gy
     double doseMax=*std::max_element(data.begin(),data.end());//pointer is important
     double doseMin=*std::min_element(data.begin(),data.end());//pointer is important
     double doseInterval=doseMax-doseMin;
     int numOfBins=vtkMath::Round(doseInterval/doseBinWidth);
     if(numOfBins<10)
     {
         numOfBins=10;
     }
     //qDebug()<<doseMin<<doseMax<<doseInterval<<numOfBins<<"Histogram values2";

     this->histogramData(numOfBins,data,voxelVolume,dvhType);

}




