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

#include "dtkWidgetsParameterBoolPushButton.h"

#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterBoolPushButtonPrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsParameterBoolPushButtonPrivate
{
public:
    QPushButton *push_button = nullptr;

    bool running = false;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterBoolPushButton implementation
// ///////////////////////////////////////////////////////////////////

dtkWidgetsParameterBoolPushButton::dtkWidgetsParameterBoolPushButton(QWidget *parent)
    : dtkWidgetsParameterBase<dtk::d_bool>(parent), d(new dtkWidgetsParameterBoolPushButtonPrivate)
{
    d->push_button = new QPushButton;

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(d->push_button);

    this->setLayout(layout);
}

dtkWidgetsParameterBoolPushButton::~dtkWidgetsParameterBoolPushButton(void)
{
    delete d;
}

bool dtkWidgetsParameterBoolPushButton::connect(dtkCoreParameter *p)
{
    if (!p) {
        qWarning() << Q_FUNC_INFO << "The input parameter is null. Nothing is done.";
        return false;
    }

    m_parameter = dynamic_cast<dtk::d_bool *>(p);

    if (!m_parameter) {
        qWarning() << Q_FUNC_INFO
                   << "The type of the parameter is not compatible with the "
                      "widget dtkWidgetsParameterBoolPushButton.";
        return false;
    }

    d->push_button->setToolTip(m_parameter->documentation());

    d->running = m_parameter->value();

    if (d->running) {
        qWarning() << Q_FUNC_INFO
                   << "It is not advised to set d_bool parameter to on before "
                      "connecting";
    }

    auto setAppearance = [this](bool state) {
        if (state) {
            this->d->push_button->setStyleSheet("background-color: red");
            this->d->push_button->setText("Running...");
        } else {
            this->d->push_button->setStyleSheet("");
            this->d->push_button->setText("Run");
        }
    };

    setAppearance(d->running);

    m_parameter->connect([=](QVariant v) {
        d->running = v.value<dtk::d_bool>().value();
        d->push_button->blockSignals(true);
        setAppearance(d->running);
        d->push_button->setChecked(d->running);
        d->push_button->blockSignals(false);
    });

    QObject::connect(d->push_button, &QPushButton::clicked, [=]() {
        if (d->running) {
            d->running = false;
        } else {
            d->running = true;
        }
        m_parameter->shareValue(QVariant::fromValue(d->running));
    });

    return true;
}

//
// dtkWidgetsParameterBoolPushButtonWidget.cpp ends here
