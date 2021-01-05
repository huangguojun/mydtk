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

#include "dtkVisualizationDecorator.h"

#include "dtkVisualization.h"
#include "dtkVisualizationCanvas.h"

#include <vtkTransform.h>

// ///////////////////////////////////////////////////////////////////
// Register to dtkVisualization layer
// ///////////////////////////////////////////////////////////////////

namespace dtk {
namespace visualization {
DTK_DEFINE_CONCEPT(dtkVisualizationDecorator, decorator, dtk::visualization);
}
} // namespace dtk

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecorator implementation
// ///////////////////////////////////////////////////////////////////

void dtkVisualizationDecorator::draw(void)
{
    if (this->canvas()) {
        this->canvas()->draw();
    }
}

void dtkVisualizationDecorator::saveSettings(const QString &name, const QVariant &value)
{
    QSettings settings;
    settings.beginGroup("canvas");
    settings.setValue(this->objectName() + "_" + name, value);
    settings.endGroup();
}

void dtkVisualizationDecorator::show(void)
{
    this->setVisibility(true);
}

void dtkVisualizationDecorator::hide(void)
{
    this->setVisibility(false);
}

void dtkVisualizationDecorator::setObjectName(const QString &name)
{
    QObject::setObjectName(name);
}

void dtkVisualizationDecorator::setTransform(vtkTransform *transform)
{
    qWarning() << Q_FUNC_INFO << "nothing is done";
}

//
// dtkVisualizationDecorator.cpp ends here
