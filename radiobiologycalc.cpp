/**************************************************************************
(c)Jothybasu Selvaraj
This program comes with no gaurantee of its accuracy.
Clinical decisions should not be made using this program
**************************************************************************/

#include "radiobiologycalc.h"
#include"mathutility.h"
//#include"DEIntegrator.h"

#include"math.h"
#include"cmath"
#include"numeric"
#include<time.h>
#include<QDebug>

#include<vtkMath.h>
#include<vtkSmartPointer.h>

RadiobiologyCalc::RadiobiologyCalc()
{
    this->T=26;//for 20#
    this->Td=3;//Default value in days
    this->Tk=21;//Days
    this->clonogenDensity=1e7;
    this->fractions=30.0;
    this->alpha=0.30;
    this->alphaByBeta=10;
    this->alphaStd=0.03;
    this->alphaMean=0.30;
    this->voxelVol=1.0;
    this->temporalCF=0.0;
    this->calcType=0;//Defaults to uniform clonogens

    //vtkMath::RandomSeed(time(NULL));

}

RadiobiologyCalc::~RadiobiologyCalc()
{
    this->T=0;//for 20#
    this->Td=0;//Default value in days
    this->Tk=0;//Days
    this->fractions=0;
    this->clonogenDensity=0;
    this->alpha=0.0;
    this->alphaByBeta=0.0;
    this->alphaStd=0.0;
    this->alphaMean=0.0;
    this->DoseBins.clear();
    this->VolBins.clear();
}

double RadiobiologyCalc::calcVCP(double dosePerFraction,double voxelVolume,double totalDose,double alpha,
                                 double alphaByBeta,double clonogenDensity,double temporalCorrectionFactor)
{
    double vcp;
    vcp=exp(-clonogenDensity*voxelVolume*(exp((-alpha*totalDose*(1.0+(dosePerFraction/alphaByBeta)))+temporalCorrectionFactor)));
    //qDebug()<<voxelVolume<<"voxelVolume";
    //qDebug()<<vcp<<"VCP";
    return vcp;
}

double RadiobiologyCalc::calcTCP(double alpha)
{
    int DoseBinSize=this->DoseBins.size();   
    std::vector<double>VCPs;
    for(int i=0;i<DoseBinSize;i++)
    {
        double d=this->DoseBins[i]/(this->fractions);
        double vol=this->VolBins[i]*this->voxelVol;        
        double vcp=this->calcVCP(d,vol,this->DoseBins[i],alpha,this->alphaByBeta,this->clonogenDensity,this->temporalCF);
        VCPs.push_back(vcp);
    }

    MathUtility* productCalc=new MathUtility;
    double TCP= productCalc->productOfVector(VCPs);
    delete productCalc;
    double ff=std::exp(-(std::pow((alpha-this->alphaMean),2)/(2.0*std::pow(this->alphaStd,2))));
    //qDebug()<<alpha<<alphaMean<<alphaStd<<ff<<"ff";
    double TCPTotal=TCP*(ff);
    //qDebug()<<TCP<<TCPTotal<<"TCP,TCPTotal";
    //qDebug()<<TCP<<"TCP";
    if(this->alphaStd==0)
    {
        return TCP*100;
    }

    else
    {
        return TCPTotal;
    }
}


double RadiobiologyCalc::calcTCPVaryingClonogens(double alpha)
{
    int DoseValueSize=this->doseValues.size();
    //qDebug()<<DoseValueSize<<"DoseBinSize";
    std::vector<double>VCPs;
    for(int i=0;i<DoseValueSize;i++)
    {        
        double d=this->doseValues[i]/(this->fractions);
        double vol=0.001;//Since the resolution of dose & clonogen grids is 1mm
        double clonogenDensityFactor=static_cast<double>(this->clonogensVector[i]/100.0);
        //qDebug()<<clonogenDensityFactor<<"CD factor"<<i;
        double vcp=this->calcVCP(d,vol,this->doseValues[i],alpha,this->alphaByBeta,this->clonogenDensity*clonogenDensityFactor,this->temporalCF);
        //qDebug()<<vcp<<"VCP";
        VCPs.push_back(vcp);
    }

    MathUtility* productCalc=new MathUtility;
    double TCP= productCalc->productOfVector(VCPs);
    delete productCalc;
    double ff=std::exp(-(std::pow((alpha-this->alphaMean),2)/(2.0*std::pow(this->alphaStd,2))));

    double TCPTotal=TCP*(ff);
    if(this->alphaStd==0)
    {
        return TCP*100;
    }

    else
    {
        return TCPTotal;
    }

}


//Total treatment time T is not properly calculated
double RadiobiologyCalc::calcTempCorrection(int Td,int Tk,int T)
{
    double correction;
    if (T<Tk)
    {
        correction=0.0;
//        qDebug()<<Td<<Tk<<T<<correction<<"Td,Tk,T,Temp CF00000";
    }

else
{
    double gamma=std::log(2)/Td;
    double  TMinusTk=(T-Tk);
    correction=gamma*TMinusTk;
}
//qDebug()<<Td<<Tk<<T<<correction<<"Td,Tk,T,Temp CF";
return correction;

}

double RadiobiologyCalc::calcNormalTCP(double start,double stop,double step)
{    
    this->temporalCF=0;//Ignore repopulation for now
    double accumulator=0.0;
    for (double i=start;i<=stop;i+=step)
    {
        if(this->calcType==0)
        {
            accumulator+=(this->calcTCP((i+(step/2.0)))*step);
        }
        else
        {
            accumulator+=(this->calcTCPVaryingClonogens((i+(step/2.0)))*step);
        }
    }
    double TCPFinal=(1.0/(this->alphaStd*std::sqrt(2*vtkMath::Pi())))*accumulator;
    //qDebug()<<TCPFinal*100.0<<"TCP Final"<<accumulator<<"accum"<<(1.0/(this->alphaStd*std::sqrt(2*3.1416)));
    return TCPFinal*100.0;
}

double RadiobiologyCalc::calcTCPRandomAlpha(double randAlpha)
{

    if(randAlpha<0)
    {
        randAlpha=0.0;
    }

        int DoseBinSize=this->DoseBins.size();
        std::vector<double>VCPs;
        for(int i=0;i<DoseBinSize;i++)
        {
            double d=this->DoseBins[i]/(this->fractions);
            double vol=this->VolBins[i]*this->voxelVol;
            double vcp=this->calcVCP(d,vol,this->DoseBins[i],randAlpha,this->alphaByBeta,this->clonogenDensity,this->temporalCF);
            VCPs.push_back(vcp);
        }

        MathUtility* productCalc=new MathUtility;
        double TCP= productCalc->productOfVector(VCPs);
        delete productCalc;

        if(TCP>=0)
        {
            return TCP ;
        }



}

