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

#ifndef CREATEOBJECTS_H
#define CREATEOBJECTS_H

#include <vtkActor.h>
#include <vtkAnnotatedCubeActor.h>
#include <vtkAssembly.h>
#include <vtkCaptionActor2D.h>
#include <vtkCornerAnnotation.h>
#include <vtkFollower.h>
#include <vtkImageData.h>
#include <vtkLegendBoxActor.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkTextActor.h>

#include <QString>
#include <vector>

class CreateObjects {
 public:
  CreateObjects();
  vtkSmartPointer<vtkCornerAnnotation> annotation;
  void createAnnotation(int location, double RGB[3], QString text1);
  vtkSmartPointer<vtkPolyData> createSphere(double radius, int thetaR, int phiR,
                                            double center[3]);
  vtkSmartPointer<vtkPolyData> createCube(double x, double y, double z,
                                          double center[3]);
  vtkSmartPointer<vtkPolyData> createCylinder(double radius, double height,
                                              double center[3]);
  vtkSmartPointer<vtkLegendBoxActor> createIsodoseLegend(
      std::vector<double> isodoseValues, std::vector<double> colorsR,
      std::vector<double> colorsG, std::vector<double> colorsB);

  vtkSmartPointer<vtkActor> createRectangle(double x1, double x2, double y1,
                                            double y2, double gantryAngle,
                                            double collAngle,
                                            double isocenter[3]);
  vtkSmartPointer<vtkActor> createLeaf(double length, double thickness,
                                       double position[3], double isocenter[3]);
  vtkSmartPointer<vtkAssembly> createVarian120MLC(double isocenter[3],
                                                  double gantryAngle,
                                                  double collAngle,
                                                  vtkTransform *userTr);
  vtkSmartPointer<vtkActor> createVectorText(QString txt, double collAngle,
                                             double gantryAngle);
  vtkSmartPointer<vtkActor> createAxes(double shift[3], double isocenter[3]);
  vtkSmartPointer<vtkCaptionActor2D> createCaption2D();
  vtkSmartPointer<vtkAssembly> createGraticule();
  vtkSmartPointer<vtkAssembly> createArc(double radius, double gantryStart,
                                         double gantryStop, QString dir,
                                         double Iso[3]);
  vtkSmartPointer<vtkActor> createBeam(double x1, double x2, double y1,
                                       double y2, double upperEnd,
                                       double lowerEnd, double gantryAngle,
                                       double collAngle, double couchAngle,
                                       double isocenter[3]);
};

#endif  // CREATEOBJECTS_H
