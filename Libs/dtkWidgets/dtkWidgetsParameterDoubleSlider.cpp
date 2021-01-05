// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkWidgetsParameterDoubleSlider.h"

#include <QtWidgets>

#include <dtkThemes/dtkThemesEngine>

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterDoubleSliderPrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsParameterDoubleSliderPrivate
{
public:
    QSlider *slider = nullptr;
    QLabel *min = nullptr;
    QLabel *max = nullptr;
    QLineEdit *val = nullptr;

    double factor = 1.0;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterDoubleSlider implementation
// ///////////////////////////////////////////////////////////////////

dtkWidgetsParameterDoubleSlider::dtkWidgetsParameterDoubleSlider(QWidget *parent)
    : dtkWidgetsParameterBase<dtk::d_real>(parent), d(new dtkWidgetsParameterDoubleSliderPrivate)
{

    d->min = new QLabel("min");
    d->max = new QLabel("max");
    d->val = new QLineEdit;
    d->slider = new QSlider;

    d->slider->setOrientation(Qt::Horizontal);
    d->slider->setTickPosition(QSlider::TicksBelow);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(d->val);
    layout->addWidget(d->min);
    layout->addWidget(d->slider);
    layout->addWidget(d->max);

    QObject::connect(d->slider, &QSlider::valueChanged,
                     [=](int value) { d->val->setText(QString::number(value / d->factor)); });
    QObject::connect(d->val, &QLineEdit::textChanged, [=](const QString &v) {
        double value = v.toDouble();
        d->slider->setValue(value * d->factor);
    });

    this->setLayout(layout);
}

dtkWidgetsParameterDoubleSlider::~dtkWidgetsParameterDoubleSlider(void)
{
    delete d;
}

void dtkWidgetsParameterDoubleSlider::resizeEvent(QResizeEvent *event)
{
    dtkWidgetsParameterBase::resizeEvent(event);
    d->val->setFixedWidth(event->size().width() / 3);
}

bool dtkWidgetsParameterDoubleSlider::connect(dtkCoreParameter *p)
{
    if (!p) {
        qWarning() << Q_FUNC_INFO << "The input parameter is null. Nothing is done.";
        return false;
    }

    m_parameter = dynamic_cast<dtk::d_real *>(p);

    if (!m_parameter) {
        qWarning() << Q_FUNC_INFO
                   << "The type of the parameter is not compatible with the "
                      "widget dtkWidgetsParameterDoubleSlider.";
        return false;
    }
    int decimals = m_parameter->decimals();
    d->factor = std::pow(10, decimals);
    int minimum = m_parameter->min() * d->factor;
    int maximum = m_parameter->max() * d->factor;
    d->slider->setMinimum(minimum);
    d->slider->setMaximum(maximum);
    d->min->setText(QString::number(m_parameter->min()));
    d->max->setText(QString::number(m_parameter->max()));
    d->slider->setToolTip(m_parameter->documentation());
    int value = m_parameter->value() * d->factor;
    // try to get the number of digits (integer part + decimals). first get the
    // integer part
    int max_i = std::max((int)std::abs(m_parameter->max()), (int)std::abs(m_parameter->min()));
    int max_chars = std::to_string(max_i).length() + 1 + decimals;
    d->val->setMaxLength(max_chars);
    QString text = QString::number(m_parameter->value());
    d->val->setText(text);
    d->slider->setValue(value);
    d->slider->setTickInterval((maximum - minimum) / 10);

    m_parameter->connect([=](QVariant v) {
        double value = v.value<dtk::d_real>().value();
        d->slider->blockSignals(true);
        d->slider->setValue(value * d->factor);
        d->val->setText(QString::number(value));
        d->slider->blockSignals(false);
    });

    QObject::connect(d->slider, QOverload<int>::of(&QSlider::valueChanged), [=](int v) {
        double value = (double)v / d->factor;
        m_parameter->shareValue(QVariant::fromValue(value));
        d->val->setText(QString::number(value));
    });

    return true;
}

//
// dtkWidgetsParameterDoubleSliderWidget.cpp ends here
