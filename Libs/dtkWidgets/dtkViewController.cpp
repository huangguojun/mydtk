// Version: $Id: 336d64ed5535b4839bd1962dde6a8f8b689fa059 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkViewController.h"
#include "dtkViewWidget.h"

class dtkViewControllerPrivate
{
public:
    QHash<QString, dtkViewWidget *> views;
};

dtkViewController *dtkViewController::instance(void)
{
    if (!s_instance)
        s_instance = new dtkViewController;

    return s_instance;
}

void dtkViewController::insert(dtkViewWidget *view)
{
    QString name = view->objectName();

    if (name.isEmpty())
        name = QString("View %1").arg(d->views.count() + 1);

    d->views.insert(name, view);

    emit inserted(view, name);
}

dtkViewWidget *dtkViewController::view(const QString &name)
{
    return d->views.value(name, nullptr);
}

QStringList dtkViewController::viewNames(void)
{
    return d->views.keys();
}

dtkViewController::dtkViewController(void) : QObject()
{
    d = new dtkViewControllerPrivate;
}

dtkViewController::~dtkViewController(void)
{
    delete d;
}

dtkViewController *dtkViewController::s_instance = NULL;

//
// dtkViewController.cpp ends here
