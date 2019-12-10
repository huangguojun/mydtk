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

#pragma once

#include <dtkVisualizationExport>

#include "dtkVisualizationDecoratorWithClut.h"

class DTKVISUALIZATION_EXPORT dtkVisualizationDecoratorScalarColorMap : public dtkVisualizationDecoratorWithClut
{
    Q_OBJECT

public:
     dtkVisualizationDecoratorScalarColorMap(void);
    ~dtkVisualizationDecoratorScalarColorMap(void);

public:
    void setData(const QVariant&) override;
    void setCanvas(dtkVisualizationCanvas *) override;
    void setTransform(vtkTransform *) override;
    void unsetCanvas(void) override;

public:
    void setVisibility(bool) override;


// /////////////////////////////////////////////////////////////////////////////

protected:
    bool setCurrentFieldName(const QString&) override;
    void setColorMap(const QMap<double, QColor>&) override;
    void restoreSettings(void) override;

protected:
    class dtkVisualizationDecoratorScalarColorMapPrivate *d;
};

//
// dtkVisualizationDecoratorScalarColorMap.h ends here
