#include "vtkinteractorstyleimagecustom.h"

#define vtkRenderingCore_AUTOINIT 3(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingOpenGL2)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL2)

#include "vtkAbstractPropPicker.h"
#include "vtkAssemblyPath.h"
#include "vtkPropCollection.h"

#include "vtkCallbackCommand.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkCamera.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkImageSlice.h"
#include "vtkImageMapper3D.h"
#include "vtkImageProperty.h"
#include<vtkSmartPointer.h>

vtkInteractorStyleImageCustom::vtkInteractorStyleImageCustom()
{

}

vtkInteractorStyleImageCustom *vtkInteractorStyleImageCustom::New()
{
    return new vtkInteractorStyleImageCustom;
}


//----------------------------------------------------------------------------
// This is a way of dealing with images as if they were layers.
// It looks through the renderer's list of props and sets the
// interactor ivars from the Nth image that it finds.  You can
// also use negative numbers, i.e. -1 will return the last image,
// -2 will return the second-to-last image, etc.
void vtkInteractorStyleImageCustom::SetCurrentImageToNthImage(int i)
{
    if (!this->CurrentRenderer)
    {
        return;
    }

    vtkPropCollection *props = this->CurrentRenderer->GetViewProps();
    vtkProp *prop = 0;
    vtkAssemblyPath *path;
    vtkImageSlice *imageProp = 0;
    vtkCollectionSimpleIterator pit;

    for (int k = 0; k < 2; k++)
    {
        int j = 0;
        for (props->InitTraversal(pit); (prop = props->GetNextProp(pit)); )
        {
            bool foundImageProp = false;
            for (prop->InitPathTraversal(); (path = prop->GetNextPath()); )
            {
                vtkProp *tryProp = path->GetLastNode()->GetViewProp();
                if ( (imageProp = vtkImageSlice::SafeDownCast(tryProp)) != 0 )
                    imageProp = vtkImageSlice::SafeDownCast(tryProp);
                if (imageProp)
                {
                    if (j == i)
                        if (j == i && imageProp->GetPickable())
                        {
                            foundImageProp = true;
                            break;
                        }
                    imageProp = 0;
                    j++;
                }
            }
            if (foundImageProp)
            {
                break;
            }
        }
        if (i < 0)
        {
            i += j;
        }
    }

    vtkImageProperty *property = 0;
    if (imageProp)
    {
        property = imageProp->GetProperty();
    }

    if (property != this->CurrentImageProperty)
    {
        if (this->CurrentImageProperty)
        {
            this->CurrentImageProperty->Delete();
        }

        this->CurrentImageProperty = property;

        if (this->CurrentImageProperty)
        {
            this->CurrentImageProperty->Register(this);
        }
    }
}

//----------------------------------------------------------------------------
void vtkInteractorStyleImageCustom::StartWindowLevel()
{
    if (this->State != VTKIS_NONE)
    {
        return;
    }
    this->StartState(VTKIS_WINDOW_LEVEL);

    // Get the last (the first) image
    this->SetCurrentImageToNthImage(0);

    if (this->HandleObservers &&
            this->HasObserver(vtkCommand::StartWindowLevelEvent))
    {
        this->InvokeEvent(vtkCommand::StartWindowLevelEvent, this);
    }
    else
    {
        if (this->CurrentImageProperty)
        {
            vtkImageProperty *property = this->CurrentImageProperty;
            this->WindowLevelInitial[0] = property->GetColorWindow();
            this->WindowLevelInitial[1] = property->GetColorLevel();
        }
    }
}

