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

#include "dtkWidgetsMenuItem.h"

#include "dtkWidgets.h"
#include "dtkWidgetsParameter.h"
#include "dtkWidgetsParameterFactory.h"

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuItemPrivate
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsMenuItemPrivate
{
public:
    QString title;

public:
    int icon_id;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuItem implementation
// ///////////////////////////////////////////////////////////////////

dtkWidgetsMenuItem::dtkWidgetsMenuItem(QObject *parent)
    : QAction(parent), d(new dtkWidgetsMenuItemPrivate)
{
}

dtkWidgetsMenuItem::dtkWidgetsMenuItem(const QString &title, QObject *parent)
    : QAction(parent), d(new dtkWidgetsMenuItemPrivate)
{
    d->title = title;
}

dtkWidgetsMenuItem::dtkWidgetsMenuItem(int icon_id, const QString &title, QObject *parent)
    : QAction(parent), d(new dtkWidgetsMenuItemPrivate)
{
    d->title = title;
    d->icon_id = icon_id;
}

dtkWidgetsMenuItem::~dtkWidgetsMenuItem(void)
{
    delete d;
}

const QString &dtkWidgetsMenuItem::title(void) const
{
    return d->title;
}

int dtkWidgetsMenuItem::icon(void) const
{
    return d->icon_id;
}

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuItemParameterPrivate
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsMenuItemParameterPrivate
{
public:
    dtkCoreParameter *parameter = nullptr;
    dtkWidgetsParameter *widget = nullptr;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuItem implementation
// ///////////////////////////////////////////////////////////////////

dtkWidgetsMenuItemParameter::dtkWidgetsMenuItemParameter(const QString &title,
                                                         dtkCoreParameter *parameter,
                                                         const QString &representation)
    : dtkWidgetsMenuItem(0, title, 0), d(new dtkWidgetsMenuItemParameterPrivate)
{
    d->parameter = parameter;
    d->widget = ::dtk::widgets::parameters::pluginFactory().create(parameter, representation);
}

dtkWidgetsMenuItemParameter::~dtkWidgetsMenuItemParameter(void)
{
    if (d->widget)
        delete d->widget;

    delete d;
}

dtkCoreParameter *dtkWidgetsMenuItemParameter::parameter(void)
{
    return d->parameter;
}

dtkWidgetsParameter *dtkWidgetsMenuItemParameter::widget(void)
{
    return d->widget;
}

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsMenuItemCustom implementation
// ///////////////////////////////////////////////////////////////////

dtkWidgetsMenuItemCustom::dtkWidgetsMenuItemCustom(const QString &title)
    : dtkWidgetsMenuItem(0, title, 0)
{
}

//
// dtkWidgetsMenuItem.cpp ends here
