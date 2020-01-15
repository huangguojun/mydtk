// Version: $Id: 0e2e50291261ff5b3901993a758480c0eff9b2c7 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkWidgetsController.h"
#include "dtkWidgetsWidget.h"

#include <QtCore>

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsControllerPrivate
// ///////////////////////////////////////////////////////////////////

class dtkWidgetsControllerPrivate
{
public:
    QList<dtkWidgetsWidget *> views;
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsController
// ///////////////////////////////////////////////////////////////////

dtkWidgetsController *dtkWidgetsController::instance(void)
{
    if(!s_instance)
        s_instance = new dtkWidgetsController;

    return s_instance;
}

void dtkWidgetsController::insert(dtkWidgetsWidget *view)
{
    QString name = view->objectName();

    if (name.isEmpty()) {
        name = QString("View %1").arg(d->views.count()+1);
        view->setObjectName(name);
    }

    d->views.append(view);

    emit inserted(view);
}

dtkWidgetsWidget *dtkWidgetsController::view(const QString& name)
{
    for(auto view : d->views) {
        if(view->objectName() == name) {
            return view;
        }
    }

    if(name != "") {
        qWarning() << Q_FUNC_INFO << "No view with name:" << name << "could be retrived. Nullptr is returned.";
    }

    return nullptr;
}

QStringList dtkWidgetsController::viewNames(void)
{
    QStringList view_names;
    for(auto view : d->views) {
        view_names.append(view->objectName());
    }

    view_names.append("");
    view_names.sort();

    return view_names;
}

void dtkWidgetsController::clear(void)
{
    qDeleteAll(d->views);
}

dtkWidgetsController::dtkWidgetsController(void) : QObject(), d(new dtkWidgetsControllerPrivate)
{

}

dtkWidgetsController::~dtkWidgetsController(void)
{
    delete d;
}

dtkWidgetsController *dtkWidgetsController::s_instance = nullptr;

//
// dtkWidgetsController.cpp ends here
