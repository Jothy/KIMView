/**************************************************************************
(c)Jothybasu Selvaraj
This program comes with no gaurantee of it accuracy.
Clinical decisions should not be made using this program
**************************************************************************/

#include "mathutility.h"

#include<vtkMath.h>
#include<time.h>
#include<vector>
#include<numeric>
#include<cmath>

#include<vtkPiecewiseFunction.h>
#include<vtkSmartPointer.h>

#include<QDebug>

MathUtility::MathUtility()
{
 //vtkMath::RandomSeed(time(NULL));
}

//int MathUtility::getGaussRandomInt(double mean,double std)
//{
//    int rand=vtkMath::Round(vtkMath::Gaussian(mean,std));
//    return rand;
//}

//std::vector<int> MathUtility::getArrayOfGaussRandomInts(unsigned int length,double mean, double std)
//{
//    std::vector<int>randArray;
//    for (unsigned int i=0;i<length;i++)
//    {
//       randArray.push_back(this->getGaussRandomInt(mean,std));
//    }
//    return randArray;
//}

//std::vector<int> MathUtility::getGaussRandomSys(unsigned int fractions,double mean,double std)
//{
//    int x=this->getGaussRandomInt(mean,std);
//    std::vector<int>sysVectors; //Initialize to 0;

//    for (unsigned int i=0;i<fractions;i++)
//    {
//        sysVectors.push_back(x);

//    }
//    return sysVectors;
//}

//std::vector<int> MathUtility::getGaussRandomRand(unsigned int fractions,double mean,double std)
//{
//    std::vector<int>randVectors;
//    for (unsigned int i=0;i<fractions;i++)
//    {
//         int x=this->getGaussRandomInt(mean,std);
//        //qDebug()<<x<<"Random";
//         randVectors.push_back(x);
//    }
//    return randVectors;
//}

//std::vector<int> MathUtility::addTwoIntArrays(std::vector<int> array1,std::vector<int>array2)
//{
//    unsigned int array1Size=array1.size();
//    unsigned int array2Size=array2.size();
//    std::vector<int>summedArray;
//    //Check sizes are equal
//    if(array1Size!=array2Size)
//    {
//        qDebug()<<"Array sizes do not match!";
//    }

//    else
//    {

//        for (unsigned int i=0;i<array1Size;i++)
//        {
//            int sum=array1[i]+array2[i];
//            summedArray.push_back(sum);

//        }
//    }

//  return summedArray;
//}

//std::vector<double> MathUtility::addTwoDoubleArrays(std::vector<double> array1,std::vector<double>array2)
//{
//    unsigned int array1Size=array1.size();
//    unsigned int array2Size=array2.size();

//    std::vector<double>summedArray;
////Check sizes are equal
//    if(array1Size!=array2Size)
//    {
//        qDebug()<<"Array sizes do not match!";
//    }

//    else
//    {

//        for (unsigned int i=0;i<array1Size;i++)
//        {
//            double sum=array1[i]+array2[i];
//            summedArray.push_back(sum);

//        }
//    }

//  return summedArray;
//}

//std::vector<double> MathUtility::divideDoubleArrayByInt(std::vector<double> array,int dividerIn)
//{
//    unsigned int arraySize=array.size();
//    std::vector<double>dividedArray;
//    for (unsigned int i=0;i<arraySize;i++)
//    {
//        double divider=static_cast<double>(dividerIn);
//        double sum=array[i]/divider;
//        dividedArray.push_back(sum);

//    }

//    return dividedArray;
//}

//std::vector<int> MathUtility::addConstIntToIntArray(std::vector<int> input,int num)
//{
//    unsigned int size=input.size();
//    for(unsigned int i=0;i<size;i++)
//    {
//        input[i]=input[i]+num;
//    }

//    return input;

//}

//double MathUtility::productOfVector(std::vector<double>input)
//{
//    int size=input.size();
//    double result=1.0;
//    for (int i=0;i<size;i++)
//    {
//        result=result*input[i];
//    }
//    return result;

//}

double MathUtility::calcMean(std::vector<double> input)
{
    double mean=std::accumulate(input.begin(),input.end(),0.0)/input.size();
    return mean;
}

double MathUtility::calcStd(std::vector<double> input)
{
    double mean=this->calcMean(input);
    unsigned int size=input.size();
    std::vector<double>difference;

    for(int i=0;i<size;i++)
    {
        difference.push_back(std::pow((input[i]-mean),2));
    }

    double sum=std::accumulate(difference.begin(),difference.end(),0.0);
    double std=std::sqrt((sum/difference.size()));
    return std;

}

double MathUtility::calcSEM(std::vector<double> input)
{
    double std=this->calcStd(input);
    //qDebug()<<std<<"std"<<std::sqrt(input.size())<<"sqrt";
    double sem=std/std::sqrt(input.size());
    return sem;

}

int MathUtility::getRandomIntWithinLimits(int lowerLimit, int upperLimit)
{
    int randInt=static_cast<int>(vtkMath::Random(lowerLimit,upperLimit));
    return randInt;
}

//double MathUtility::distanceBetweenTwoPoints(double p1[],double p2[])//target,src
//{
//    double d12 = sqrt(((p1[0] - p2[0]) * (p1[0] - p2[0]))+
//                      ( (p1[1] - p2[1]) * (p1[1] - p2[1]))+
//                      ((p1[2] - p2[2]) * (p1[2] - p2[2])));
//    //qDebug()<<d12<<"dconv";
//    return d12;

//}

//void MathUtility::extendRay(double raySrc[3],double rayTarget[3],double length)
//{
//    double dist=this->distanceBetweenTwoPoints(raySrc,rayTarget);
//    double xDiff=(rayTarget[0]-raySrc[0])/dist;
//    double yDiff=(rayTarget[1]-raySrc[1])/dist;
//    double zDiff=(rayTarget[2]-raySrc[2])/dist;
//    this->extendedRay[0]=(xDiff*length)+rayTarget[0];
//    this->extendedRay[1]=(yDiff*length)+rayTarget[1];
//    this->extendedRay[2]=(zDiff*length)+rayTarget[2];
//    //qDebug()<<extendedRay[0]<<extendedRay[1]<<extendedRay[2];

//}

void MathUtility::pieceWiseInterpolate(std::vector<double>xPts,std::vector<double>yPts)
{

    vtkSmartPointer<vtkPiecewiseFunction>interpolator=
            vtkSmartPointer<vtkPiecewiseFunction>::New();
    int size=xPts.size();
    qDebug()<<size;
    for(int i=0;i<size;i++)
    {
      interpolator->AddPoint(xPts[i],yPts[i]);
    }   
    //qDebug()<<interpolator->GetValue(500.0);

}


std::vector<int> MathUtility::xyzTOijk(double xyz[3], double* origin, double *spacing)
{
    std::vector<int>ijk;
    ijk.push_back(vtkMath::Round((xyz[0]-(origin[0]))/spacing[0]));
    ijk.push_back(vtkMath::Round((xyz[1]-(origin[1]))/spacing[1]));
    ijk.push_back(vtkMath::Round((xyz[2]-(origin[2]))/spacing[2]));
    return ijk;

}

std::vector<double> MathUtility::extendVertex(double vertexCoords[3],double ROIOrigin[3], double distance)
{
    double distanceFromOrigin= std::sqrt((std::pow((vertexCoords[0]-ROIOrigin[0]),2)+std::pow((vertexCoords[1]-ROIOrigin[1]),2)+std::pow((vertexCoords[2]-ROIOrigin[2]),2)));

    double x=(vertexCoords[0]/distanceFromOrigin)*(distanceFromOrigin+distance);
    double y=(vertexCoords[1]/distanceFromOrigin)*(distanceFromOrigin+distance);
    double z=(vertexCoords[2]/distanceFromOrigin)*(distanceFromOrigin+distance);
    std::vector<double>extendedVertex;
    extendedVertex.push_back(x);
    extendedVertex.push_back(y);
    extendedVertex.push_back(z);
    return extendedVertex;

}
























