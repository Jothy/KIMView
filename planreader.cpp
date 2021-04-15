/******************************************************************************
MIT License

Copyright (c) 2021 Jothy Selvaraj

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
                                                              copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

       THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
            ******************************************************************************/
#include "planreader.h"

#include <gdcmAttribute.h>
#include <gdcmDataElement.h>
#include <gdcmDataSet.h>
#include <gdcmDirectory.h>
#include <gdcmFile.h>
#include <gdcmFileMetaInformation.h>
#include <gdcmFilename.h>
#include <gdcmIPPSorter.h>
#include <gdcmItem.h>
#include <gdcmNestedModuleEntries.h>
#include <gdcmReader.h>
#include <gdcmSequenceOfItems.h>
#include <gdcmSystem.h>
#include <gdcmTag.h>
#include <gdcmValue.h>
#include <vtkMedicalImageProperties.h>

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <algorithm>
#include <vector>

PlanReader::PlanReader() { this->numOfBeams = 0; }

PlanReader::~PlanReader() {}

void PlanReader::readRTPlan() {
  QString planFile = QFileDialog::getOpenFileName(0, "Open RT Plan");
  this->numOfBeams = 0;  // Reset to 0
  if (planFile != NULL)  // Check whether the file is selected or not
  {
    gdcm::Reader RTreader;
    RTreader.SetFileName(planFile.toLatin1().data());
    if (!RTreader.Read()) {
      QMessageBox* msgBox = new QMessageBox;
      msgBox->setText("Can't' read the file!");
      msgBox->setIcon(QMessageBox::Critical);
      msgBox->exec();
      delete msgBox;
    }

    else {
      const gdcm::DataSet& ds = RTreader.GetFile().GetDataSet();
      gdcm::Tag dosecsq(0x300a, 0x0010);  // Dose ref sequence
      gdcm::Tag fgcsq(0x300a, 0x0070);    // Fraction group sequence
      gdcm::Tag bcsq(0x300a, 0x00b0);     // Beam group sequence
      gdcm::Tag cpcsq(0x300a, 0x0111);    // Control pt sequence

      gdcm::Attribute<0x300a, 0x0002> planLabel;
      planLabel.SetFromDataElement(ds.GetDataElement(planLabel.GetTag()));
      // qDebug()<<planLabel.GetValue()<<"Plan label";
      this->planLabel = planLabel.GetValue();

      if (!ds.FindDataElement(fgcsq)) {
        // qDebug() << "Problem locating 0x3006,0x0039 - Is this a valid RT plan
        // file?";
        QMessageBox* msgBox = new QMessageBox;
        msgBox->setText("This is not a RT plan file!");
        msgBox->setIcon(QMessageBox::Critical);
        msgBox->exec();
        delete msgBox;
      }

      else {
        planDetail details;
        mlcDetail imrtMLCDetails;
        const gdcm::DataElement& fractionSq = ds.GetDataElement(fgcsq);
        gdcm::SmartPointer<gdcm::SequenceOfItems> fsqi =
            fractionSq.GetValueAsSQ();  // fraction sequence
        // qDebug()<<fsqi->GetNumberOfItems()<<"No. of fractions group";
        gdcm::Attribute<0x300a, 0x0078> fractionsPlanned;
        gdcm::Item& fgitem = fsqi->GetItem(1);

        const gdcm::DataSet& fnestedds = fgitem.GetNestedDataSet();
        fractionsPlanned.SetFromDataElement(
            fnestedds.GetDataElement(fractionsPlanned.GetTag()));
        // qDebug()<<fractionsPlanned.GetValue()<<"Fractions planned";
        this->fractionsPlanned = fractionsPlanned.GetValue();

        const gdcm::DataElement& beamSq = ds.GetDataElement(bcsq);
        gdcm::SmartPointer<gdcm::SequenceOfItems> bsqi =
            beamSq.GetValueAsSQ();  // fraction sequence
        int numOfBeams = bsqi->GetNumberOfItems();
        // qDebug()<<numOfBeams<<"No. of beams";
        this->numOfBeams = numOfBeams;

        for (int i = 1; i <= numOfBeams; i++) {
          gdcm::Item& beamitem = bsqi->GetItem(i);
          // Get m/c Name
          const gdcm::DataSet& beamds = beamitem.GetNestedDataSet();
          gdcm::Attribute<0x300a, 0x00b2> mcName;  // m/c name
          mcName.SetFromDataElement(beamds.GetDataElement(mcName.GetTag()));
          details.mcName = mcName.GetValue();
          // qDebug()<<mcName.GetValue()<<"m/c Name";

          // Get beam name
          gdcm::Attribute<0x300a, 0x00c2> beamName;
          beamName.SetFromDataElement(beamds.GetDataElement(beamName.GetTag()));
          details.beamName = beamName.GetValue();
          // qDebug()<<beamName.GetValue()<<"Beam Name";

          // Get beam No.
          gdcm::Attribute<0x300a, 0x00c0> beamNum;
          beamNum.SetFromDataElement(beamds.GetDataElement(beamNum.GetTag()));
          details.beamNum = beamNum.GetValue();
          // qDebug()<<beamNo.GetValue()<<"Beam No";

          // Get beam type
          gdcm::Attribute<0x300a, 0x00c4> beamType;
          beamType.SetFromDataElement(beamds.GetDataElement(beamType.GetTag()));
          details.beamType = beamType.GetValue();
          // qDebug()<<beamType.GetValue()<<"Beam Type";

          // Get control point sequence
          const gdcm::DataElement& cpSq = beamitem.GetDataElement(cpcsq);
          gdcm::SmartPointer<gdcm::SequenceOfItems> cptSqs =
              cpSq.GetValueAsSQ();  // fraction sequence
          // qDebug()<<cptSqs->GetNumberOfItems() <<"No. of ctrl pts";
          imrtMLCDetails.numCtrlPts = cptSqs->GetNumberOfItems();

          std::vector<std::vector<double> > mlcX1PosPerBeam;
          std::vector<std::vector<double> > mlcX2PosPerBeam;
          std::vector<double> muWeightsCurbeam;

          // Reading control point details
          for (int j = 1; j <= cptSqs->GetNumberOfItems(); j++) {
            gdcm::Item& cptitem = cptSqs->GetItem(j);
            const gdcm::DataSet& cptds =
                cptitem.GetNestedDataSet();  // ctrl pt dataSet

            // Except RapidArc and VMAT the beam details except mlc leaf
            // positions remain same for all ctrl pts
            // may vary for varian large field IMRT, where jaw carriages are
            // moved in the same field

            // Start of reading basic field details other than MLC
            if (j == 1) {
              // Get beam energy
              gdcm::Attribute<0x300a, 0x0114> beamEnergy;
              beamEnergy.SetFromDataElement(
                  cptds.GetDataElement(beamEnergy.GetTag()));
              details.beamEnergy = beamEnergy.GetValue();
              // qDebug()<<beamEnergy.GetValue()<<"Beam Energy";

              // Get arc direction
              gdcm::Attribute<0x300a, 0x011f> arcDir;
              arcDir.SetFromDataElement(cptds.GetDataElement(arcDir.GetTag()));
              details.arcDirection = arcDir.GetValue();
              // qDebug() << arcDir.GetValue() << "Arc direction";

              // Get Gantry angle
              gdcm::Attribute<0x300a, 0x011e> beamAngle;
              beamAngle.SetFromDataElement(
                  cptds.GetDataElement(beamAngle.GetTag()));
              details.beamAngle = beamAngle.GetValue();
              // qDebug()<<beamAngle.GetValue()<<"Beam Angle ";

              // Get Collimator angle
              gdcm::Attribute<0x300a, 0x0120> collAngle;
              collAngle.SetFromDataElement(
                  cptds.GetDataElement(collAngle.GetTag()));
              details.collAngle = collAngle.GetValue();
              // qDebug()<<collAngle.GetValue()<<"coll Angle ";

              // Get Couch angle
              gdcm::Attribute<0x300a, 0x0122> couchAngle;
              couchAngle.SetFromDataElement(
                  cptds.GetDataElement(couchAngle.GetTag()));
              details.couchAngle = couchAngle.GetValue();
              // qDebug()<<couchAngle.GetValue()<<"couch Angle ";

              // Get SSD
              gdcm::Attribute<0x300a, 0x0130> ssd;
              gdcm::Tag ssdTag(0x300a, 0x0130);
              if (cptds.FindDataElement(
                      ssdTag))  // May not be available in some cases
              {
                ssd.SetFromDataElement(cptds.GetDataElement(ssd.GetTag()));
                details.ssd = ssd.GetValue() / 10;  // mm to cm
                // qDebug()<<ssd.GetValue()<<"SSD";
              } else {
                details.ssd = 0.0;
                // qDebug()<<"SSD value not available";
              }

              // Get Beam limit device sequence
              gdcm::Tag beamLimit(0x300a, 0x011a);
              const gdcm::DataElement& beamLimitSq =
                  cptds.GetDataElement(beamLimit);
              gdcm::SmartPointer<gdcm::SequenceOfItems> beamLimitSqs =
                  beamLimitSq.GetValueAsSQ();  // beam limit device sequence
              // qDebug()<<beamLimitSqs->GetNumberOfItems()<<"No. of beam
              // limiting devices";

              // Get FieldX1 & FieldX2
              gdcm::Item deviceItemX = beamLimitSqs->GetItem(1);
              const gdcm::DataSet& beamLimitdsX =
                  deviceItemX.GetNestedDataSet();  // beam limit dataSet X
              gdcm::Attribute<0x300a, 0x011c> fieldX;
              fieldX.SetFromDataElement(
                  beamLimitdsX.GetDataElement(fieldX.GetTag()));
              details.fieldX1 = fieldX.GetValues()[0] / 10;  // mm to cm
              details.fieldX2 = fieldX.GetValues()[1] / 10;  // mm to cm
              // qDebug()<<fieldX.GetValues()[0]<<fieldX.GetValues()[1]<<"Field
              // X";

              // Get FieldY1 & FieldY2
              gdcm::Item deviceItemY = beamLimitSqs->GetItem(2);
              const gdcm::DataSet& beamLimitdsY =
                  deviceItemY.GetNestedDataSet();  // beam limit dataSet Y
              gdcm::Attribute<0x300a, 0x011c> fieldY;
              fieldY.SetFromDataElement(
                  beamLimitdsY.GetDataElement(fieldY.GetTag()));
              details.fieldY1 = fieldY.GetValues()[0] / 10;  // mm to cm
              details.fieldY2 = fieldY.GetValues()[1] / 10;  // mm to cm
              // qDebug()<<fieldY.GetValues()[0]<<fieldY.GetValues()[1]<<"Field
              // Y";

              // Get Isocenter X,Y&Z
              gdcm::Attribute<0x300a, 0x012c> isocenter;
              isocenter.SetFromDataElement(
                  cptds.GetDataElement(isocenter.GetTag()));
              details.icX = isocenter.GetValues()[0] / 10;  // IC X (mm to cm)
              details.icY = isocenter.GetValues()[1] / 10;  // IC Y (mm to cm)
              details.icZ = isocenter.GetValues()[2] / 10;  // IC Z (mm to cm)
            }
            // End of reading basic field details other than MLC

            if (j <= cptSqs->GetNumberOfItems()) {
              // Get Gantry angle
              gdcm::Attribute<0x300a, 0x011e> beamStopAngle;
              beamStopAngle.SetFromDataElement(
                  cptds.GetDataElement(beamStopAngle.GetTag()));
              details.beamStopAngle = beamStopAngle.GetValue();
              // qDebug()<<beamAngle.GetValue()<<"Beam Angle ";
            }

            // Get beam energy
            gdcm::Attribute<0x300a, 0x0134> beamWeight;
            beamWeight.SetFromDataElement(
                cptds.GetDataElement(beamWeight.GetTag()));
            muWeightsCurbeam.push_back(beamWeight.GetValue());
            // qDebug()<<beamWeight.GetValue()<<"Beam Weight"<<i;

            // Get Beam limit device sequence
            gdcm::Tag beamLimit(0x300a, 0x011a);
            if (cptds.FindDataElement(beamLimit)) {
              const gdcm::DataElement& beamLimitSq =
                  cptds.GetDataElement(beamLimit);
              gdcm::SmartPointer<gdcm::SequenceOfItems> beamLimitSqs =
                  beamLimitSq.GetValueAsSQ();  // beam limit device sequence
              // qDebug()<<beamLimitSqs->GetNumberOfItems()<<"No. of beam
              // limiting devices";

              gdcm::Item deviceItemMLC;
              // Get MLC leaf details
              if (beamLimitSqs->GetNumberOfItems() == 1) {
                deviceItemMLC = beamLimitSqs->GetItem(1);
              }

              else {
                deviceItemMLC = beamLimitSqs->GetItem(3);
              }

              const gdcm::DataSet& beamLimitMLC =
                  deviceItemMLC.GetNestedDataSet();  // beam limit dataSet MLC
              gdcm::Attribute<0x300a, 0x011c> mlcLeafPositions;

              mlcLeafPositions.SetFromDataElement(
                  beamLimitMLC.GetDataElement(mlcLeafPositions.GetTag()));
              // qDebug()<<mlcLeafPositions.GetNumberOfValues()<<"No. of
              // leaves";
              int numOfLeaves = mlcLeafPositions.GetNumberOfValues();

              std::vector<double> mlcX1PosCurrent;
              std::vector<double> mlcX2PosCurrent;

              // Get X1 leaves position
              for (int i = 0; i < numOfLeaves / 2; i++) {
                mlcX1PosCurrent.push_back(mlcLeafPositions.GetValues()[i]);
                // qDebug()<<mlcLeafPositions.GetValues()[i]<<"X1";
              }

              // Get X2 leaves position
              for (int i = numOfLeaves / 2; i < numOfLeaves; i++) {
                mlcX2PosCurrent.push_back(mlcLeafPositions.GetValues()[i]);
                // qDebug()<<mlcLeafPositions.GetValues()[i]<<"X2";
              }

              mlcX1PosPerBeam.push_back(mlcX1PosCurrent);
              mlcX2PosPerBeam.push_back(mlcX2PosCurrent);
            }

            imrtMLCDetails.mlcX1Pos = mlcX1PosPerBeam;
            imrtMLCDetails.mlcX2Pos = mlcX2PosPerBeam;
          }

          // Read last ctrl pt beam angle (=arc stop angle for VMAT)

          this->planDetailStruct.push_back(details);
          this->mlcDetailStruct.push_back(imrtMLCDetails);
          this->muWeights.push_back(muWeightsCurbeam);
        }

        gdcm::Tag refBeamSq(0x300c, 0x0004);  // Referenced beam sequence
        const gdcm::DataElement& refBeams = fgitem.GetDataElement(refBeamSq);
        gdcm::SmartPointer<gdcm::SequenceOfItems> refBeamItems =
            refBeams.GetValueAsSQ();  // ref beam sequence
        // qDebug()<<refBeamItems->GetNumberOfItems()<<"No. of referenced
        // beams";

        QList<int> beamNoList;
        // Initialize
        for (int i = 1; i <= refBeamItems->GetNumberOfItems(); i++) {
          this->planDetailStruct[i].mu = 0.0;
          this->planDetailStruct[i].beamDose = 0.0;
          beamNoList.push_back(this->planDetailStruct[i - 1].beamNum);
        }

        for (int i = 1; i <= refBeamItems->GetNumberOfItems(); i++) {
          gdcm::Attribute<0x300a, 0x0086> meterSet;
          gdcm::Attribute<0x300a, 0x0084> beamDose;
          gdcm::Item& refBeamItem = refBeamItems->GetItem(i);
          meterSet.SetFromDataElement(
              refBeamItem.GetDataElement(meterSet.GetTag()));
          // qDebug()<<meterSet.GetValue()<<"MU";

          gdcm::Attribute<0x300c, 0x0006> refBeamNum;
          refBeamNum.SetFromDataElement(
              refBeamItem.GetDataElement(refBeamNum.GetTag()));
          // qDebug()<<refBeamNum.GetValue()<<"Ref beam
          // No."<<meterSet.GetValue()<<"MU";

          // qDebug()<<beamNoList[i-1]<<"beam list";
          beamDose.SetFromDataElement(
              refBeamItem.GetDataElement(beamDose.GetTag()));
          // Put the MU corresponding to the beam reference No.
          unsigned int beamNoRef = this->planDetailStruct[i - 1].beamNum;
          unsigned int index = beamNoList.indexOf(beamNoRef);
          this->planDetailStruct[index].mu = meterSet.GetValue();
          // qDebug()<<beamDose.GetValue()<<"Beam Dose";
          this->planDetailStruct[index].beamDose = beamDose.GetValue();
          this->targetDose += beamDose.GetValue();
        }
      }
    }
    this->targetDose = this->targetDose * this->fractionsPlanned;
    // qDebug()<<this->targetDose<<" Gy"<<"Prescribed dose";
  }
}
