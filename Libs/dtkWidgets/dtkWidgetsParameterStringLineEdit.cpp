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

#include "dtkWidgetsParameterStringLineEdit.h"

#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterStringLineEditPrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsParameterStringLineEditPrivate
{
public:
    QLineEdit *line_edit = nullptr;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterStringLineEdit implementation
// ///////////////////////////////////////////////////////////////////

dtkWidgetsParameterStringLineEdit::dtkWidgetsParameterStringLineEdit(QWidget *parent)
    : dtkWidgetsParameterBase<dtk::d_string>(parent),
      d(new dtkWidgetsParameterStringLineEditPrivate)
{
    d->line_edit = new QLineEdit;

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(d->line_edit);

    this->setLayout(layout);
}

dtkWidgetsParameterStringLineEdit::~dtkWidgetsParameterStringLineEdit(void)
{
    delete d;
}

bool dtkWidgetsParameterStringLineEdit::connect(dtkCoreParameter *p)
{
    if (!p) {
        qWarning() << Q_FUNC_INFO << "The input parameter is null. Nothing is done.";
        return false;
    }

    m_parameter = dynamic_cast<dtk::d_string *>(p);

    if (!m_parameter) {
        qWarning() << Q_FUNC_INFO
                   << "The type of the parameter is not compatible with the "
                      "widget dtkWidgetsParameterStringLineEdit.";
        return false;
    }

    d->line_edit->setText(m_parameter->value());
    d->line_edit->setToolTip(m_parameter->documentation());

    m_parameter->connect([=](QVariant v) {
        QString value = v.value<dtk::d_string>().value();
        d->line_edit->blockSignals(true);
        d->line_edit->setText(value);
        d->line_edit->blockSignals(false);
    });

    QObject::connect(d->line_edit, &QLineEdit::editingFinished, [=]() {
        QString v = d->line_edit->text();
        m_parameter->shareValue(QVariant::fromValue(v));
    });

    return true;
}

//
// dtkWidgetsParameterStringLineEdit.cpp ends here
