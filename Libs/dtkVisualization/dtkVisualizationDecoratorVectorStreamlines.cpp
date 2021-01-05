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

#include "dtkVisualizationDecoratorVectorStreamlines.h"

#include "dtkVisualizationDecoratorWithClut_p.h"

#include "dtkVisualizationMetaType.h"

#include "dtkVisualizationCanvas.h"
#include "dtkVisualizationDecoratorClutEditorBase.h"
#include "dtkVisualizationView2D.h"

#include <dtkLog>
#include <dtkWidgets>

#include <QtGui/QColor>

#include <vtkActor.h>
#include <vtkCellData.h>
#include <vtkCenterOfMass.h>
#include <vtkColorTransferFunction.h>
#include <vtkDataSet.h>
#include <vtkEvenlySpacedStreamlines2D.h>
#include <vtkLookupTable.h>
#include <vtkMatrix4x4.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorVectorStreamlinesPrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationDecoratorVectorStreamlinesPrivate
{
public:
    vtkSmartPointer<vtkActor> actor;
    vtkSmartPointer<vtkPolyDataMapper> mapper;

public:
    vtkSmartPointer<vtkActor> source_actor;
    vtkSmartPointer<vtkPolyDataMapper> source_mapper;

public:
    vtkSmartPointer<vtkTransform> transform;
    vtkSmartPointer<vtkTransformFilter> transform_filter;
    vtkSmartPointer<vtkEvenlySpacedStreamlines2D> streamlines;

public:
    vtkSmartPointer<vtkSphereSource> source;

public:
    QCheckBox *show_actor_cb = nullptr;
    QCheckBox *show_source_actor_cb = nullptr;
};

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorVectorStreamlines implementation
// ///////////////////////////////////////////////////////////////////

dtkVisualizationDecoratorVectorStreamlines::dtkVisualizationDecoratorVectorStreamlines(void)
    : dtkVisualizationDecoratorWithClut(),
      d(new dtkVisualizationDecoratorVectorStreamlinesPrivate())
{
    vtkSmartPointer<vtkMatrix4x4> matrix = vtkSmartPointer<vtkMatrix4x4>::New();
    matrix->Identity();
    matrix->SetElement(2, 2, 0.);

    d->transform = vtkSmartPointer<vtkTransform>::New();
    d->transform->SetMatrix(matrix);

    d->transform_filter = vtkSmartPointer<vtkTransformFilter>::New();
    d->transform_filter->SetTransform(d->transform);

    d->streamlines = vtkSmartPointer<vtkEvenlySpacedStreamlines2D>::New();
    d->streamlines->SetInputConnection(d->transform_filter->GetOutputPort());
    d->streamlines->SetIntegratorType(1);
    d->streamlines->SetClosedLoopMaximumDistance(0.2);
    d->streamlines->SetSeparatingDistance(5.);
    d->streamlines->SetSeparatingDistanceRatio(0.4);

    d->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    d->mapper->SetInputConnection(d->streamlines->GetOutputPort());

    d->actor = vtkSmartPointer<vtkActor>::New();
    d->actor->SetMapper(d->mapper);
    d->actor->GetProperty()->SetColor(0, 0, 0);
    d->actor->GetProperty()->SetLineWidth(3);

    d->source = vtkSmartPointer<vtkSphereSource>::New();
    d->source->SetRadius(5.);

    d->source_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    d->source_mapper->SetInputConnection(d->source->GetOutputPort());

    d->source_actor = vtkSmartPointer<vtkActor>::New();
    d->source_actor->SetMapper(d->source_mapper);
    d->source_actor->SetVisibility(false);

    //////////
    // Inspectors creation and setup

    d->show_actor_cb = new QCheckBox;
    d->show_source_actor_cb = new QCheckBox;

    d->show_source_actor_cb->setChecked(false);

    //////////
    // Inspectors connections

    connect(d->show_actor_cb, &QCheckBox::stateChanged, [=](int state) {
        this->saveSettings("visibility", state == Qt::Checked);
        this->setVisibility(state == Qt::Checked);
        this->draw();
    });

    connect(d->show_source_actor_cb, &QCheckBox::stateChanged, [=](int state) {
        d->source_actor->SetVisibility(state == Qt::Checked);
        this->draw();
    });

    this->setObjectName("Vector Streamlines 2D");

    d->show_actor_cb->setObjectName("Display");
    d->show_source_actor_cb->setObjectName("Display Source");

    d_func()->inspectors << d->show_actor_cb << d->show_source_actor_cb;
}

dtkVisualizationDecoratorVectorStreamlines::~dtkVisualizationDecoratorVectorStreamlines(void)
{
    dtkVisualizationDecoratorVectorStreamlines::unsetCanvas();

    delete d;

    d = nullptr;
}

void dtkVisualizationDecoratorVectorStreamlines::restoreSettings(void)
{
    QString name = this->objectName();
    if (name.isEmpty())
        return;
    dtkVisualizationDecoratorWithClut::restoreSettings();
    d->actor->SetVisibility(d_func()->default_visibility);
    d->show_actor_cb->blockSignals(true);
    d->show_actor_cb->setChecked(d_func()->default_visibility);
    d->show_actor_cb->blockSignals(false);
}

bool dtkVisualizationDecoratorVectorStreamlines::isCurrentFieldUniform(void)
{
    auto r = d_func()->ranges[d_func()->current_field_name];
    if (std::abs(r[0] - r[1]) < 1.e-12) {
        return true;
    }
    return false;
}

void dtkVisualizationDecoratorVectorStreamlines::setData(const QVariant &data)
{
    vtkDataSet *dataset = data.value<vtkDataSet *>();
    if (!dataset) {
        dtkWarn() << Q_FUNC_INFO << "vtkDataSet is expected. Input data is not stored.";
        return;
    }

    d_func()->clear();
    d_func()->retrieveVectorPoints(dataset);
    d_func()->retrieveVectorCells(dataset);

    if (!this->isDecorating()) {
        dtkWarn() << Q_FUNC_INFO << "vtkDataSet has no field to decorate. Nothing is done.";
        d_func()->clear();
        return;
    }

    d_func()->dataset = dataset;
    this->restoreSettings();

    d->transform_filter->SetInputData(dataset);

    vtkSmartPointer<vtkCenterOfMass> center_of_mass = vtkSmartPointer<vtkCenterOfMass>::New();
    center_of_mass->SetInputConnection(d->transform_filter->GetOutputPort());
    center_of_mass->SetUseScalarsAsWeights(false);
    center_of_mass->Update();

    double center[3];
    center_of_mass->GetCenter(center);

    d->streamlines->SetStartPosition(center[0], center[1], 0.);
    d->source->SetCenter(center[0], center[1], 0.);

    d->streamlines->Modified();
    d->mapper->Modified();

    d->source->Modified();

    d_func()->sortEligibleFields();
    this->setCurrentFieldName(d_func()->current_field_name);

    if (this->canvas()) {
        this->canvas()->renderer()->AddActor(d->actor);
        this->canvas()->renderer()->AddActor(d->source_actor);
    }

    d_func()->enableScalarBar();
}

void dtkVisualizationDecoratorVectorStreamlines::setCanvas(dtkVisualizationCanvas *canvas)
{
    this->unsetCanvas();

    d_func()->view = dynamic_cast<dtkVisualizationView2D *>(canvas);
    if (!d_func()->view) {
        qWarning() << Q_FUNC_INFO
                   << "View 2D or view 3D expected as canvas. Canvas is reset "
                      "to nullptr.";
        return;
    }

    if (this->isDecorating()) {
        d_func()->view->renderer()->AddActor(d->actor);
        d_func()->view->renderer()->AddActor(d->source_actor);
    }

    d_func()->enableScalarBar();
}

void dtkVisualizationDecoratorVectorStreamlines::unsetCanvas(void)
{
    if (d_func()->view) {
        d_func()->view->renderer()->RemoveActor(d->actor);
        d_func()->view->renderer()->RemoveActor(d->source_actor);
    }

    d_func()->disableScalarBar();

    d_func()->view = nullptr;
}

void dtkVisualizationDecoratorVectorStreamlines::setVisibility(bool visible)
{
    dtkVisualizationDecoratorWithClut::setVisibility(visible);
    d->actor->SetVisibility(visible);
}

bool dtkVisualizationDecoratorVectorStreamlines::setCurrentFieldName(const QString &field_name)
{
    if (field_name.isEmpty()) {
        dtkWarn() << Q_FUNC_INFO << "Field name is empty, nothing is done.";
        return false;
    }

    if (!d_func()->dataset) {
        dtkWarn() << Q_FUNC_INFO
                  << "Before calling setCurrentFieldName, setDataSet must be called.";
        return false;
    }

    if (!d_func()->eligible_field_names.contains(field_name)) {
        dtkWarn() << Q_FUNC_INFO << "The field name :" << field_name
                  << "that was specified doesn't match any of the eligible "
                     "scalar field names";
        return false;
    }

    d_func()->current_field_name = field_name;

    using Support = dtkVisualizationDecoratorWithClut::Support;
    int support = d_func()->supports[field_name];
    if (support == Support::Point) {
        d_func()->dataset->GetPointData()->SetActiveVectors(
                qPrintable(d_func()->current_field_name));
        d->streamlines->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS,
                                               qPrintable(field_name));
    } else if (support == Support::Cell) {
        d_func()->dataset->GetCellData()->SetActiveVectors(
                qPrintable(d_func()->current_field_name));
        d->streamlines->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS,
                                               qPrintable(field_name));
    }

    return dtkVisualizationDecoratorWithClut::setCurrentFieldName(field_name);
}

void dtkVisualizationDecoratorVectorStreamlines::setColorMap(
        const QMap<double, QColor> &new_colormap)
{
    dtkVisualizationDecoratorWithClut::setColorMap(new_colormap);

    // dtkVisualizationWidgetsColorMapEditor *color_map_editor =
    // d_func()->colormap_editor;

    // QString& current_field_name = d_func()->current_field_name;
    // vtkSmartPointer<vtkColorTransferFunction>& color_function =
    // d_func()->color_function;

    // if(d->mapper->GetInput()) {
    //     d->mapper->SetLookupTable(color_function);

    //     auto&& range = d_func()->ranges[current_field_name];
    //     d->mapper->SetScalarRange(range[0], range[1]);

    //     //set opacity
    //     // int size = opacity_function->GetSize();

    //     // double val[4];
    //     // double opacity_min = 1.;
    //     // for(std::size_t i = 0; i < size; ++i) {
    //     //     opacity_function->GetNodeValue(i, val);
    //     //     if(val[1] < opacity_min) opacity_min = val[1];
    //     // }

    //     // d->actor->GetProperty()->SetOpacity(opacity_min);

    //     d->mapper->Modified();
    //     d->actor->Modified();

    //     // if (this->canvas() && this->canvas()->interactor()) {
    //     //     this->canvas()->interactor()->Render();
    //     //     this->canvas()->renderer()->GetRenderWindow()->Render();
    //     // }
    // }
}

//
// dtkVisualizationDecoratorVectorStreamlines.cpp ends here
