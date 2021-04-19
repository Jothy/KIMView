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
#ifndef DVHCALC_H
#define DVHCALC_H

#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
//#include"boost/multi_array.hpp"

#include <vector>

class DVHCalc {
 public:
  DVHCalc();
  ~DVHCalc();
  std::vector<double> calcStructDVH(vtkPolyData* structure,
                                    vtkImageData* doseGrid);
  std::vector<double> getDoseValuesAsVector(std::vector<int>, std::vector<int>,
                                            std::vector<int>,
                                            vtkImageData* doseGrid);
  std::vector<float> getClonogensAsVector(vtkImageData* clonogenGrid);

  void getIndicesOfOnes(vtkImageData* imgData);
  void getDoseValues(std::vector<int>, std::vector<int>, std::vector<int>,
                     vtkImageData* doseGrid);
  void diffToCumulative(std::vector<double> doseBins,
                        std::vector<double> volBins);
  void histogramData(int bins, std::vector<double> data, double voxelVolume,
                     bool dvhType);
  void histogramData2(double doseBinWidth, std::vector<double> data,
                      double voxelVolume, bool dvhType);
  void getStructBoundsInMatrixCoords(double imgBounds[6],
                                     double structBounds[6], double xSpc,
                                     double ySpc, double zSpc, int xDims,
                                     int yDims, int zDims);
  void resampleImage(vtkImageData* imgData, double, double, double,
                     int interpolationFlag);

  std::vector<int> indicesX;
  std::vector<int> indicesY;
  std::vector<int> indicesZ;
  std::vector<double> doseValues;
  std::vector<double> doseBins;
  std::vector<double> volBins;
  std::vector<double> cumVolume;
  double structBoundsInMatrixCoords[6];
  vtkSmartPointer<vtkImageData> resampledImage;
};

#endif  // DVHCALC_H
