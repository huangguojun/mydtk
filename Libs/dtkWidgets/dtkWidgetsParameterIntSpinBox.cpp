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

#include "dtkWidgetsParameterIntSpinBox.h"

#include "dtkWidgetsSpinBoxLong.h"

#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterIntSpinBoxPrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsParameterIntSpinBoxPrivate
{
public:
    dtkWidgetsSpinBoxLong *spin_box = nullptr;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterIntSpinBox implementation
// ///////////////////////////////////////////////////////////////////

dtkWidgetsParameterIntSpinBox::dtkWidgetsParameterIntSpinBox(QWidget* parent) : dtkWidgetsParameterBase<dtk::d_int>(parent), d(new dtkWidgetsParameterIntSpinBoxPrivate)
{
    d->spin_box = new dtkWidgetsSpinBoxLong;

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(d->spin_box);

    this->setLayout(layout);
}

dtkWidgetsParameterIntSpinBox::~dtkWidgetsParameterIntSpinBox(void)
{
    delete d;
}

bool dtkWidgetsParameterIntSpinBox::connect(dtkCoreParameter *p)
{
    if (!p) {
        qWarning() << Q_FUNC_INFO << "The input parameter is null. Nothing is done.";
        return false;
    }

    m_parameter = dynamic_cast<dtk::d_int *>(p);

    if(!m_parameter) {
        qWarning() << Q_FUNC_INFO << "The type of the parameter is not compatible with the widget dtkWidgetsParameterIntSpinBox.";
        return false;
    }

    d->spin_box->setMinimum(m_parameter->min());
    d->spin_box->setMaximum(m_parameter->max());
    d->spin_box->setToolTip(m_parameter->documentation());
    d->spin_box->setKeyboardTracking(false);

    d->spin_box->setValue(m_parameter->value());

    m_parameter->connect([=] (QVariant v)
    {
        int value = v.value<dtk::d_int>().value();
        d->spin_box->blockSignals(true);
        d->spin_box->setValue(value);
        d->spin_box->blockSignals(false);
    });

    //Note: here the type is "long long" instead of "int", to inherit the focus manager from the BaseSpinBox.
    QObject::connect(d->spin_box, QOverload<long long>::of(&dtkWidgetsSpinBoxLong::valueChanged), [=] (int v)
    {
        m_parameter->shareValue(QVariant::fromValue(v));
    });

    return true;
}

//
// dtkWidgetsParameterIntSpinBoxWidget.cpp ends here
