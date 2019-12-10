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

#include <QtCore>

class vtkDataSet;
class QColor;
class dtkVisualizationCanvas;

class DTKVISUALIZATION_EXPORT dtkVisualizationDecoratorWithClut : public dtkVisualizationDecorator
{
    Q_OBJECT

public:
    enum Support {
        Unknown = 0,
        Point = 1,
        Cell = 2
    };

public:
     dtkVisualizationDecoratorWithClut(void);
    ~dtkVisualizationDecoratorWithClut(void);

public:
    void touch(void) override;

public:
    bool isDecorating(void) override;

    QVariant data(void) const override;
    dtkVisualizationCanvas *canvas(void) const override;

    QList<QWidget *> inspectors(void) const override;

public:
    virtual void setVisibility(bool visible) override;

 public:
    QString fieldName(void) const;

protected:
    virtual void restoreSettings(void) override;
    virtual void setCurrentRange(double, double);
    virtual bool setCurrentFieldName(const QString&);
    virtual void setColorMap(const QMap<double, QColor>&);

protected:
    class dtkVisualizationDecoratorWithClutPrivate *d;
          dtkVisualizationDecoratorWithClutPrivate *d_func(void);
    const dtkVisualizationDecoratorWithClutPrivate *d_func(void) const;
};

//
// dtkVisualizationDecoratorWithClut.h ends here
