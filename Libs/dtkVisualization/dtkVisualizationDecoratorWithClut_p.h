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

#include <vtkSmartPointer.h>

#include <array>

class dtkVisualizationView2D;
class dtkVisualizationWidgetsColorMapEditor;

class QComboBox;
class QCheckBox;
class QLineEdit;

class vtkColorTransferFunction;
class vtkDataSet;
class vtkDataSetAttributes;
class vtkPiecewiseFunction;
class vtkPiecewiseFunction;
class vtkScalarBarActor;

class DTKVISUALIZATION_EXPORT dtkVisualizationDecoratorWithClutPrivate
{
public:
    enum Support {
        Unknown = 0,
        Point = 1,
        Cell = 2
    };

    enum Kind {
        Scalar = 0x001,
        Vector = 0x003,
        Tensor = 0x009
    };

public:
     dtkVisualizationDecoratorWithClutPrivate(void);
    ~dtkVisualizationDecoratorWithClutPrivate(void) = default;

public:
    vtkDataSet *dataset = nullptr;
    dtkVisualizationView2D *view = nullptr;

public:
    QList<QWidget *> inspectors;

    QComboBox *fields_box  = nullptr;
    QCheckBox *fixed_range = nullptr;
    QLineEdit *min_range   = nullptr;
    QLineEdit *max_range   = nullptr;
    QCheckBox *show_scalar_bar = nullptr;

public:
    dtkVisualizationWidgetsColorMapEditor *colormap_editor = nullptr;

public:
    QStringList eligible_field_names;
    QString current_field_name;
    QString default_field_name;
    QString default_color_map;
    bool default_visibility;
    bool default_show_scalar_bar;

public:
    vtkSmartPointer<vtkColorTransferFunction> color_function;
    vtkSmartPointer<vtkPiecewiseFunction>     opacity;
    vtkSmartPointer<vtkTransform>             transform;

    vtkSmartPointer<vtkScalarBarActor> scalar_bar;

    QHash<QString, QString> color_transfer_functions; // <field_name, colormap_name>
    QHash<QString, vtkSmartPointer<vtkPiecewiseFunction>> opacity_functions;
    QHash<QString, std::array<double,2>> ranges;
    QHash<QString, Support> supports;
    QHash<QString, Kind> kinds;

public:
    void clear(void);
    void enableScalarBar(void);
    void disableScalarBar(void);
    void retrieveScalarPoints(vtkDataSet *);
    void retrieveScalarCells(vtkDataSet *);
    void retrieveVectorPoints(vtkDataSet *);
    void retrieveVectorCells(vtkDataSet *);
    void sortEligibleFields(void);

private:
    void retrieveScalarData(vtkDataSetAttributes *, Support support);
    void retrieveVectorData(vtkDataSetAttributes *, Support support);
};
