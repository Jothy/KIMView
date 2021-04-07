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
#ifndef WLWWDIALOG_H
#define WLWWDIALOG_H

#include <imageviewer2d.h>

#include <QDialog>

namespace Ui {
class WLWWDialog;
}

class WLWWDialog : public QDialog {
  Q_OBJECT

 public:
  explicit WLWWDialog(QWidget *parent = nullptr);
  ~WLWWDialog();
  ImageViewer2D *AxialViewer;
  ImageViewer2D *CoronalViewer;
  ImageViewer2D *SagittalViewer;

 private slots:

  void on_pushButtonReset_clicked();

  void on_ScrollBar_level_sliderMoved(int position);

  void on_ScrollBar_width_sliderMoved(int position);

 private:
  Ui::WLWWDialog *ui;
};

#endif  // WLWWDIALOG_H
