#include "alignimageinteractorstyle.h"

#define vtkRenderingCore_AUTOINIT 3(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingOpenGL2)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL2)


#include<vtkRenderWindowInteractor.h>
#include<vtkCommand.h>

#include<QDebug>

AlignImageInteractorStyle::AlignImageInteractorStyle()
{
    class SpinImage:public vtkCommand
    {

    };
}

AlignImageInteractorStyle *AlignImageInteractorStyle::New()
{
    return new AlignImageInteractorStyle;
}

//Disable rotation in 3D
void AlignImageInteractorStyle::Rotate()
{
    //qDebug()<<"Rotation disabled:)";
}

//Disable zoom right button down
void AlignImageInteractorStyle::OnRightButtonDown()
{

}






