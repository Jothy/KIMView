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

#ifndef MESHREADER_H
#define MESHREADER_H

#define vtkRenderingCore_AUTOINIT 3(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingOpenGL2)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL2)

#include<vtkImageData.h>
#include<vtkActorCollection.h>
#include<vtkSmartPointer.h>
#include<vtkPolyData.h>
#include"QString"
#include"QList"
#include<vector>
#include<QWidget>

class meshReader
{
public:
    meshReader(QWidget* parent);
    ~meshReader();
    void getROIMeshes(vtkImageData* imgData,float zSpacing,float targetReduction,QList<int> selectionList,QWidget* parent);//zSpacing is used for extrusion in meshing
    void getStructFileName();
    QString structFileName;
    int NumberOfROIs;
    QList<int> ROINo;
    QList<QString> ROINames;
    QList<QString>ROITypes;
    double ROIColors[50][3]; //No. of ROIs,RGB- maximum 50 ROIs supported for now
    vtkSmartPointer<vtkActorCollection>ROIActors;
    std::vector<vtkSmartPointer<vtkPolyData> > meshes;
    QString structSetLabel;
    QList<int> selectionList;
    QWidget *parent;
};

#endif // MESHREADER_H
