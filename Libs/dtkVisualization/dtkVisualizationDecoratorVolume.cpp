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

#include "dtkVisualizationDecoratorVolume.h"
#include "dtkVisualizationDecoratorWithClut_p.h"
#include "dtkVisualizationMetaType.h"
#include "dtkVisualizationView2D.h"

#include <dtkLog>

#include <QtGui>
#include <QtWidgets>

#include <vtkActor.h>
#include <vtkAssembly.h>
#include <vtkCellData.h>
#include <vtkCellDataToPointData.h>
#include <vtkColorTransferFunction.h>
#include <vtkDataSet.h>
#include <vtkDataSetMapper.h>
#include <vtkLookupTable.h>
#include <vtkOutlineFilter.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkTextProperty.h>
#include <vtkTransformFilter.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkVolumeProperty.h>

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorVolumePrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationDecoratorVolumePrivate
{
public:
    vtkSmartPointer<vtkAssembly> actor;
    vtkSmartPointer<vtkSmartVolumeMapper> mapper;
    vtkSmartPointer<vtkVolume> volume;
    vtkSmartPointer<vtkVolumeProperty> volume_property;
    vtkSmartPointer<vtkPiecewiseFunction> opacity_transfer_function;
    vtkSmartPointer<vtkOutlineFilter> outline_contour;
    vtkSmartPointer<vtkActor> outline_contour_actor;
    vtkSmartPointer<vtkPolyDataMapper> outline_contour_mapper;
    vtkSmartPointer<vtkCellDataToPointData> c2p_filter;

public:
    QCheckBox *show_actor_cb = nullptr;

public:
    QMap<double, double> opacity_map;

public slots:
    void onCanvasFocused(void);
};

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorVolume implementation
// ///////////////////////////////////////////////////////////////////

dtkVisualizationDecoratorVolume::dtkVisualizationDecoratorVolume(void)
    : dtkVisualizationDecoratorWithClut(), d(new dtkVisualizationDecoratorVolumePrivate())
{
    d->c2p_filter = vtkSmartPointer<vtkCellDataToPointData>::New();
    d->mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
    d->mapper->SetRequestedRenderMode(vtkSmartVolumeMapper::DefaultRenderMode);

    d->opacity_transfer_function = vtkSmartPointer<vtkPiecewiseFunction>::New();
    d->outline_contour = vtkSmartPointer<vtkOutlineFilter>::New();

    d->outline_contour_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    d->outline_contour_mapper->SetInputConnection(d->outline_contour->GetOutputPort());

    d->outline_contour_actor = vtkSmartPointer<vtkActor>::New();
    d->outline_contour_actor->SetMapper(d->outline_contour_mapper);
    d->outline_contour_actor->GetProperty()->SetColor(1, 0, 0);
    d->outline_contour_actor->SetVisibility(1);

    d->volume_property = vtkSmartPointer<vtkVolumeProperty>::New();
    d->volume_property->SetInterpolationTypeToLinear();
    d->volume_property->SetScalarOpacity(d->opacity_transfer_function);
    d->volume_property->ShadeOff();
    d->volume_property->SetAmbient(0.1);
    d->volume_property->SetDiffuse(0.5);
    d->volume_property->SetSpecular(0.4);

    d->volume = vtkSmartPointer<vtkVolume>::New();
    d->volume->SetMapper(d->mapper);
    d->volume->SetProperty(d->volume_property);

    d->actor = vtkSmartPointer<vtkAssembly>::New();
    d->actor->AddPart(d->volume);
    d->actor->AddPart(d->outline_contour_actor);

    d->show_actor_cb = new QCheckBox;

    //////////
    // Inspectors connections

    connect(d->show_actor_cb, &QCheckBox::stateChanged, [=](int state) {
        this->saveSettings("visibility", state == Qt::Checked);
        this->setVisibility(state == Qt::Checked);
        this->draw();
    });

    this->setObjectName("Volume");
    d->show_actor_cb->setObjectName("Display");

    d_func()->inspectors << d->show_actor_cb;
}

dtkVisualizationDecoratorVolume::~dtkVisualizationDecoratorVolume(void)
{
    this->unsetCanvas();

    delete d;
    d = nullptr;
}

void dtkVisualizationDecoratorVolume::setData(const QVariant &data)
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
    d->mapper->SetInputData(dataset);
    d->outline_contour->SetInputData(dataset);

    d_func()->sortEligibleFields();
    this->setCurrentFieldName(d_func()->current_field_name);

    if (this->canvas()) {
        this->canvas()->renderer()->AddActor(d->actor);
    }

    d->mapper->Modified();
    d->outline_contour->Modified();

    d_func()->enableScalarBar();
}

void dtkVisualizationDecoratorVolume::setCanvas(dtkVisualizationCanvas *canvas)
{
    this->unsetCanvas();

    d_func()->view = dynamic_cast<dtkVisualizationView2D *>(canvas);
    if (!d_func()->view) {
        qWarning() << Q_FUNC_INFO
                   << "View 2D or view 3D expected as canvas. Canvas is reset "
                      "to nullptr.";
        return;
    }

    if (d->mapper->GetInput()) {
        d_func()->view->renderer()->AddActor(d->actor);
    }

    d_func()->enableScalarBar();
}

void dtkVisualizationDecoratorVolume::unsetCanvas(void)
{
    if (d_func()->view) {
        d_func()->view->renderer()->RemoveActor(d->actor);
    }

    d_func()->disableScalarBar();

    d_func()->view = nullptr;
}

void dtkVisualizationDecoratorVolume::setVisibility(bool visible)
{
    d->actor->SetVisibility(visible);
    dtkVisualizationDecoratorWithClut::setVisibility(visible);
}

void dtkVisualizationDecoratorVolume::restoreSettings(void)
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

bool dtkVisualizationDecoratorVolume::setCurrentFieldName(const QString &field_name)
{
    if (!dtkVisualizationDecoratorWithClut::setCurrentFieldName(field_name))
        return false;

    using Support = dtkVisualizationDecoratorWithClut::Support;
    int support = d_func()->supports[field_name];
    if (support == Support::Cell) {
        d->c2p_filter->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS,
                                              qPrintable(field_name));
        d->mapper->SetInputConnection(d->c2p_filter->GetOutputPort());
    }
    d->mapper->SetScalarModeToUsePointFieldData();
    d->mapper->SelectScalarArray(qPrintable(field_name));
    d->mapper->Modified();

    return true;
}

void dtkVisualizationDecoratorVolume::setColorMap(const QMap<double, QColor> &new_colormap)
{
    dtkVisualizationDecoratorWithClut::setColorMap(new_colormap);

    d->volume_property->SetColor(d_func()->color_function);

    if (d->opacity_map.isEmpty()) {
        auto &&range = d_func()->ranges[d_func()->current_field_name];
        double min = range[0];
        double max = range[1];
        double mid = (min + max) / 2.0;

        d->opacity_map.insert(min, 0.01);
        d->opacity_map.insert(mid, 0.1);
        d->opacity_map.insert(max, 0.25);
        this->setAlphaMap(d->opacity_map);
    } else {
        d->volume_property->Modified();
        d->mapper->Modified();
    }
}

void dtkVisualizationDecoratorVolume::setAlphaMap(const QMap<double, double> &map)
{
    d->opacity_map = map;
    d->opacity_transfer_function->RemoveAllPoints();

    foreach (double value, map.keys()) {
        d->opacity_transfer_function->AddPoint(value, map[value]);
        d->opacity_transfer_function->AddPoint(value, map[value]);
        d->opacity_transfer_function->AddPoint(value, map[value]);
    }

    d->opacity_transfer_function->Modified();
    d->volume_property->Modified();
    d->mapper->Modified();
}

//
// dtkVisualizationDecoratorVolume.cpp ends here
