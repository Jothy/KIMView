/**************************************************************************
(c)Jothybasu Selvaraj
This program comes with no gaurantee of its accuracy.
Clinical decisions should not be made using this program
**************************************************************************/

#ifndef RADIOBIOLOGYCALC_H
#define RADIOBIOLOGYCALC_H

#include"vector"

class RadiobiologyCalc
{
public:
    RadiobiologyCalc();
    ~RadiobiologyCalc();
    //Doses are in Gy and volumes are in cc
    double calcVCP(double dosePerFraction,double voxelVolume,double totalDose,double alpha,
                   double alphaByBeta,double clonogenDensity,double temporalCorrectionFactor);
    double calcTCP(double alpha);
    double calcTCPVaryingClonogens(double alpha);
    double calcNormalTCP(double start,double stop,double step);
    double calcTempCorrection(int Td,int Tk,int T);

    double Td;//Doubling time
    double Tk;//Time to kick start repopulation
    double T;//Overall treatment time
    std::vector<double>DoseBins;
    std::vector<double>doseValues;
    std::vector<double>VolBins;
    std::vector<float>clonogensVector;
    double alphaByBeta;
    double alpha;
    double alphaMean;
    double alphaStd;
    double clonogenDensity;
    double fractions;
    double temporalCF;
    double voxelVol;
    bool calcType;
    double calcTCPRandomAlpha(double randAlpha);


};

#endif // RADIOBIOLOGYCALC_H
