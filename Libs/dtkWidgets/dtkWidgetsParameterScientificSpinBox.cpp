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

#include "dtkWidgetsParameterScientificSpinBox.h"
#include "dtkWidgetsSpinBoxDouble.h"

#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterScientificSpinBoxPrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsParameterScientificSpinBoxPrivate
{
public:
    dtkWidgetsSpinBoxDouble *spin_box = nullptr;

};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterScientificSpinBox implementation
// ///////////////////////////////////////////////////////////////////

dtkWidgetsParameterScientificSpinBox::dtkWidgetsParameterScientificSpinBox(QWidget* parent) : dtkWidgetsParameterBase<dtk::d_real>(parent), d(new dtkWidgetsParameterScientificSpinBoxPrivate)
{
    d->spin_box = new dtkWidgetsSpinBoxDouble(QDoubleValidator::ScientificNotation);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(d->spin_box);

    this->setLayout(layout);
}

dtkWidgetsParameterScientificSpinBox::~dtkWidgetsParameterScientificSpinBox(void)
{
    delete d;
}

bool dtkWidgetsParameterScientificSpinBox::connect(dtkCoreParameter *p)
{
    if (!p) {
        qWarning() << Q_FUNC_INFO << "The input parameter is null. Nothing is done.";
        return false;
    }

    m_parameter = dynamic_cast<dtk::d_real *>(p);

    if(!m_parameter) {
        qWarning() << Q_FUNC_INFO << "The type of the parameter is not compatible with the widget dtkWidgetsParameterIntSpinBox.";
        return false;
    }

    d->spin_box->setMinimum(m_parameter->min());
    d->spin_box->setMaximum(m_parameter->max());
    d->spin_box->setToolTip(m_parameter->documentation());

    d->spin_box->setValue(m_parameter->value());
    d->spin_box->setDecimals(m_parameter->decimals());

    m_parameter->connect([=] (QVariant v)
    {
        double value = v.value<dtk::d_real>().value();
        d->spin_box->blockSignals(true);
        d->spin_box->setValue(value);
        d->spin_box->blockSignals(false);
    });

    QObject::connect(d->spin_box, QOverload<double>::of(&dtkWidgetsSpinBoxDouble::valueChanged), [=] (double v)
    {
        m_parameter->shareValue(QVariant::fromValue(v));
    });

    return true;
}

//
// dtkWidgetsParameterScientificSpinBoxWidget.cpp ends here
