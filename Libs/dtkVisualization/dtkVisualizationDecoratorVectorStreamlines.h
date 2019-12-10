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

class DTKVISUALIZATION_EXPORT dtkVisualizationDecoratorVectorStreamlines : public dtkVisualizationDecoratorWithClut
{
    Q_OBJECT

public:
     dtkVisualizationDecoratorVectorStreamlines(void);
    ~dtkVisualizationDecoratorVectorStreamlines(void);

public:
    void setData(const QVariant&) override;
    void setCanvas(dtkVisualizationCanvas *) override;
    void unsetCanvas(void) override;

protected:
    void restoreSettings(void) override;
    bool setCurrentFieldName(const QString&) override;
    void setColorMap(const QMap<double, QColor>&) override;

public:
    void setVisibility(bool visible) override;

protected:
    bool isCurrentFieldUniform(void);

protected:
    class dtkVisualizationDecoratorVectorStreamlinesPrivate *d = nullptr;
};

//
// dtkVisualizationDecoratorVectorStreamlines.h ends here
