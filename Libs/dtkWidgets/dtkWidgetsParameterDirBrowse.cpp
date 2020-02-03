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

#include "dtkWidgetsParameterDirBrowse.h"

#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterDirBrowsePrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsParameterDirBrowsePrivate
{
public:
    QPushButton *push_button = nullptr;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterDirBrowse implementation
// ///////////////////////////////////////////////////////////////////

dtkWidgetsParameterDirBrowse::dtkWidgetsParameterDirBrowse(QWidget* parent) : dtkWidgetsParameterBase<dtk::d_path>(parent), d(new dtkWidgetsParameterDirBrowsePrivate)
{
    d->push_button = new QPushButton;

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(d->push_button);

    this->setLayout(layout);
}

dtkWidgetsParameterDirBrowse::~dtkWidgetsParameterDirBrowse(void)
{
    delete d;
}

bool dtkWidgetsParameterDirBrowse::connect(dtkCoreParameter *p)
{
    if (!p) {
        qWarning() << Q_FUNC_INFO << "The input parameter is null. Nothing is done.";
        return false;
    }

    m_parameter = dynamic_cast<dtk::d_path *>(p);

    if(!m_parameter) {
        qWarning() << Q_FUNC_INFO << "The type of the parameter is not compatible with the widget dtkWidgetsParameterDirBrowse.";
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
        QString v = QFileDialog::getExistingDirectory(this, "Open Directory", m_parameter->dirName());
        m_parameter->shareValue(QVariant::fromValue(v));
    });

    return true;
}

//
// dtkWidgetsParameterDirBrowse.cpp ends here
