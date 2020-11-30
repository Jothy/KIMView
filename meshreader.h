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
