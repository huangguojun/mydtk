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

class DTKVISUALIZATION_EXPORT dtkVisualizationDecoratorSlices
    : public dtkVisualizationDecoratorWithClut
{
    Q_OBJECT

public:
    dtkVisualizationDecoratorSlices(void);
    ~dtkVisualizationDecoratorSlices(void);

public:
    void setData(const QVariant &) override;
    void setCanvas(dtkVisualizationCanvas *) override;
    void unsetCanvas(void) override;
    void touch(void) override;

public:
    bool setCurrentFieldName(const QString &) override;

public:
    void setColorMap(const QMap<double, QColor> &) override;

public:
    void setVisibility(bool) override;
    void restoreSettings(void) override;

protected:
    class dtkVisualizationDecoratorSlicesPrivate *d = nullptr;
};

//
// dtkVisualizationDecoratorSlicess.h ends here
