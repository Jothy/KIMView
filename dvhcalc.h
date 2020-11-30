/**************************************************************************
(c)Jothybasu Selvaraj
This program comes with no gaurantee of it accuracy.
Clinical decisions should not be made using this program
**************************************************************************/

#ifndef DVHCALC_H
#define DVHCALC_H

#include<vtkPolyData.h>
#include<vtkImageData.h>
#include<vtkSmartPointer.h>
//#include"boost/multi_array.hpp"
#include<vector>

class DVHCalc
{
public:
    DVHCalc();
    ~DVHCalc();
    std::vector<double>calcStructDVH(vtkPolyData* structure,vtkImageData* doseGrid);
    std::vector<double>getDoseValuesAsVector(std::vector<int>,std::vector<int>,std::vector<int>,vtkImageData *doseGrid);
    std::vector<float>getClonogensAsVector(vtkImageData *clonogenGrid);

    void getIndicesOfOnes(vtkImageData* imgData);
    void getDoseValues(std::vector<int>,std::vector<int>,std::vector<int>,vtkImageData* doseGrid);    
    void diffToCumulative(std::vector<double>doseBins,std::vector<double>volBins);
    void histogramData(int bins,std::vector<double> data,double voxelVolume,bool dvhType);
    void histogramData2(double doseBinWidth,std::vector<double> data,double voxelVolume,bool dvhType);
    void getStructBoundsInMatrixCoords(double imgBounds[6],double structBounds[6],double xSpc,double ySpc,double zSpc,int xDims ,int yDims,int zDims);
    void resampleImage(vtkImageData*imgData,double,double,double,int interpolationFlag);

    std::vector<int>indicesX;
    std::vector<int>indicesY;
    std::vector<int>indicesZ;
    std::vector<double>doseValues;
    std::vector<double>doseBins;
    std::vector<double>volBins;
    std::vector<double>cumVolume;
    double structBoundsInMatrixCoords[6];
    vtkSmartPointer<vtkImageData>resampledImage;
};

#endif // DVHCALC_H
