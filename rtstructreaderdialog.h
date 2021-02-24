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

#ifndef RTSTRUCTREADERDIALOG_H
#define RTSTRUCTREADERDIALOG_H

#include <QDialog>

namespace Ui {
    class RTStructReaderDialog;
}

class RTStructReaderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RTStructReaderDialog(QWidget *parent = 0);
    ~RTStructReaderDialog();
    int NumberOfROIs;
    double ROIColors[50][3]; //No. of ROIs,RGB- maximum 50 ROIs supported for now
    QList<QString> ROINames;
    QList<int>selectedItems;
    QString structFileName;
    void getSelectedItems();


private:
    Ui::RTStructReaderDialog *ui;

private slots:
    void on_pushButtonRead_clicked();
    void on_pushButtonChooseFile_clicked();
};

#endif // RTSTRUCTREADERDIALOG_H
