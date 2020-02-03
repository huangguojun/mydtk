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

#include "dtkWidgetsParameterBoolCheckBox.h"

#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterBoolCheckBoxPrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsParameterBoolCheckBoxPrivate
{
public:
    QCheckBox *check_box = nullptr;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterBoolCheckBox implementation
// ///////////////////////////////////////////////////////////////////

dtkWidgetsParameterBoolCheckBox::dtkWidgetsParameterBoolCheckBox(QWidget* parent) : dtkWidgetsParameterBase<dtk::d_bool>(parent), d(new dtkWidgetsParameterBoolCheckBoxPrivate)
{
    d->check_box = new QCheckBox;

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(d->check_box);

    this->setLayout(layout);
}

dtkWidgetsParameterBoolCheckBox::~dtkWidgetsParameterBoolCheckBox(void)
{
    delete d;
}

bool dtkWidgetsParameterBoolCheckBox::connect(dtkCoreParameter *p)
{
    if (!p) {
        qWarning() << Q_FUNC_INFO << "The input parameter is null. Nothing is done.";
        return false;
    }

    m_parameter = dynamic_cast<dtk::d_bool *>(p);

    if(!m_parameter) {
        qWarning() << Q_FUNC_INFO << "The type of the parameter is not compatible with the widget dtkWidgetsParameterBoolCheckBox.";
        return false;
    }

    d->check_box->setToolTip(m_parameter->documentation());

    if(m_parameter->value()) {
        d->check_box->setCheckState(Qt::Checked);
    } else {
        d->check_box->setCheckState(Qt::Unchecked);
    }


    m_parameter->connect([=] (QVariant v)
    {
        bool value = v.value<dtk::d_bool>().value();
        d->check_box->blockSignals(true);
        if (value) {
            d->check_box->setCheckState(Qt::Checked);
        } else {
            d->check_box->setCheckState(Qt::Unchecked);
        }
        d->check_box->blockSignals(false);
    });

    QObject::connect(d->check_box, &QCheckBox::stateChanged, [=] (int state)
    {
        m_parameter->shareValue(QVariant::fromValue(state == Qt::Checked));
    });

    return true;
}

//
// dtkWidgetsParameterBoolCheckBoxWidget.cpp ends here
