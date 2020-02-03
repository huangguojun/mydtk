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

#include "dtkWidgetsParameterFileSave.h"

#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterFileSavePrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsParameterFileSavePrivate
{
public:
    QPushButton *push_button = nullptr;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterFileSave implementation
// ///////////////////////////////////////////////////////////////////

dtkWidgetsParameterFileSave::dtkWidgetsParameterFileSave(QWidget* parent) : dtkWidgetsParameterBase<dtk::d_path>(parent), d(new dtkWidgetsParameterFileSavePrivate)
{
    d->push_button = new QPushButton;

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(d->push_button);

    this->setLayout(layout);
}

dtkWidgetsParameterFileSave::~dtkWidgetsParameterFileSave(void)
{
    delete d;
}

bool dtkWidgetsParameterFileSave::connect(dtkCoreParameter *p)
{
    if (!p) {
        qWarning() << Q_FUNC_INFO << "The input parameter is null. Nothing is done.";
        return false;
    }

    m_parameter = dynamic_cast<dtk::d_path *>(p);

    if(!m_parameter) {
        qWarning() << Q_FUNC_INFO << "The type of the parameter is not compatible with the widget dtkWidgetsParameterFileSave.";
        return false;
    }

    d->push_button->setText("Save");

    m_parameter->connect([=] (QVariant v) {
        d->push_button->blockSignals(true);
        QString value = v.value<dtk::d_path>().baseName();
        if (value.isEmpty()) {
            value = "Save";
        }
        d->push_button->setText(value);
        d->push_button->blockSignals(false);
    });

    QObject::connect(d->push_button, &QPushButton::clicked, [=] ()
    {
        QString v = QFileDialog::getSaveFileName(this, "Save file", m_parameter->dirName(), m_parameter->filters().join(";;"));
        m_parameter->shareValue(QVariant::fromValue(v));
    });

    return true;
}

//
// dtkWidgetsParameterFileSave.cpp ends here
