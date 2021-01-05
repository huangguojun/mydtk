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

#include "dtkVisualizationDecoratorClutEditorBase.h"

class DTKVISUALIZATION_EXPORT dtkVisualizationDecoratorClutEditorSimple
    : public dtkVisualizationDecoratorClutEditorBase
{
    Q_OBJECT

public:
    dtkVisualizationDecoratorClutEditorSimple(void);
    ~dtkVisualizationDecoratorClutEditorSimple(void);

public:
    bool isDecorating(void) override;

    void setData(const QVariant &) override;
    void setCanvas(dtkVisualizationCanvas *) override;
    void unsetCanvas(void) override;

    QVariant data(void) const override;
    dtkVisualizationCanvas *canvas(void) const override;

    QList<QWidget *> inspectors(void) const override;

public:
    void show(void);
    void hide(void);

public:
    QStringList eligibleFieldNames(void) const override;
    QString currentFieldName(void) const override;
    QPair<double, double> currentRange(void) const override;
    vtkSmartPointer<vtkColorTransferFunction> currentColorTransferFunction(void) const override;
    vtkSmartPointer<vtkPiecewiseFunction> currentOpacityTransferFunction(void) const override;

public:
    bool setCurrentFieldName(const QString &) override;

public:
    void setCurrentColorTransferFunction(vtkSmartPointer<vtkColorTransferFunction>);
    void setCurrentOpacityTransferFunction(vtkSmartPointer<vtkPiecewiseFunction>);
    void setCurrentRange(double, double);

public:
    void touch(void) override;

protected:
    class dtkVisualizationDecoratorClutEditorSimplePrivate *d;
};

//
// dtkVisualizationDecoratorClutEditorSimple.h ends here
