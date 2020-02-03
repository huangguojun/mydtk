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

#include "dtkWidgetsParameterStringListComboBox.h"

#include <QtWidgets>



// ///////////////////////////////////////////////////////////////////
// dtkWidgetsBaseComboBox declaration
// ///////////////////////////////////////////////////////////////////
// We define a BaseComboBox, which is just a QComboBox with an eventFilter to reject all scrolling events. That's it!

class dtkWidgetsBaseComboBox : public QComboBox
{

    public: 
        dtkWidgetsBaseComboBox(QWidget* parent = 0);
        ~dtkWidgetsBaseComboBox(void);

    protected:
        bool eventFilter(QObject *o, QEvent *e) override;
};

dtkWidgetsBaseComboBox::dtkWidgetsBaseComboBox(QWidget* parent)
{
    this->installEventFilter(this); // filter scroll events (see below)
}

dtkWidgetsBaseComboBox::~dtkWidgetsBaseComboBox()
{
}

// The event filter kicks out every scroll event. We don't want these to be scrolled and when they're clicked they're not scrollable anyway.
bool dtkWidgetsBaseComboBox::eventFilter(QObject *o, QEvent *e)
{
    if(e->type() == QEvent::Wheel &&
       qobject_cast<QComboBox*>(o))
    {
        e->ignore();
        return true;
    }
    return QWidget::eventFilter(o, e);
}

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterStringListComboBoxPrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsParameterStringListComboBoxPrivate
{
public:
    dtkWidgetsBaseComboBox  *combo_box = nullptr;
};



// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterStringListComboBox implementation
// ///////////////////////////////////////////////////////////////////

dtkWidgetsParameterStringListComboBox::dtkWidgetsParameterStringListComboBox(QWidget* parent) : dtkWidgetsParameterBase<dtk::d_inliststring>(parent), d(new dtkWidgetsParameterStringListComboBoxPrivate)
{
    d->combo_box = new dtkWidgetsBaseComboBox;

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(d->combo_box);

    this->setLayout(layout);
}

dtkWidgetsParameterStringListComboBox::~dtkWidgetsParameterStringListComboBox(void)
{
    delete d;
}

bool dtkWidgetsParameterStringListComboBox::connect(dtkCoreParameter *p)
{
    if (!p) {
        qWarning() << Q_FUNC_INFO << "The input parameter is null. Nothing is done.";
        return false;
    }

    m_parameter = dynamic_cast<dtk::d_inliststring *>(p);

    if(!m_parameter) {
        qWarning() << Q_FUNC_INFO << "The type of the parameter is not compatible with the widget dtkWidgetsParameterStringListComboBox.";
        return false;
    }

    d->combo_box->addItems(m_parameter->values());
    d->combo_box->setToolTip(m_parameter->documentation());

    int index = d->combo_box->findText(m_parameter->value());
    if ( index != -1 ) {
        d->combo_box->setCurrentIndex(index);
    }

    m_parameter->connect([=] (QVariant v) {
        if(d->combo_box->count() != m_parameter->values().count()) {
            d->combo_box->clear();
            d->combo_box->addItems(m_parameter->values());
        }

        QString value = v.value<dtk::d_inliststring>().value();
        int index = d->combo_box->findText(value);
        if ( index != -1 ) {
            d->combo_box->blockSignals(true);
            d->combo_box->setCurrentIndex(index);
            d->combo_box->blockSignals(false);
        }
    });

    QObject::connect(d->combo_box, &QComboBox::currentTextChanged, [=] (const QString& v)
    {
        m_parameter->shareValue(QVariant::fromValue(v));
    });

    return true;
}

//
// dtkWidgetsParameterStringListComboBox.cpp ends here
