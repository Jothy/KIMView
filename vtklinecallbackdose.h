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

#ifndef VTKLINECALLBACKDOSE_H
#define VTKLINECALLBACKDOSE_H

#include <vtkCommand.h>
#include <vtkImageData.h>
#include <vtkLineWidget2.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>

#include <QMdiArea>
#include <QSplineSeries>

#include "doseprofiledialog.h"

class vtkLineCallbackDose : public vtkCommand {
 public:
  vtkLineCallbackDose(QWidget *parent);
  ~vtkLineCallbackDose();
  vtkLineCallbackDose *New(QWidget *parent);
  vtkSmartPointer<vtkPolyData> lineData;
  void Execute(vtkObject *caller, unsigned long, void *);
  vtkSmartPointer<vtkImageData> dose;
  DoseProfileDialog *doseProfiler;
  float distance;
  double trX, trY, trZ;  // X,Y,Z translations
  void transformPolyData();
  int SliceOrientation = 0;  // Axial by default
  QSplineSeries *SplineSeries;
};

#endif  // VTKLINECALLBACKDOSE_H
