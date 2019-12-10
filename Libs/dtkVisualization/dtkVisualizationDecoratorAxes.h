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

class DTKVISUALIZATION_EXPORT dtkVisualizationDecoratorAxes : public dtkVisualizationDecorator
{
    Q_OBJECT

public:
     dtkVisualizationDecoratorAxes(void);
    ~dtkVisualizationDecoratorAxes(void);

public:
    bool isDecorating(void) override;

    void setData(const QVariant&) override;
    void setCanvas(dtkVisualizationCanvas *) override;
    void unsetCanvas(void) override;

    QVariant data(void) const override;
    dtkVisualizationCanvas *canvas(void) const override;

    QList<QWidget *> inspectors(void) const override;

public:
    void setVisibility(bool) override;
    void restoreSettings(void) override;

public:
    void touch(void) override;

protected:
    class dtkVisualizationDecoratorAxesPrivate *d = nullptr;
};

//
// dtkVisualizationDecoratorAxess.h ends here
