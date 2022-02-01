/**************************************************************************
(c)Jothybasu Selvaraj
This program comes with no gaurantee of its accuracy.
Clinical decisions should not be made using this program
**************************************************************************/

#include "tcpdialog.h"
#include "ui_tcpdialog.h"

#include"radiobiologycalc.h"
#include"dvhcalc.h"

#include<QString>
#include<QList>
#include<vtkImageData.h>
#include<vtkPolyData.h>

#include<QDebug>
#include<QMessageBox>

TCPDialog::TCPDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TCPDialog)
{
    ui->setupUi(this);
    this->calcType=0;//Defaults to uniform clonogen in CTV
}

TCPDialog::~TCPDialog()
{
    delete ui;
}

void TCPDialog::setStructureNames(QList<QString>structures)
{
    const int itemNo=structures.count();//No. of items in QList

    for (int n=0;n<itemNo;n++)
    {
        this->ui->comboBox->addItem(structures[n]);
    }

}

void TCPDialog::on_pushButton_clicked()
{
    if(this->calcType==0)
    {
        this->calcTCP();
    }

    else if (this->calcType==1)
    {
     this->calcTCPVaryingClonogens();
    }

}




void TCPDialog::calcTCP()
{
    this->alpha=this->ui->alpahMeanValue->text().toDouble();
    this->alphaByBeta=this->ui->alphaBetaValue->text().toDouble();
    this->alphaStd=this->ui->alphaStdValue->text().toDouble();
    this->clonogenDensity=this->ui->CDValue->text().toDouble();
    this->alphaMean=this->alpha;

    this->selectedStructureNum=this->ui->comboBox->currentIndex();
    DVHCalc* myDVHCalc=new DVHCalc;
    std::vector<double>results;
    this->structure=this->meshes[this->selectedStructureNum];
    results=myDVHCalc->calcStructDVH(this->structure,this->doseGrid);

    myDVHCalc->histogramData2(0.02,myDVHCalc->doseValues,results[5],1);
    this->voxelVol=results[5];

    RadiobiologyCalc* RBcalc=new RadiobiologyCalc;
    RBcalc->DoseBins=myDVHCalc->doseBins;
    RBcalc->VolBins=myDVHCalc->volBins;
    RBcalc->alpha=this->alpha;
    RBcalc->alphaMean=this->alpha;
    RBcalc->alphaByBeta=this->alphaByBeta;
    RBcalc->alphaStd=this->alphaStd;
    RBcalc->clonogenDensity=this->clonogenDensity;
    RBcalc->fractions=this->fractions;
    RBcalc->voxelVol=results[5];
    RBcalc->Td=this->ui->TdValue->value();
    RBcalc->Tk=this->ui->TkValue->value();
    RBcalc->T=((this->fractions/5)*7)-1.6;//Assuming 5 frs/week

    this->doseBins=myDVHCalc->doseBins;
    this->volBins=myDVHCalc->volBins;

    double TCP;

    if(this->alphaStd==0)
    {

       TCP=RBcalc->calcTCP(this->alpha);
    }
    else
    {

    double stop=this->alphaMean+(this->alphaStd*3.0)+0.0;
    double start=this->alphaMean-(this->alphaStd*3.0)-0.0;
    if(start<0)
    {
        start=0.0;
    }
    //qDebug()<<stop<<"stop"<<start<<"start";
    TCP=RBcalc->calcNormalTCP(start,stop,0.01);
}
    delete RBcalc;
    delete myDVHCalc;

    QMessageBox* msg=new QMessageBox;
    msg->setText(QString::number(TCP,'f',1)+" %");
    msg->setIcon(QMessageBox::Information);
    msg->setWindowTitle("TCP");
    msg->setTextFormat(Qt::RichText);
    QFont font;
    font.setPointSize(11);
    font.setBold(true);
    font.setWeight(50);
    msg->setFont(font);
    QIcon msgIcon;
    msgIcon.addFile(QString::fromUtf8(":/Icons/Icons/wand.png"), QSize(), QIcon::Normal, QIcon::Off);
    msg->setWindowIcon(msgIcon);
    msg->exec();
    delete msg;
}


void TCPDialog::calcTCPVaryingClonogens()
{
    this->alpha=this->ui->alpahMeanValue->text().toDouble();
    this->alphaByBeta=this->ui->alphaBetaValue->text().toDouble();
    this->alphaStd=this->ui->alphaStdValue->text().toDouble();
    this->clonogenDensity=this->ui->CDValue->text().toDouble();
    this->alphaMean=this->alpha;

    this->selectedStructureNum=this->ui->comboBox->currentIndex();
    DVHCalc* myDVHCalc=new DVHCalc;
    std::vector<double>results;
    this->structure=this->meshes[this->selectedStructureNum];
    results=myDVHCalc->calcStructDVH(this->structure,this->doseGrid);
    //this->clonogenVector=myDVHCalc->getClonogensAsVector(this->clonogenGrid);

    //myDVHCalc->histogramData2(0.02,myDVHCalc->doseValues,results[5],1);
    this->voxelVol=results[5];

    RadiobiologyCalc* RBcalc=new RadiobiologyCalc;
    RBcalc->calcType=1;
    RBcalc->doseValues=myDVHCalc->doseValues;
    RBcalc->VolBins=myDVHCalc->volBins;
    RBcalc->clonogensVector=myDVHCalc->getClonogensAsVector(this->clonogenGrid);
    RBcalc->alpha=this->alpha;
    RBcalc->alphaMean=this->alpha;
    RBcalc->alphaByBeta=this->alphaByBeta;
    RBcalc->alphaStd=this->alphaStd;
    RBcalc->clonogenDensity=this->clonogenDensity;
    RBcalc->fractions=this->fractions;
    RBcalc->voxelVol=results[5];   
    RBcalc->Td=this->ui->TdValue->value();
    RBcalc->Tk=this->ui->TkValue->value();
    RBcalc->T=((this->fractions/5)*7)-1.6;//Assuming 5 frs/week

    this->doseBins=myDVHCalc->doseBins;
    this->volBins=myDVHCalc->volBins;

    double TCP;

    if(this->alphaStd==0)
    {

       TCP=RBcalc->calcTCP(this->alpha);
    }
    else
    {

    double stop=this->alphaMean+(this->alphaStd*3.0)+0.0;
    double start=this->alphaMean-(this->alphaStd*3.0)-0.0;
    if(start<0)
    {
        start=0.0;
    }
    //qDebug()<<stop<<"stop"<<start<<"start";
    TCP=RBcalc->calcNormalTCP(start,stop,0.01);
}
    delete RBcalc;
    delete myDVHCalc;

    QMessageBox* msg=new QMessageBox;
    msg->setText(QString::number(TCP,'f',1)+" %");
    msg->setIcon(QMessageBox::Information);
    msg->setWindowTitle("TCP");
    msg->setTextFormat(Qt::RichText);
    QFont font;
    font.setPointSize(11);
    font.setBold(true);
    font.setWeight(50);
    msg->setFont(font);
    QIcon msgIcon;
    msgIcon.addFile(QString::fromUtf8(":/Icons/Icons/wand.png"), QSize(), QIcon::Normal, QIcon::Off);
    msg->setWindowIcon(msgIcon);
    msg->exec();
    delete msg;
}
