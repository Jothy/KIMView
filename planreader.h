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
    unsigned int beamEnergy;
    unsigned int beamNum;
    double beamAngle;
    double beamStopAngle =
        -1; // By default, if this is not set corectly the plan is not VMAT
    double collAngle = 0.0;
    double couchAngle = 0.0;
    double fieldX1 = 0.0;
    double fieldX2 = 0.0;
    double fieldY1 = 0.0;
    double fieldY2 = 0.0;
    double icX = 0.0; // Isocenter x
    double icY = 0.0; // Isocenter y
    double icZ = 0.0; // Isocenter z
    double mu = 0.0;
    double beamDose = 0.0;
    QString arcDirection;
  };

  struct mlcDetail {
    int leafPairs = 0;
    double cmsw = 0.0; // Cumulative meter set weight
    QString beamName;
    QString arcDirection;
    int beamNo = 0;
    double beamAngle = 0.0;
    double collAngle = 0.0;
    double couchAngle = 0.0;
    double fieldX1 = 0.0;
    double fieldX2 = 0.0;
    double fieldY1 = 0.0;
    double fieldY2 = 0.0;
    int numCtrlPts = 0; // No. of control points
    std::vector<std::vector<double>> mlcX1Pos;
    std::vector<std::vector<double>> mlcX2Pos;
  };

  std::vector<planDetail> planDetailStruct;
  std::vector<mlcDetail> mlcDetailStruct;
  std::vector<std::vector<double>>
      muWeights; // muWeight[3][5] corresponds to 3rd beam 5th segment
  unsigned int numOfBeams = 0;
  double targetDose = 0.0;
};

#endif // PLANREADER_H
