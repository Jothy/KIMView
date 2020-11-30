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
