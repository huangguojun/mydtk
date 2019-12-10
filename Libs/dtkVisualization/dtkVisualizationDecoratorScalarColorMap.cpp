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

#include "dtkVisualizationDecoratorScalarColorMap.h"
#include "dtkVisualizationDecoratorWithClut_p.h"
#include "dtkVisualizationMetaType.h"
#include "dtkVisualizationView2D.h"

#include <dtkLog>

#include <QtGui>
#include <QtWidgets>

#include <vtkActor.h>
#include <vtkColorTransferFunction.h>
#include <vtkCellData.h>
#include <vtkDataSet.h>
#include <vtkDataSetMapper.h>
#include <vtkLookupTable.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorScalarColorMapPrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationDecoratorScalarColorMapPrivate
{
public:
    vtkSmartPointer<vtkActor> actor;
    vtkSmartPointer<vtkDataSetMapper> mapper;

public:
    QCheckBox *show_actor_cb = nullptr;

public slots:
    void onCanvasFocused(void);
};

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorScalarColorMap implementation
// ///////////////////////////////////////////////////////////////////

dtkVisualizationDecoratorScalarColorMap::dtkVisualizationDecoratorScalarColorMap(void): dtkVisualizationDecoratorWithClut(), d(new dtkVisualizationDecoratorScalarColorMapPrivate())
{
    d->mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    d->mapper->SetColorModeToMapScalars();
    d->mapper->SetScalarVisibility(true);

    d->actor = vtkSmartPointer<vtkActor>::New();
    d->actor->SetMapper(d->mapper);

    //////////
    // Inspectors creation

    d->show_actor_cb = new QCheckBox;

    //////////
    // Inspectors connections

    connect(d->show_actor_cb, &QCheckBox::stateChanged, [=] (int state) {
            this->saveSettings("visibility", state == Qt::Checked);
            this->setVisibility(state == Qt::Checked);
            this->draw();
        });

    this->setObjectName("Scalar ColorMap");
    d->show_actor_cb->setObjectName("Display");

    d_func()->inspectors << d->show_actor_cb;

}

dtkVisualizationDecoratorScalarColorMap::~dtkVisualizationDecoratorScalarColorMap(void)
{
    this->unsetCanvas();

    delete d;
    d = nullptr;
}

void dtkVisualizationDecoratorScalarColorMap::setData(const QVariant& data)
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
    d->mapper->SetInputData(d_func()->dataset);

    d_func()->sortEligibleFields();
    this->setCurrentFieldName(d_func()->current_field_name);

    if (this->canvas()) {
        this->canvas()->renderer()->AddActor(d->actor);
    }

    d_func()->enableScalarBar();
}

void dtkVisualizationDecoratorScalarColorMap::setCanvas(dtkVisualizationCanvas *canvas)
{
    this->unsetCanvas();

    d_func()->view = dynamic_cast<dtkVisualizationView2D *>(canvas);
    if (!d_func()->view) {
        qWarning() << Q_FUNC_INFO << "View 2D or view 3D expected as canvas. Canvas is reset to nullptr.";
        return;
    }

    if (d->mapper->GetInput()) {
        d_func()->view->renderer()->AddActor(d->actor);
    }

    d_func()->enableScalarBar();
}

void dtkVisualizationDecoratorScalarColorMap::unsetCanvas(void)
{
    if (d_func()->view) {
        d_func()->view->renderer()->RemoveActor(d->actor);
    }

    d_func()->disableScalarBar();

    d_func()->view = nullptr;
}

void dtkVisualizationDecoratorScalarColorMap::setVisibility(bool visible)
{
    d->actor->SetVisibility(visible);
    dtkVisualizationDecoratorWithClut::setVisibility(visible);
}

void dtkVisualizationDecoratorScalarColorMap::restoreSettings(void)
{
    dtkVisualizationDecoratorWithClut::restoreSettings();

    d->actor->SetVisibility(d_func()->default_visibility);
    d->show_actor_cb->blockSignals(true);
    d->show_actor_cb->setChecked(d_func()->default_visibility);
    d->show_actor_cb->blockSignals(false);
}


bool dtkVisualizationDecoratorScalarColorMap::setCurrentFieldName(const QString& field_name)
{
    if (!dtkVisualizationDecoratorWithClut::setCurrentFieldName(field_name)) {
        return false;
    }

    using Support = dtkVisualizationDecoratorWithClut::Support;
    int support = d_func()->supports[field_name];
    if(support == Support::Point) {
        d->mapper->SetScalarModeToUsePointFieldData();
    } else if(support == Support::Cell) {
        d->mapper->SetScalarModeToUseCellFieldData();
    }
    d->mapper->SelectColorArray(qPrintable(field_name));
    d->mapper->Modified();

    return true;
}

void dtkVisualizationDecoratorScalarColorMap::setColorMap(const QMap<double, QColor>& new_colormap)
{
    dtkVisualizationDecoratorWithClut::setColorMap(new_colormap);

    d->mapper->SetLookupTable(d_func()->color_function);
    d->mapper->SelectColorArray(qPrintable(d_func()->current_field_name));
    auto&& range = d_func()->ranges[d_func()->current_field_name];
    d->mapper->SetScalarRange(range[0], range[1]);
    d->mapper->Modified();

    // -- set opacity

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

void dtkVisualizationDecoratorScalarColorMap::setTransform(vtkTransform *transform)
{
    d_func()->transform = transform;
    d->actor->SetUserTransform(transform);
}

// /////////////////////////////////////////////////////////////////////////////

//
// dtkVisualizationDecoratorScalarColorMap.cpp ends here
