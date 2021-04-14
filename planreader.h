/******************************************************************************
MIT License

Copyright (c) 2021 Jothy Selvaraj

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
                                                              copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

       THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
            ******************************************************************************/
#ifndef PLANREADER_H
#define PLANREADER_H

#include <vtkImageData.h>
#include <vtkMatrix4x4.h>
#include <vtkSmartPointer.h>

#include <QList>
#include <QString>
#include <vector>

class PlanReader {
 public:
  PlanReader();
  ~PlanReader();
  void readRTPlan();
  QString planLabel;
  unsigned int fractionsPlanned;

  struct planDetail {
    QString mcName;
    double ssd;
    QString beamName;
    QString beamType;
    int beamEnergy;
    unsigned int beamNo;
    double beamAngle;
    double collAngle;
    double couchAngle;
    double fieldX1;
    double fieldX2;
    double fieldY1;
    double fieldY2;
    double icX;  // Isocenter x
    double icY;  // Isocenter y
    double icZ;  // Isocenter z
    double mu;
    double beamDose;
  };

  struct mlcDetail {
    int leafPairs;
    double cmsw;  // Cumulative meter set weight
    QString beamName;
    int beamNo;
    double beamAngle;
    double collAngle;
    double couchAngle;
    double fieldX1;
    double fieldX2;
    double fieldY1;
    double fieldY2;
    int numCtrlPts;  // No. of control points
    std::vector<std::vector<double>> mlcX1Pos;
    std::vector<std::vector<double>> mlcX2Pos;
  };

  std::vector<planDetail> planDetailStruct;
  std::vector<mlcDetail> mlcDetailStruct;
  std::vector<std::vector<double>>
      muWeights;  // muWeight[3][5] corresponds to 3rd beam 5th segment
  unsigned int numOfBeams;
  double targetDose;
};

#endif  // PLANREADER_H
