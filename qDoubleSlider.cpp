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

#include "qDoubleSlider.h"
QDoubleSlider::QDoubleSlider(QWidget* pParent/*= NULL*/) :
QSlider(pParent),
m_Multiplier(10000.0)
{
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));

    setSingleStep(1);

    setOrientation(Qt::Horizontal);
    setFocusPolicy(Qt::NoFocus);
}

void QDoubleSlider::setValue(int Value)
{
    emit valueChanged((double)Value/m_Multiplier);
}

void QDoubleSlider::setValue(double Value, bool BlockSignals)
{
    QSlider::blockSignals(BlockSignals);

    QSlider::setValue(Value * m_Multiplier);

    if (!BlockSignals)
        emit valueChanged(Value);

    QSlider::blockSignals(false);
}

void QDoubleSlider::setRange(double Min, double Max)
{
    QSlider::setRange(Min * m_Multiplier, Max * m_Multiplier);

    emit rangeChanged(Min, Max);
}

void QDoubleSlider::setMinimum(double Min)
{
    QSlider::setMinimum(Min * m_Multiplier);

    emit rangeChanged(minimum(), maximum());
}

double QDoubleSlider::minimum() const
{
    return QSlider::minimum()/m_Multiplier;
}

void QDoubleSlider::setMaximum(double Max)
{
    QSlider::setMaximum(Max * m_Multiplier);

    emit rangeChanged(minimum(), maximum());
}

double QDoubleSlider::maximum() const
{
    return QSlider::maximum()/m_Multiplier;
}

double QDoubleSlider::value() const
{
    int Value = QSlider::value();
    return (double)Value/m_Multiplier;
}
