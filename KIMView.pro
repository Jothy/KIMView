QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts network

CONFIG += c++19

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES +=\
    aboutdialog.cpp \
    alignimageinteractorstyle.cpp \
    bevwidget.cpp \
    createobjects.cpp \
    ipconfigdialog.cpp \
    doseprofiledialog.cpp \
    dvhcalc.cpp \
    dvhdialog.cpp \
    imageviewer2d.cpp \
    mathutility.cpp \
    planreader.cpp \
    presetwlwwdialog.cpp \
    radiobiologycalc.cpp \
    selecttargetdialog.cpp \
    tcpdialog.cpp \
    utilities.cpp   \
    main.cpp \
    mainwindow.cpp \
    meshreader.cpp \
    qDoubleSlider.cpp \
    rangesliderdialog.cpp \
    wlwwdialog.cpp  \
    rtstructreaderdialog.cpp \    
    udplistener.cpp \
    vtkinteractorstyleimagecustom.cpp \    
    vtklinecallbackdose.cpp



HEADERS +=\
    mathutility.h \
    radiobiologycalc.h \
    tcpdialog.h \
    aboutdialog.h \
    alignimageinteractorstyle.h \
    bevwidget.h \
    ipconfigdialog.h \
    createobjects.h \
    doseprofiledialog.h \
    dvhcalc.h \
    planreader.h \
    presetwlwwdialog.h \
    selecttargetdialog.h \
    utilities.h \
    dvhdialog.h \
    wlwwdialog.h \
    imageviewer2d.h \   
    mainwindow.h \
    meshreader.h \
    qDoubleSlider.h \
    rangesliderdialog.h \
    rtstructreaderdialog.h \   
    udplistener.h \
    vtkinteractorstyleimagecustom.h \   
    vtklinecallbackdose.h



FORMS += \
    aboutdialog.ui \
    bevwidget.ui \
    doseprofiledialog.ui \
    dvhdialog.ui \
    imageviewer2d.ui \
    ipconfigdialog.ui \
    mainwindow.ui \
    presetwlwwdialog.ui \
    rangesliderdialog.ui \
    rtstructreaderdialog.ui \
    selecttargetdialog.ui \
    tcpdialog.ui \
    wlwwdialog.ui

TRANSLATIONS += 
    KIMView_en_AU.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH+= D:\Libs\VTK\Install\include\vtk-7.1

LIBS+= -LD:\Libs\VTK\Install\lib -lvtkalglib-7.1 -lvtkChartsCore-7.1   -lvtkCommonColor-7.1   -lvtkCommonComputationalGeometry-7.1   -lvtkCommonCore-7.1\
-lvtkCommonDataModel-7.1   -lvtkCommonExecutionModel-7.1   -lvtkCommonMath-7.1   -lvtkCommonMisc-7.1   -lvtkCommonSystem-7.1   -lvtkCommonTransforms-7.1\
-lvtkDICOMParser-7.1   -lvtkDomainsChemistry-7.1   -lvtkDomainsChemistryOpenGL2-7.1   -lvtkexoIIc-7.1   -lvtkexpat-7.1   -lvtkFiltersAMR-7.1\
-lvtkFiltersCore-7.1   -lvtkFiltersExtraction-7.1   -lvtkFiltersFlowPaths-7.1   -lvtkFiltersGeneral-7.1   -lvtkFiltersGeneric-7.1   -lvtkFiltersGeometry-7.1\
-lvtkFiltersHybrid-7.1   -lvtkFiltersHyperTree-7.1   -lvtkFiltersImaging-7.1   -lvtkFiltersModeling-7.1   -lvtkFiltersParallel-7.1   -lvtkFiltersParallelImaging-7.1\
-lvtkFiltersPoints-7.1   -lvtkFiltersProgrammable-7.1   -lvtkFiltersSelection-7.1   -lvtkFiltersSMP-7.1   -lvtkFiltersSources-7.1   -lvtkFiltersStatistics-7.1\
-lvtkFiltersTexture-7.1   -lvtkFiltersVerdict-7.1   -lvtkfreetype-7.1   -lvtkGeovisCore-7.1   -lvtkgl2ps-7.1   -lvtkglew-7.1   -lvtkGUISupportQt-7.1\
-lvtkGUISupportQtSQL-7.1   -lvtkhdf5-7.1   -lvtkhdf5_hl-7.1   -lvtkImagingColor-7.1   -lvtkImagingCore-7.1   -lvtkImagingFourier-7.1   -lvtkImagingGeneral-7.1\
-lvtkImagingHybrid-7.1   -lvtkImagingMath-7.1   -lvtkImagingMorphological-7.1   -lvtkImagingSources-7.1   -lvtkImagingStatistics-7.1   -lvtkImagingStencil-7.1\
-lvtkInfovisCore-7.1   -lvtkInfovisLayout-7.1   -lvtkInteractionImage-7.1   -lvtkInteractionStyle-7.1   -lvtkInteractionWidgets-7.1   -lvtkIOAMR-7.1\
-lvtkIOCore-7.1   -lvtkIOEnSight-7.1   -lvtkIOExodus-7.1   -lvtkIOExport-7.1   -lvtkIOGeometry-7.1   -lvtkIOImage-7.1   -lvtkIOImport-7.1\
-lvtkIOInfovis-7.1   -lvtkIOLegacy-7.1   -lvtkIOLSDyna-7.1   -lvtkIOMINC-7.1   -lvtkIOMovie-7.1   -lvtkIONetCDF-7.1   -lvtkIOParallel-7.1   -lvtkIOParallelXML-7.1\
-lvtkIOPLY-7.1   -lvtkIOSQL-7.1   -lvtkIOTecplotTable-7.1   -lvtkIOVideo-7.1   -lvtkIOXML-7.1   -lvtkIOXMLParser-7.1   -lvtkjpeg-7.1   -lvtkjsoncpp-7.1\
-lvtklibxml2-7.1   -lvtkmetaio-7.1   -lvtkNetCDF-7.1   -lvtkNetCDF_cxx-7.1   -lvtkoggtheora-7.1   -lvtkParallelCore-7.1   -lvtkpng-7.1   -lvtkproj4-7.1\
-lvtkRenderingAnnotation-7.1   -lvtkRenderingContext2D-7.1   -lvtkRenderingContextOpenGL2-7.1   -lvtkRenderingCore-7.1   -lvtkRenderingFreeType-7.1\
-lvtkRenderingGL2PSOpenGL2-7.1   -lvtkRenderingImage-7.1   -lvtkRenderingLabel-7.1   -lvtkRenderingLOD-7.1   -lvtkRenderingOpenGL2-7.1   -lvtkRenderingQt-7.1\
-lvtkRenderingVolume-7.1   -lvtkRenderingVolumeOpenGL2-7.1   -lvtksqlite-7.1   -lvtksys-7.1   -lvtktiff-7.1   -lvtkverdict-7.1   -lvtkViewsContext2D-7.1\
-lvtkViewsCore-7.1   -lvtkViewsGeovis-7.1   -lvtkViewsInfovis-7.1   -lvtkViewsQt-7.1   -lvtkzlib-7.1

INCLUDEPATH+= D:\Libs\ITK\Install\include\ITK-4.13

LIBS+= -LD:\Libs\ITK\Install\lib  -lITKBiasCorrection-4.13   -lITKBioCell-4.13   -lITKCommon-4.13   -lITKDICOMParser-4.13   -litkdouble-conversion-4.13   -lITKEXPAT-4.13\
 -lITKFEM-4.13   -litkgdcmcharls-4.13   -litkgdcmCommon-4.13   -litkgdcmDICT-4.13   -litkgdcmDSED-4.13   -litkgdcmIOD-4.13   -litkgdcmjpeg12-4.13   -litkgdcmjpeg16-4.13\
 -litkgdcmjpeg8-4.13   -litkgdcmMEXD-4.13   -litkgdcmMSFF-4.13   -litkgdcmopenjp2-4.13   -litkgdcmsocketxx-4.13   -lITKgiftiio-4.13   -lITKIOBioRad-4.13   -lITKIOBMP-4.13\
 -lITKIOBruker-4.13   -lITKIOCSV-4.13   -lITKIOGDCM-4.13   -lITKIOGE-4.13   -lITKIOGIPL-4.13   -lITKIOHDF5-4.13   -lITKIOImageBase-4.13   -lITKIOIPL-4.13\
 -lITKIOJPEG-4.13   -lITKIOLSM-4.13   -lITKIOMesh-4.13   -lITKIOMeta-4.13   -lITKIOMINC-4.13   -lITKIOMRC-4.13   -lITKIONIFTI-4.13   -lITKIONRRD-4.13   -lITKIOPNG-4.13\
 -lITKIOSiemens-4.13   -lITKIOSpatialObjects-4.13   -lITKIOStimulate-4.13   -lITKIOTIFF-4.13   -lITKIOTransformBase-4.13   -lITKIOTransformHDF5-4.13   -lITKIOTransformInsightLegacy-4.13\
 -lITKIOTransformMatlab-4.13   -lITKIOVTK-4.13   -lITKIOXML-4.13   -litkjpeg-4.13   -lITKKLMRegionGrowing-4.13   -lITKLabelMap-4.13   -litklbfgs-4.13    -lITKMesh-4.13   -lITKMetaIO-4.13\
 -litkminc2-4.13   -litknetlib-4.13   -litkNetlibSlatec-4.13   -lITKniftiio-4.13   -lITKNrrdIO-4.13   -lITKOptimizers-4.13   -lITKOptimizersv4-4.13   -lITKPath-4.13   -litkpng-4.13\
 -lITKPolynomials-4.13   -lITKQuadEdgeMesh-4.13   -lITKSpatialObjects-4.13   -lITKStatistics-4.13   -litksys-4.13   -litktestlib-4.13   -litktiff-4.13   -lITKTransform-4.13\
 -lITKTransformFactory-4.13   -litkv3p_netlib-4.13   -litkvcl-4.13   -lITKVideoCore-4.13   -lITKVideoIO-4.13   -litkvnl-4.13   -lITKVNLInstantiation-4.13   -litkvnl_algo-4.13\
 -lITKVTK-4.13   -lITKVtkGlue-4.13   -lITKWatersheds-4.13   -litkzlib-4.13   -lITKznz-4.13   -llibitkhdf5   -llibitkhdf5_cpp

##Needed to link ITK libs. This is advanced windows api
LIBS += advapi32.lib

INCLUDEPATH+= D:\Libs\GDCM\Install\include\gdcm-2.8

LIBS+= -LD:\Libs\GDCM\Install\lib -lgdcmcharls -lgdcmCommon -lgdcmDICT -lgdcmDSED -lgdcmexpat -lgdcmgetopt -lgdcmIOD -lgdcmjpeg8 -lgdcmjpeg12 -lgdcmjpeg16 -lgdcmMEXD -lgdcmMSFF -lgdcmopenjp2\
-lgdcmzlib -lsocketxx


DISTFILES += \    
    Human.vtp \
    LicenseTemplate \
    Notes \
    Styles/AMOLED.qss \
    Styles/Aqua.qss \
    Styles/ConsoleStyle.qss \
    Styles/ElegantDark.qss \
    Styles/ImageX.qss \
    Styles/ManjaroMix.qss \
    Styles/MaterialDark.qss \
    Styles/darkstyle.qss

RESOURCES += \
    RC.qrc
