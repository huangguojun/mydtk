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

#include "dtkVisualizationDecoratorScalarIsolines.h"

#include "dtkVisualizationDecoratorWithClut_p.h"
#include "dtkVisualizationCanvas.h"
#include "dtkVisualizationMetaType.h"
#include "dtkVisualizationView2D.h"

#include <dtkLog>

#include <QtGui>
#include <QtWidgets>

#include <vtkActor.h>
#include <vtkCellDataToPointData.h>
#include <vtkCellData.h>
#include <vtkColorTransferFunction.h>
#include <vtkContourFilter.h>
#include <vtkDataSet.h>
#include <vtkLookupTable.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorScalarIsolinesPrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationDecoratorScalarIsolinesPrivate
{
public:
    vtkSmartPointer<vtkActor> actor;
    vtkSmartPointer<vtkPolyDataMapper> mapper;

public:
    vtkSmartPointer<vtkContourFilter> isolines;
    vtkSmartPointer<vtkCellDataToPointData> c2p_filter;

public:
    int default_isolines_count = 10;

public:
    QCheckBox *show_actor_cb = nullptr;
    QSpinBox *isolines_counts_sb = nullptr;

public:
    QHash<QString, std::size_t> isolines_counts;
};

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorScalarIsolines implementation
// ///////////////////////////////////////////////////////////////////

dtkVisualizationDecoratorScalarIsolines::dtkVisualizationDecoratorScalarIsolines(void): dtkVisualizationDecoratorWithClut(), d(new dtkVisualizationDecoratorScalarIsolinesPrivate())
{
    d->isolines = vtkSmartPointer<vtkContourFilter>::New();

    d->c2p_filter = vtkSmartPointer<vtkCellDataToPointData>::New();

    d->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    d->mapper->SetInputConnection(d->isolines->GetOutputPort());
    d->mapper->SetColorModeToMapScalars();
    d->mapper->SetScalarVisibility(true);

    d->actor = vtkSmartPointer<vtkActor>::New();
    d->actor->SetMapper(d->mapper);

    //////////
    // Inspectors creation and setup

    d->show_actor_cb = new QCheckBox;
    d->isolines_counts_sb = new QSpinBox;

    d->isolines_counts_sb->setValue(10);
    d->isolines_counts_sb->setMaximum(10000);

    //////////
    // Inspectors connections

    connect(d->show_actor_cb, &QCheckBox::stateChanged, [=] (int state) {
            this->saveSettings("visibility",state == Qt::Checked);
            this->setVisibility(state == Qt::Checked);
            this->draw();
        });

    connect(d->isolines_counts_sb, QOverload<int>::of(&QSpinBox::valueChanged), [=] (int value) {
            this->saveSettings("isolines_count", value);
            this->setCurrentIsolinesCount(value);
            this->draw();
        });

    this->setObjectName("Scalar Isolines");
    d->show_actor_cb->setObjectName("Display");
    d->isolines_counts_sb->setObjectName("Count");

    d_func()->inspectors << d->show_actor_cb << d->isolines_counts_sb;
}

dtkVisualizationDecoratorScalarIsolines::~dtkVisualizationDecoratorScalarIsolines(void)
{
    dtkVisualizationDecoratorScalarIsolines::unsetCanvas();

    delete d;
    d = nullptr;
}

void dtkVisualizationDecoratorScalarIsolines::restoreSettings(void)
{
    QString name = this->objectName();
    if (name.isEmpty())
        return;
    dtkVisualizationDecoratorWithClut::restoreSettings();

    d->actor->SetVisibility(d_func()->default_visibility);
    d->show_actor_cb->blockSignals(true);
    d->show_actor_cb->setCheckState(d_func()->default_visibility ? Qt::Checked : Qt::Unchecked);
    d->show_actor_cb->blockSignals(false);

    QSettings settings;
    settings.beginGroup("canvas");
    d->default_isolines_count = settings.value(name+"_isolines_count", 10).toInt();
    settings.endGroup();

    this->setCurrentIsolinesCount(d->default_isolines_count);
    this->touch();

    this->draw();
}

void dtkVisualizationDecoratorScalarIsolines::touch(void)
{
    dtkVisualizationDecoratorWithClut::touch();

    if (!d->show_actor_cb->isChecked()) {
        return;
    }

    auto field_name = d_func()->current_field_name;

    if (field_name.isEmpty() || !d_func()->dataset)
        return;

    auto&& isoline_range = d_func()->ranges[field_name];

    auto count = d->isolines_counts[field_name];

    d->isolines_counts_sb->blockSignals(true);
    d->isolines_counts_sb->setValue(count);
    d->isolines_counts_sb->blockSignals(false);

    d->isolines->GenerateValues(count, isoline_range[0], isoline_range[1]);
    d->isolines->Modified();
}

void dtkVisualizationDecoratorScalarIsolines::setData(const QVariant& data)
{
    vtkDataSet *dataset = data.value<vtkDataSet *>();
    if (!dataset) {
        dtkWarn() << Q_FUNC_INFO << "vtkDataSet is expected. Input data is not stored.";
        return;
    }

    d_func()->clear();
    d_func()->retrieveScalarPoints(dataset);
    d_func()->retrieveScalarCells(dataset);

    if (!this->isDecorating()) {
        dtkWarn() << Q_FUNC_INFO << "vtkDataSet has no field to decorate. Nothing is done.";
        d_func()->clear();
        return;
    }

    d_func()->dataset = dataset;
    this->restoreSettings();

    d->c2p_filter->SetInputData(dataset);

    d->isolines_counts.clear();
    for (auto field_name : d_func()->eligible_field_names) {
        d->isolines_counts[field_name] = d->default_isolines_count;
    }

    d->isolines->Modified();
    d->mapper->Modified();

    d_func()->sortEligibleFields();
    this->setCurrentFieldName(d_func()->current_field_name);

    if(this->canvas()) {
        this->canvas()->renderer()->AddActor(d->actor);
    }

    d_func()->enableScalarBar();
}

void dtkVisualizationDecoratorScalarIsolines::setCanvas(dtkVisualizationCanvas *canvas)
{
    this->unsetCanvas();

    d_func()->view = dynamic_cast<dtkVisualizationView2D *>(canvas);
    if (!d_func()->view) {
        qWarning() << Q_FUNC_INFO << "View 2D or view 3D expected as canvas. Canvas is reset to nullptr.";
        return;
    }

    if(d->isolines->GetInput()) {
        d_func()->view->renderer()->AddActor(d->actor);
    }

    d_func()->enableScalarBar();
}

void dtkVisualizationDecoratorScalarIsolines::unsetCanvas(void)
{
    if (d_func()->view) {
        d_func()->view->renderer()->RemoveActor(d->actor);
    }

    d_func()->disableScalarBar();

    d_func()->view = nullptr;
}

void dtkVisualizationDecoratorScalarIsolines::setVisibility(bool b)
{
    d->isolines_counts_sb->setEnabled(b);
    dtkVisualizationDecoratorWithClut::setVisibility(b);
    d->actor->SetVisibility(b);

}

void dtkVisualizationDecoratorScalarIsolines::setCurrentRange(double min, double max)
{
    if (d_func()->current_field_name.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "Field not selected";
        return;
    }
    if (min >= max) {
        qWarning() << Q_FUNC_INFO << " min >= max : " << min << max;
        return;
    }

    dtkVisualizationDecoratorWithClut::setCurrentRange(min, max);
}

void dtkVisualizationDecoratorScalarIsolines::setCurrentIsolinesCount(std::size_t count)
{
    QString& field_name = d_func()->current_field_name;
    if (field_name.isEmpty()) {
        if (d_func()->default_field_name.isEmpty())
            return;
        field_name = d_func()->default_field_name;
    }
    d->isolines_counts[field_name] = count;

    this->touch();
}

bool dtkVisualizationDecoratorScalarIsolines::setCurrentFieldName(const QString& field_name)
{
    if (field_name.isEmpty()) {
        dtkWarn() << Q_FUNC_INFO << "Field name is empty, nothing is done.";
        return false;
    }

    if (!d_func()->dataset) {
        dtkWarn() << Q_FUNC_INFO << "Before calling setCurrentFieldName, setDataSet must be called.";
        return false;
    }

    if(!d_func()->eligible_field_names.contains(field_name)) {
        dtkWarn() << Q_FUNC_INFO << "The field name :" << field_name << "that was specified doesn't match any of the eligible scalar field names";

        return false;
    }

    using Support = dtkVisualizationDecoratorWithClut::Support;
    int support = d_func()->supports[field_name];
    if(support == Support::Point) {
        d->isolines->SetInputData(d_func()->dataset);
        d->isolines->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, qPrintable(field_name));
    } else if(support == Support::Cell) {
        d->c2p_filter->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS, qPrintable(field_name));
        d->isolines->SetInputConnection(d->c2p_filter->GetOutputPort());
        d->isolines->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, qPrintable(field_name));
        d->c2p_filter->Modified();
    }
    d->isolines->Modified();

    return dtkVisualizationDecoratorWithClut::setCurrentFieldName(field_name);
}

void dtkVisualizationDecoratorScalarIsolines::setColorMap(const QMap<double, QColor>& new_colormap)
{
    dtkVisualizationDecoratorWithClut::setColorMap(new_colormap);

    d->mapper->SetLookupTable(d_func()->color_function);
    d->mapper->SelectColorArray(qPrintable(d_func()->current_field_name));
    auto&& range = d_func()->ranges[d_func()->current_field_name];
    d->mapper->SetScalarRange(range[0], range[1]);
    d->mapper->Modified();

    //set opacity
    // int size = opacity_function->GetSize();

    // double val[4];
    // double opacity_min = 1.;
    // for(std::size_t i = 0; i < size; ++i) {
    //     opacity_function->GetNodeValue(i, val);
    //     if(val[1] < opacity_min) opacity_min = val[1];
    // }

    // d->actor->GetProperty()->SetOpacity(opacity_min);
    // d->actor->Modified();
}

//
// dtkVisualizationDecoratorScalarIsolines.cpp ends here
