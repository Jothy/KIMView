#ifndef ALIGNIMAGEINTERACTORSTYLE_H
#define ALIGNIMAGEINTERACTORSTYLE_H

#define vtkRenderingCore_AUTOINIT 3(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingOpenGL2)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL2)


#include<vtkInteractorStyleTrackballActor.h>
#include<vtkSmartPointer.h>


class AlignImageInteractorStyle : public vtkInteractorStyleTrackballActor
{
public:
    AlignImageInteractorStyle();
    static AlignImageInteractorStyle *New();
    void Rotate();
    void OnRightButtonDown();

};

#endif // ALIGNIMAGEINTERACTORSTYLE_H
