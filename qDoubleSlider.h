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

#ifndef Q_DOUBLE_SLIDER_H
#define Q_DOUBLE_SLIDER_H
#include <QtGui/QtGui>
#include <QSlider>
class QDoubleSlider : public QSlider
{
    Q_OBJECT

public:
    QDoubleSlider(QWidget* pParent = NULL);

    void setRange(double Min, double Max);
    void setMinimum(double Min);
    double minimum() const;
    void setMaximum(double Max);
    double maximum() const;
    double value() const;

    public slots:
    void setValue(int value);
    void setValue(double Value, bool BlockSignals = false);

    private slots:

signals :
    void valueChanged(double Value);
    void rangeChanged(double Min, double Max);

private:
    double	m_Multiplier;
};
#endif
