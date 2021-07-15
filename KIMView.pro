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
    planreader.cpp \
    presetwlwwdialog.cpp \
    selecttargetdialog.cpp \
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
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmAAbortPDU.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmAAssociateACPDU.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmAAssociateRJPDU.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmAAssociateRQPDU.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmARTIMTimer.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmAReleaseRPPDU.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmAReleaseRQPDU.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmASN1.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmAbstractSyntax.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmAnonymizeEvent.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmAnonymizer.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmApplicationContext.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmApplicationEntity.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmAsynchronousOperationsWindowSub.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmAttribute.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmAudioCodec.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmBase64.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmBaseCompositeMessage.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmBaseNormalizedMessage.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmBasePDU.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmBaseQuery.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmBaseRootQuery.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmBasicOffsetTable.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmBitmap.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmBitmapToBitmapFilter.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmBoxRegion.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmByteBuffer.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmByteSwap.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmByteSwap.txx \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmByteSwapFilter.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmByteValue.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmCAPICryptoFactory.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmCAPICryptographicMessageSyntax.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmCEchoMessages.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmCFindMessages.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmCMoveMessages.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmCP246ExplicitDataElement.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmCP246ExplicitDataElement.txx \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmCSAElement.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmCSAHeader.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmCSAHeader.txx \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmCSAHeaderDict.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmCSAHeaderDictEntry.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmCStoreMessages.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmCodeString.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmCodec.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmCoder.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmCommand.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmCommandDataSet.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmCompositeMessageFactory.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmCompositeNetworkFunctions.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmConfigure.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmConstCharWrapper.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmCryptoFactory.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmCryptographicMessageSyntax.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmCurve.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmDICOMDIR.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmDICOMDIRGenerator.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmDIMSE.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmDataElement.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmDataEvent.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmDataSet.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmDataSet.txx \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmDataSetEvent.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmDataSetHelper.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmDecoder.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmDefinedTerms.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmDeflateStream.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmDefs.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmDeltaEncodingCodec.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmDict.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmDictConverter.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmDictEntry.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmDictPrinter.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmDicts.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmDirectionCosines.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmDirectory.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmDirectoryHelper.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmDummyValueGenerator.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmDumper.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmElement.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmEmptyMaskGenerator.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmEncapsulatedDocument.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmEnumeratedValues.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmEvent.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmException.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmExplicitDataElement.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmExplicitDataElement.txx \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmExplicitImplicitDataElement.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmExplicitImplicitDataElement.txx \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmFiducials.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmFile.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmFileAnonymizer.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmFileChangeTransferSyntax.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmFileDecompressLookupTable.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmFileDerivation.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmFileExplicitFilter.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmFileMetaInformation.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmFileNameEvent.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmFileSet.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmFileStreamer.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmFilename.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmFilenameGenerator.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmFindPatientRootQuery.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmFindStudyRootQuery.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmFragment.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmFragment.txx \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmGlobal.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmGroupDict.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmIOD.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmIODEntry.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmIODs.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmIPPSorter.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmIconImage.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmIconImageFilter.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmIconImageGenerator.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmImage.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmImageApplyLookupTable.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmImageChangePhotometricInterpretation.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmImageChangePlanarConfiguration.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmImageChangeTransferSyntax.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmImageCodec.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmImageConverter.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmImageFragmentSplitter.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmImageHelper.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmImageReader.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmImageRegionReader.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmImageToImageFilter.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmImageWriter.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmImplementationClassUIDSub.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmImplementationUIDSub.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmImplementationVersionNameSub.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmImplicitDataElement.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmImplicitDataElement.txx \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmItem.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmItem.txx \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmJPEG12Codec.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmJPEG16Codec.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmJPEG2000Codec.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmJPEG8Codec.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmJPEGCodec.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmJPEGLSCodec.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmJSON.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmKAKADUCodec.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmLO.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmLegacyMacro.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmLookupTable.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmMD5.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmMacro.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmMacroEntry.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmMacros.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmMaximumLengthSub.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmMediaStorage.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmMeshPrimitive.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmModalityPerformedProcedureStepCreateQuery.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmModalityPerformedProcedureStepSetQuery.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmModule.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmModuleEntry.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmModules.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmMovePatientRootQuery.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmMoveStudyRootQuery.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmMrProtocol.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmNActionMessages.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmNCreateMessages.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmNDeleteMessages.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmNEventReportMessages.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmNGetMessages.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmNSetMessages.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmNestedModuleEntries.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmNetworkEvents.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmNetworkStateID.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmNormalizedMessageFactory.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmNormalizedNetworkFunctions.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmObject.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmOpenSSLCryptoFactory.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmOpenSSLCryptographicMessageSyntax.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmOpenSSLP7CryptoFactory.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmOpenSSLP7CryptographicMessageSyntax.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmOrientation.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmOverlay.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmPDBElement.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmPDBHeader.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmPDFCodec.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmPDUFactory.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmPDataTFPDU.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmPGXCodec.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmPNMCodec.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmPVRGCodec.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmParseException.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmParser.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmPatient.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmPersonName.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmPhotometricInterpretation.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmPixelFormat.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmPixmap.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmPixmapReader.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmPixmapToPixmapFilter.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmPixmapWriter.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmPreamble.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmPresentationContext.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmPresentationContextAC.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmPresentationContextGenerator.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmPresentationContextRQ.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmPresentationDataValue.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmPrinter.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmPrivateTag.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmProgressEvent.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmQueryBase.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmQueryFactory.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmQueryImage.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmQueryPatient.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmQuerySeries.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmQueryStudy.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmRAWCodec.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmRLECodec.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmReader.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmRegion.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmRescaler.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmRoleSelectionSub.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSHA1.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSOPClassExtendedNegociationSub.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSOPClassUIDToIOD.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmScanner.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSegment.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSegmentHelper.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSegmentReader.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSegmentWriter.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSegmentedPaletteColorLookupTable.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSequenceOfFragments.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSequenceOfFragments.txx \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSequenceOfItems.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSequenceOfItems.txx \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSerieHelper.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSeries.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmServiceClassApplicationInformation.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmServiceClassUser.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSimpleSubjectWatcher.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSmartPointer.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSorter.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSpacing.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSpectroscopy.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSplitMosaicFilter.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmStaticAssert.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmStreamImageReader.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmStreamImageWriter.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmStrictScanner.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmString.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmStringFilter.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmStudy.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSubject.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSurface.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSurfaceHelper.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSurfaceReader.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSurfaceWriter.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSwapCode.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSwapper.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSwapper.txx \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmSystem.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmTable.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmTableEntry.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmTableReader.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmTag.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmTagKeywords.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmTagPath.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmTagToType.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmTagToVR.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmTerminal.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmTestDriver.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmTesting.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmTrace.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmTransferSyntax.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmTransferSyntaxSub.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmType.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmTypes.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmUIDGenerator.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmUIDs.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmULAction.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmULActionAA.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmULActionAE.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmULActionAR.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmULActionDT.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmULBasicCallback.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmULConnection.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmULConnectionCallback.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmULConnectionInfo.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmULConnectionManager.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmULEvent.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmULTransitionTable.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmULWritingCallback.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmUNExplicitDataElement.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmUNExplicitDataElement.txx \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmUNExplicitImplicitDataElement.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmUNExplicitImplicitDataElement.txx \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmUUIDGenerator.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmUnpacker12Bits.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmUsage.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmUserInformation.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmVL.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmVM.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmVR.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmVR16ExplicitDataElement.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmVR16ExplicitDataElement.txx \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmValidate.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmValue.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmValue.txx \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmValueIO.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmValueIO.txx \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmVersion.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmWLMFindQuery.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmWaveform.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmWin32.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmWriter.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmXMLDictReader.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmXMLPrinter.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmXMLPrivateDictReader.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcm_j2k.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcm_jp2.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmcharls/colortransform.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmcharls/config.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmcharls/context.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmcharls/contextrunmode.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmcharls/decoderstrategy.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmcharls/defaulttraits.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmcharls/encoderstrategy.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmcharls/header.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmcharls/interface.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmcharls/jpegimagedatasegment.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmcharls/jpegmarker.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmcharls/jpegmarkersegment.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmcharls/jpegsegment.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmcharls/jpegstreamwriter.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmcharls/lookuptable.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmcharls/losslesstraits.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmcharls/processline.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmcharls/publictypes.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmcharls/scan.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmcharls/util.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/12/jconfig.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/12/jerror.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/12/jinclude.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/12/jmorecfg.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/12/jpegcmake.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/12/jpeglib.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/12/mangle_jpeg12bits.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/16/jconfig.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/16/jerror.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/16/jinclude.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/16/jmorecfg.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/16/jpegcmake.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/16/jpeglib.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/16/mangle_jpeg16bits.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/8/jconfig.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/8/jerror.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/8/jinclude.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/8/jmorecfg.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/8/jpegcmake.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/8/jpeglib.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/8/mangle_jpeg8bits.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/jchuff.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/jconfig.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/jdct.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/jdhuff.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/jerror.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/jinclude.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/jlossls.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/jlossy.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/jmemsys.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/jmorecfg.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/jpegint.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/jpeglib.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmjpeg/jversion.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmopenjpeg/openjpeg.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmopenjpeg/opj_config.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/gdcmopenjpeg/opj_stdint.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/socketxx/echo.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/socketxx/fork.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/socketxx/ftp.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/socketxx/local.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/socketxx/pipestream.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/socketxx/protocol.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/socketxx/sig.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/socketxx/smtp.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/socketxx/sockinet.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/socketxx/sockstream.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/socketxx/sockunix.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/vtkGDCMImageReader.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/vtkGDCMImageReader2.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/vtkGDCMImageWriter.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/vtkGDCMMedicalImageProperties.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/vtkGDCMPolyDataReader.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/vtkGDCMPolyDataWriter.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/vtkGDCMTesting.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/vtkGDCMThreadedImageReader.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/vtkGDCMThreadedImageReader2.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/vtkImageColorViewer.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/vtkImageMapToColors16.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/vtkImageMapToWindowLevelColors2.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/vtkImagePlanarComponentsToComponents.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/vtkImageRGBToYBR.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/vtkImageYBRToRGB.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/vtkLookupTable16.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/vtkRTStructSetProperties.h \
    ../../Libs/GDCM/Install/include/gdcm-2.8/zipstreamimpl.h \
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
    ../../Libs/GDCM/Install/bin/concrt140.dll \
    ../../Libs/GDCM/Install/bin/gdcmCommon.dll \
    ../../Libs/GDCM/Install/bin/gdcmDICT.dll \
    ../../Libs/GDCM/Install/bin/gdcmDSED.dll \
    ../../Libs/GDCM/Install/bin/gdcmIOD.dll \
    ../../Libs/GDCM/Install/bin/gdcmMEXD.dll \
    ../../Libs/GDCM/Install/bin/gdcmMSFF.dll \
    ../../Libs/GDCM/Install/bin/gdcmcharls.dll \
    ../../Libs/GDCM/Install/bin/gdcmexpat.dll \
    ../../Libs/GDCM/Install/bin/gdcmgetopt.dll \
    ../../Libs/GDCM/Install/bin/gdcmjpeg12.dll \
    ../../Libs/GDCM/Install/bin/gdcmjpeg16.dll \
    ../../Libs/GDCM/Install/bin/gdcmjpeg8.dll \
    ../../Libs/GDCM/Install/bin/gdcmopenjp2.dll \
    ../../Libs/GDCM/Install/bin/gdcmzlib.dll \
    ../../Libs/GDCM/Install/bin/msvcp140.dll \
    ../../Libs/GDCM/Install/bin/socketxx.dll \
    ../../Libs/GDCM/Install/bin/vcruntime140.dll \
    ../../Libs/GDCM/Install/bin/vtkCommonCore-7.1.dll \
    ../../Libs/GDCM/Install/bin/vtkIOImage-7.1.dll \
    ../../Libs/GDCM/Install/bin/vtkIOLegacy-7.1.dll \
    ../../Libs/GDCM/Install/bin/vtkImagingCore-7.1.dll \
    ../../Libs/GDCM/Install/bin/vtkImagingSources-7.1.dll \
    ../../Libs/GDCM/Install/bin/vtkInteractionStyle-7.1.dll \
    ../../Libs/GDCM/Install/bin/vtkRenderingCore-7.1.dll \
    ../../Libs/GDCM/Install/bin/vtkRenderingFreeType-7.1.dll \
    ../../Libs/GDCM/Install/bin/vtkgdcm.dll \
    ../../Libs/GDCM/Install/bin/vtksys-7.1.dll \
    ../../Libs/GDCM/Install/lib/gdcm-2.8/GDCMConfig.cmake \
    ../../Libs/GDCM/Install/lib/gdcm-2.8/GDCMConfigVersion.cmake \
    ../../Libs/GDCM/Install/lib/gdcm-2.8/GDCMTargets-release.cmake \
    ../../Libs/GDCM/Install/lib/gdcm-2.8/GDCMTargets.cmake \
    ../../Libs/GDCM/Install/lib/gdcm-2.8/UseGDCM.cmake \
    ../../Libs/GDCM/Install/lib/gdcmCommon.lib \
    ../../Libs/GDCM/Install/lib/gdcmDICT.lib \
    ../../Libs/GDCM/Install/lib/gdcmDSED.lib \
    ../../Libs/GDCM/Install/lib/gdcmIOD.lib \
    ../../Libs/GDCM/Install/lib/gdcmMEXD.lib \
    ../../Libs/GDCM/Install/lib/gdcmMSFF.lib \
    ../../Libs/GDCM/Install/lib/gdcmcharls.lib \
    ../../Libs/GDCM/Install/lib/gdcmexpat.lib \
    ../../Libs/GDCM/Install/lib/gdcmgetopt.lib \
    ../../Libs/GDCM/Install/lib/gdcmjpeg12.lib \
    ../../Libs/GDCM/Install/lib/gdcmjpeg16.lib \
    ../../Libs/GDCM/Install/lib/gdcmjpeg8.lib \
    ../../Libs/GDCM/Install/lib/gdcmopenjp2.lib \
    ../../Libs/GDCM/Install/lib/gdcmopenjpeg-2.1/OpenJPEGConfig.cmake \
    ../../Libs/GDCM/Install/lib/gdcmzlib.lib \
    ../../Libs/GDCM/Install/lib/socketxx.lib \
    ../../Libs/GDCM/Install/lib/vtkgdcm.lib \
    ../../Libs/GDCM/Install/share/gdcm-2.8/XML/CSAHeader.xml \
    ../../Libs/GDCM/Install/share/gdcm-2.8/XML/Part3.xml \
    ../../Libs/GDCM/Install/share/gdcm-2.8/XML/Part4.xml \
    ../../Libs/GDCM/Install/share/gdcm-2.8/XML/Part6.xml \
    ../../Libs/GDCM/Install/share/gdcm-2.8/XML/Part7a.xml \
    ../../Libs/GDCM/Install/share/gdcm-2.8/XML/Part7b.xml \
    ../../Libs/GDCM/Install/share/gdcm-2.8/XML/UIDs.xml \
    ../../Libs/GDCM/Install/share/gdcm-2.8/XML/cp699.xml \
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
