/**************************************************************************
(c)Jothybasu Selvaraj
This program comes with no gaurantee of its accuracy.
Clinical decisions should not be made using this program
**************************************************************************/

#include "rtstructreaderdialog.h"
#include "ui_rtstructreaderdialog.h"

#define vtkRenderingCore_AUTOINIT 3(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingOpenGL2)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL2)

#include <gdcmReader.h>
#include<gdcmFile.h>
#include<gdcmSequenceOfItems.h>
#include<gdcmDataSet.h>
#include<gdcmTag.h>
#include<gdcmItem.h>
#include<gdcmNestedModuleEntries.h>
#include<gdcmDataElement.h>
#include <gdcmAttribute.h>

#include<QFileDialog>
#include<QMessageBox>
#include<QDebug>
#include<QListWidgetItem>

RTStructReaderDialog::RTStructReaderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RTStructReaderDialog)
{
    ui->setupUi(this);

    //For auto opening the file dialog
    this->ui->pushButtonChooseFile->click();


}

RTStructReaderDialog::~RTStructReaderDialog()
{
    delete ui;
}

void RTStructReaderDialog::on_pushButtonChooseFile_clicked()
{
    this->ui->listWidgetStructs->clear();
    this->structFileName=QFileDialog::getOpenFileName(this,"Open RT Structures");
    this->ui->lineEdit->setText(this->structFileName);
    gdcm::Reader RTreader;
    RTreader.SetFileName(this->structFileName.toLatin1().data());

    //show error msg if file can't be read.
    if( !RTreader.Read() )
    {
        QMessageBox msgBox;
        msgBox.setText("Not a valid DICOM file!");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }


    else//Start reading the necessary info
    {        
        const gdcm::DataSet& ds = RTreader.GetFile().GetDataSet();

        gdcm::Tag tssroisq(0x3006,0x0020);//Structure set ROI sequence
        if( !ds.FindDataElement( tssroisq ) )
        {           
            //qDebug() <<"Problem locating 0x3006,0x0020 - Is this a valid RT Struct file?";
            QMessageBox msgBox;
            msgBox.setText("This is not a valid RT structure file!");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
        }

        else
        {

            gdcm::Tag troicsq(0x3006,0x0039);//ROI contour sequence
            if( !ds.FindDataElement(troicsq ))
            {
                //qDebug() << "Problem locating 0x3006,0x0039 - Is this a valid RT Struct file?";
                QMessageBox msgBox;
                msgBox.setText("This is not a RT structure file!");
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.exec();
            }
            else
            {

                gdcm::Tag troiobsq(0x3006,0x0080);//ROI observation sequence
                const gdcm::DataElement &roicsq = ds.GetDataElement( troicsq );
                gdcm::SmartPointer<gdcm::SequenceOfItems> sqi = roicsq.GetValueAsSQ();

                const gdcm::DataElement &ssroisq = ds.GetDataElement( tssroisq );
                gdcm::SmartPointer<gdcm::SequenceOfItems> ssqi = ssroisq.GetValueAsSQ();
                this->NumberOfROIs=sqi->GetNumberOfItems();
                //qDebug()<< "Number of structures found:" <<  this->NumberOfROIs;

                const gdcm::DataElement &roiobsq = ds.GetDataElement( troiobsq );
                gdcm::SmartPointer<gdcm::SequenceOfItems> obsq = roiobsq.GetValueAsSQ(); //observation sequence
                qDebug()<<"No. of ROIs found:"<<this->NumberOfROIs;
                if(this->NumberOfROIs>50)
                {
                    this->NumberOfROIs=50;//Only read the first 50 ROIs
                    QMessageBox messageBox;
                    messageBox.warning(this,"Warning","More than 50 ROIs found, only reading the first 50.");
                    messageBox.setFixedSize(500,200);

                }

                for (int i=1;i<this->NumberOfROIs+1;i++)
                {

                    //Get the ROI Nos.
                    gdcm::Item & item=sqi->GetItem(i);
                    const gdcm::DataSet& nestedds = item.GetNestedDataSet();

                    //Get ROIs  color
                    gdcm::Attribute<0x3006,0x002a> roiColor;//ROI color tag
                    roiColor.SetFromDataElement( nestedds.GetDataElement( roiColor.GetTag() ));
                    double R=roiColor.GetValues()[0];
                    double G=roiColor.GetValues()[1];
                    double B=roiColor.GetValues()[2];
                    // qDebug()<<"ROi color:"<<R<<G<<B;
                    this->ROIColors[i-1][0]=R; this->ROIColors[i-1][1]=G; this->ROIColors[i-1][2]=B;//C++ array starts from "0".

                    //Get ROI Names
                    gdcm::Item & sitem=ssqi->GetItem(i);
                    gdcm::Attribute<0x3006,0x0026> roiName; //ROI Name tag in Structure Set ROI Sequence
                    const gdcm::DataSet& snestedds = sitem.GetNestedDataSet();
                    roiName.SetFromDataElement(snestedds.GetDataElement( roiName.GetTag()));
                    // qDebug()<<roiName.GetValue();
                    this->ROINames.append(QString(roiName.GetValue()));

                    QListWidgetItem *wItem = new QListWidgetItem(QString(roiName.GetValue()));
                    wItem->setCheckState(Qt::Unchecked);
                    wItem->setBackgroundColor(QColor(R,G,B));
                    this->ui->listWidgetStructs->addItem(wItem);                    

                }
            }
        }
    }

}

void RTStructReaderDialog::getSelectedItems()
{
    this->selectedItems.clear();
    if(this->ui->groupBox->isChecked())
    {
        for(int i=0;i<this->NumberOfROIs;i++)
        {
            int checkState=this->ui->listWidgetStructs->item(i)->checkState();
//           qDebug()<<checkState<<"checkState";
            if(checkState==2)
            {
                this->selectedItems.append(i+1);//In GDCM dicom items start from "1";
            }
        }

    }
    else
    {
        for(int i=0;i<this->NumberOfROIs;i++)
        {
            this->selectedItems.append(i+1);
        }

    }

}

void RTStructReaderDialog::on_pushButtonRead_clicked()
{
    this->getSelectedItems();
    this->close();

}
