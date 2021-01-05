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

#include "dtkVisualizationWidgetsFieldScalar.h"

#include <QtWidgets>

#include <vtkActor.h>
#include <vtkVolume.h>

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsFieldScalarPrivate
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationWidgetsFieldScalarPrivate
{
public:
    vtkActor *actor = nullptr;
    vtkVolume *volume = nullptr;

public:
    QComboBox *box;
};

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationWidgetsFieldScalar
// ///////////////////////////////////////////////////////////////////

dtkVisualizationWidgetsFieldScalar::dtkVisualizationWidgetsFieldScalar(QWidget *parent)
    : QWidget(parent), d(new dtkVisualizationWidgetsFieldScalarPrivate)
{
    d->box = new QComboBox(this);

    QFormLayout *layout = new QFormLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(d->box);
}

dtkVisualizationWidgetsFieldScalar::~dtkVisualizationWidgetsFieldScalar(void)
{
    delete d;
}

void dtkVisualizationWidgetsFieldScalar::setActor(vtkActor *actor)
{
    d->actor = actor;

    this->update();
}

void dtkVisualizationWidgetsFieldScalar::setVolume(vtkVolume *volume)
{
    d->volume = volume;

    this->update();
}

void dtkVisualizationWidgetsFieldScalar::update(void)
{
    qDebug() << Q_FUNC_INFO;
}

//
// dtkVisualizationWidgetsFieldScalar.cpp ends here
