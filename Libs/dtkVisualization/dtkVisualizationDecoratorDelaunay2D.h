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

class DTKVISUALIZATION_EXPORT dtkVisualizationDecoratorDelaunay2D : public dtkVisualizationDecorator
{
    Q_OBJECT

public:
    dtkVisualizationDecoratorDelaunay2D(void);
    ~dtkVisualizationDecoratorDelaunay2D(void);

public:
    bool isDecorating(void) override;

    void setData(const QVariant &) override;
    void setCanvas(dtkVisualizationCanvas *) override;
    void unsetCanvas(void) override;

    QVariant data(void) const override;
    dtkVisualizationCanvas *canvas(void) const override;

    QList<QWidget *> inspectors(void) const override;

public:
    void setVisibility(bool) override;

public:
    virtual void setColor(const QColor &);
    virtual void setOpacity(const double &);

public:
    void touch(void) override;

signals:
    void updated(void);

protected:
    void restoreSettings(void) override;

protected:
    class dtkVisualizationDecoratorDelaunay2DPrivate *d;
};

//
// dtkVisualizationDecoratorDelaunay2D.h ends here
