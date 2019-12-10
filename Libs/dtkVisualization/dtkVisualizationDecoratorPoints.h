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

#include "dtkVisualizationDecorator.h"

class DTKVISUALIZATION_EXPORT dtkVisualizationDecoratorPoints : public dtkVisualizationDecorator
{
    Q_OBJECT

public:
     dtkVisualizationDecoratorPoints(void);
    ~dtkVisualizationDecoratorPoints(void);

public:
    bool isDecorating(void) override;

    void setData(const QVariant&)            override;
    void setCanvas(dtkVisualizationCanvas *) override;
    void unsetCanvas(void)                   override;

    QVariant data(void) const override;
    dtkVisualizationCanvas *canvas(void) const override;

    QList<QWidget *> inspectors(void) const override;

    void touch(void) override;

public:
    void setVisibility(bool) override;

public:
    virtual void setColor(const QColor&);
    virtual void setOpacity(const double&);
    virtual void setSize(const double&);

signals:
    void updated(void);

protected:
    class dtkVisualizationDecoratorPointsPrivate *d;
};

//
// dtkVisualizationDecoratorPoints.h ends here
