#ifndef VTKINTERACTORSTYLEIMAGECUSTOM_H
#define VTKINTERACTORSTYLEIMAGECUSTOM_H

#define vtkRenderingCore_AUTOINIT 3(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingOpenGL2)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL2)

#include<vtkInteractorStyleImage.h>
#include<vtkSmartPointer.h>

//vtkInteractorStyleImage is subclassed to interact only with the FIRST image (using SetCurrentImageToNthImage(0)
//so that only the CT image WL/WW is changed

class vtkInteractorStyleImageCustom : public vtkInteractorStyleImage
{
public:
    vtkInteractorStyleImageCustom();
    static vtkInteractorStyleImageCustom *New();
    void StartWindowLevel();

 protected:
    void SetCurrentImageToNthImage(int i);

};

#endif // VTKINTERACTORSTYLEIMAGECUSTOM_H
