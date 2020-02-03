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

#include "dtkWidgetsParameterFileBrowse.h"

#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterFileBrowsePrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsParameterFileBrowsePrivate
{
public:
    QPushButton *push_button = nullptr;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterFileBrowse implementation
// ///////////////////////////////////////////////////////////////////

dtkWidgetsParameterFileBrowse::dtkWidgetsParameterFileBrowse(QWidget* parent) : dtkWidgetsParameterBase<dtk::d_path>(parent), d(new dtkWidgetsParameterFileBrowsePrivate)
{
    d->push_button = new QPushButton;

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(d->push_button);

    this->setLayout(layout);
}

dtkWidgetsParameterFileBrowse::~dtkWidgetsParameterFileBrowse(void)
{
    delete d;
}

bool dtkWidgetsParameterFileBrowse::connect(dtkCoreParameter *p)
{
    if (!p) {
        qWarning() << Q_FUNC_INFO << "The input parameter is null. Nothing is done.";
        return false;
    }

    m_parameter = dynamic_cast<dtk::d_path *>(p);

    if(!m_parameter) {
        qWarning() << Q_FUNC_INFO << "The type of the parameter is not compatible with the widget dtkWidgetsParameterFileBrowse.";
        return false;
    }

    d->push_button->setText("Browse");

    m_parameter->connect([=] (QVariant v) {
        d->push_button->blockSignals(true);
        QString value = v.value<dtk::d_path>().baseName();
        if (value.isEmpty()) {
            value = "Browse";
        }
        d->push_button->setText(value);
        d->push_button->blockSignals(false);
    });

    QObject::connect(d->push_button, &QPushButton::clicked, [=] ()
    {
        QString v = QFileDialog::getOpenFileName(this, "Open file", m_parameter->dirName(), m_parameter->filters().join(";;"));
        m_parameter->shareValue(QVariant::fromValue(v));
    });

    return true;
}

//
// dtkWidgetsParameterFileBrowse.cpp ends here
