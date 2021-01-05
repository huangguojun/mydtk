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

class DTKVISUALIZATION_EXPORT dtkVisualizationDecoratorVolume
    : public dtkVisualizationDecoratorWithClut
{
    Q_OBJECT

public:
    dtkVisualizationDecoratorVolume(void);
    ~dtkVisualizationDecoratorVolume(void);

public:
    void setData(const QVariant &) override;
    void setCanvas(dtkVisualizationCanvas *) override;
    void unsetCanvas(void) override;

public:
    bool setCurrentFieldName(const QString &) override;

public:
    void setColorMap(const QMap<double, QColor> &) override;
    void setAlphaMap(const QMap<double, double> &);

public:
    void setVisibility(bool) override;
    void restoreSettings(void) override;

protected:
    class dtkVisualizationDecoratorVolumePrivate *d = nullptr;
};

//
// dtkVisualizationDecoratorVolumes.h ends here
