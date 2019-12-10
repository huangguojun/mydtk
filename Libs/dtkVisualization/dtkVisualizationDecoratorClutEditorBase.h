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

#include <QtCore>

#include "dtkVisualizationDecorator.h"

template <typename T> class vtkSmartPointer;

class vtkColorTransferFunction;
class vtkPiecewiseFunction;

class DTKVISUALIZATION_EXPORT dtkVisualizationDecoratorClutEditorBase : public dtkVisualizationDecorator
{
    Q_OBJECT

public:
     dtkVisualizationDecoratorClutEditorBase(void) : dtkVisualizationDecorator() {};
    ~dtkVisualizationDecoratorClutEditorBase(void) = default;

public:
    virtual QStringList                               eligibleFieldNames(void) const = 0;
    virtual QString                                   currentFieldName(void) const = 0;
    virtual QPair<double, double>                     currentRange(void) const = 0;
    virtual vtkSmartPointer<vtkColorTransferFunction> currentColorTransferFunction(void) const = 0;
    virtual vtkSmartPointer<vtkPiecewiseFunction>     currentOpacityTransferFunction(void) const = 0;

public:
    virtual bool setCurrentFieldName(const QString&) = 0;

signals:
    void updated(void);
};
