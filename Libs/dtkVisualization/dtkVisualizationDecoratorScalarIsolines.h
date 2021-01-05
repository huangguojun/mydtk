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

class DTKVISUALIZATION_EXPORT dtkVisualizationDecoratorScalarIsolines
    : public dtkVisualizationDecoratorWithClut
{
    Q_OBJECT
public:
    dtkVisualizationDecoratorScalarIsolines(void);
    ~dtkVisualizationDecoratorScalarIsolines(void);

public:
    void touch(void) override;

public:
    void setData(const QVariant &) override;
    void setCanvas(dtkVisualizationCanvas *) override;
    void unsetCanvas(void) override;

protected:
    void restoreSettings(void) override;
    void setCurrentRange(double, double) override;
    bool setCurrentFieldName(const QString &) override;
    void setColorMap(const QMap<double, QColor> &) override;

public:
    void setVisibility(bool) override;

public:
    void setCurrentIsolinesCount(std::size_t);

protected:
    class dtkVisualizationDecoratorScalarIsolinesPrivate *d = nullptr;
};

//
// dtkVisualizationDecoratorScalarIsolines.h ends here
