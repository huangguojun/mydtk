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

#include <dtkVisualizationExport.h>

#include <QtCore>

#include "dtkVisualizationDecorator.h"

class DTKVISUALIZATION_EXPORT dtkVisualizationDecoratorSnapshot : public dtkVisualizationDecorator
{
    Q_OBJECT

public:
    dtkVisualizationDecoratorSnapshot(void);
    ~dtkVisualizationDecoratorSnapshot(void);

public:
    bool isDecorating(void) override;
    QList<QWidget *> inspectors(void) const override;
    QVariant data(void) const override;
    dtkVisualizationCanvas *canvas(void) const override;

    void touch(void) override;

public:
    void setData(const QVariant &) override;
    void setCanvas(dtkVisualizationCanvas *) override;
    void unsetCanvas(void) override;

public:
    void setVisibility(bool) override;

signals:
    void updated(void);

protected:
    class dtkVisualizationDecoratorSnapshotPrivate *d;
};

//
// dtkVisualizationDecoratorSnapshot.h ends here
