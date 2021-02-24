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
