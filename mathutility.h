/**************************************************************************
(c)Jothybasu Selvaraj
This program comes with no gaurantee of it accuracy.
Clinical decisions should not be made using this program
**************************************************************************/

#ifndef MATHUTILITY_H
#define MATHUTILITY_H

#include<vector>

#include<vtkMath.h>
#include<QDebug>

class MathUtility
{
public:
    MathUtility();

    inline int getGaussRandomInt(double mean,double std)
    {
        int rand=vtkMath::Round(vtkMath::Gaussian(mean,std));
        return rand;
    }

    inline std::vector<int> getArrayOfGaussRandomInts(unsigned int length,double mean, double std)
    {
        std::vector<int>randArray;
        for (unsigned int i=0;i<length;++i)
        {
            randArray.push_back(this->getGaussRandomInt(mean,std));
        }
        return randArray;
    }

    inline std::vector<int> getGaussRandomSys(unsigned int fractions,double mean,double std)
    {
        int x=this->getGaussRandomInt(mean,std);
        std::vector<int>sysVectors; //Initialize to 0;

        for (unsigned int i=0;i<fractions;++i)
        {
            sysVectors.push_back(x);

        }
        return sysVectors;

    }

    inline std::vector<int> getGaussRandomRand(unsigned int fractions,double mean,double std)
    {
        std::vector<int>randVectors;
        for (unsigned int i=0;i<fractions;++i)
        {
            int x=this->getGaussRandomInt(mean,std);
            //qDebug()<<x<<"Random";
            randVectors.push_back(x);
        }
        return randVectors;

    }

    inline std::vector<int> addTwoIntArrays(std::vector<int> array1,std::vector<int>array2)
    {
        unsigned int array1Size=array1.size();
        unsigned int array2Size=array2.size();
        std::vector<int>summedArray;
        //Check sizes are equal
        if(array1Size!=array2Size)
        {
            qDebug()<<"Array sizes do not match!";
        }

        else
        {

            for (unsigned int i=0;i<array1Size;i++)
            {
                int sum=array1[i]+array2[i];
                summedArray.push_back(sum);

            }
        }

        return summedArray;

    }

    inline std::vector<double> addTwoDoubleArrays(std::vector<double> array1,std::vector<double>array2)
    {
        unsigned int array1Size=array1.size();
        unsigned int array2Size=array2.size();

        std::vector<double>summedArray;
        //Check sizes are equal
        if(array1Size!=array2Size)
        {
            qDebug()<<"Array sizes do not match!";
        }

        else
        {

            for (unsigned int i=0;i<array1Size;i++)
            {
                double sum=array1[i]+array2[i];
                summedArray.push_back(sum);

            }
        }

        return summedArray;

    }

    inline std::vector<double> divideDoubleArrayByInt(std::vector<double> array,int dividerIn)
    {
        unsigned int arraySize=array.size();
        std::vector<double>dividedArray;
        for (unsigned int i=0;i<arraySize;i++)
        {
            double divider=static_cast<double>(dividerIn);
            double sum=array[i]/divider;
            dividedArray.push_back(sum);

        }

        return dividedArray;
    }

    inline std::vector<int> addConstIntToIntArray(std::vector<int>input,int num)
    {
        unsigned int size=input.size();
        for(unsigned int i=0;i<size;i++)
        {
            input[i]=input[i]+num;
        }

        return input;
    }

    inline double productOfVector(std::vector<double>input)
    {

        int size=input.size();
        double result=1.0;
        for (int i=0;i<size;++i)
        {
            result=result*input[i];
        }
        return result;
    }

    double calcMean(std::vector<double>input);
    double calcStd(std::vector<double>input);
    double calcSEM(std::vector<double>input);
    int getRandomIntWithinLimits(int lowerLimit,int upperLimit);

    inline double distanceBetweenTwoPoints(double p1[],double p2[])
    {
        double d12 = sqrt(((p1[0] - p2[0]) * (p1[0] - p2[0]))+
                          ( (p1[1] - p2[1]) * (p1[1] - p2[1]))+
                          ((p1[2] - p2[2]) * (p1[2] - p2[2])));
        //qDebug()<<d12<<"dconv";
        return d12;
    }

    inline void extendRay(double raySrc[3],double rayTarget[3],double length)
    {
        double dist=this->distanceBetweenTwoPoints(raySrc,rayTarget);
        double xDiff=(rayTarget[0]-raySrc[0])/dist;
        double yDiff=(rayTarget[1]-raySrc[1])/dist;
        double zDiff=(rayTarget[2]-raySrc[2])/dist;
        this->extendedRay[0]=(xDiff*length)+rayTarget[0];
        this->extendedRay[1]=(yDiff*length)+rayTarget[1];
        this->extendedRay[2]=(zDiff*length)+rayTarget[2];
        //qDebug()<<extendedRay[0]<<extendedRay[1]<<extendedRay[2];
    }

    double extendedRay[3];
    void pieceWiseInterpolate(std::vector<double>xPts,std::vector<double>yPts);
    std::vector<int>xyzTOijk(double xyz[3],double *origin,double* spacing);
    std::vector<double>extendVertex(double vertexCoords[3],double ROIorigin[3],double distance);

};

#endif // MATHUTILITY_H
