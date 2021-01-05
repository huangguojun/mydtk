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

#include "dtkVisualizationDecoratorAxes.h"
#include "dtkVisualizationMetaType.h"
#include "dtkVisualizationView2D.h"

#include <dtkLog>

#include <QtGui>
#include <QtWidgets>

#include <vtkActor.h>
#include <vtkAxesActor.h>
#include <vtkAxisActor.h>
#include <vtkCellData.h>
#include <vtkCellDataToPointData.h>
#include <vtkColorTransferFunction.h>
#include <vtkCubeAxesActor2D.h>
#include <vtkDataSet.h>
#include <vtkDataSetMapper.h>
#include <vtkLookupTable.h>
#include <vtkOrientationMarkerWidget.h>
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
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkVolumeProperty.h>

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorAxesPrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationDecoratorAxesPrivate
{
public:
    vtkDataSet *dataset = nullptr;
    dtkVisualizationView2D *view = nullptr;

public:
    vtkSmartPointer<vtkActor> outline_contour_actor;
    vtkSmartPointer<vtkCubeAxesActor2D> cube_axes_actor;
    vtkSmartPointer<vtkAxesActor> axes;
    vtkSmartPointer<vtkOrientationMarkerWidget> marker;
    vtkSmartPointer<vtkCellDataToPointData> c2p_filter;
    vtkSmartPointer<vtkTransform> transform;

public:
    QCheckBox *show_actor_cb = nullptr;
    QCheckBox *show_axes_cb = nullptr;
    QCheckBox *show_cube_axes_cb = nullptr;

public:
    QList<QWidget *> inspectors;

public:
    bool lock = false;

public slots:
    void onCanvasFocused(void);
};

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorAxes implementation
// ///////////////////////////////////////////////////////////////////

dtkVisualizationDecoratorAxes::dtkVisualizationDecoratorAxes(void)
    : dtkVisualizationDecorator(), d(new dtkVisualizationDecoratorAxesPrivate())
{
    d->transform = vtkSmartPointer<vtkTransform>::New();

    d->cube_axes_actor = vtkSmartPointer<vtkCubeAxesActor2D>::New();

    d->cube_axes_actor->SetNumberOfLabels(5);
    d->cube_axes_actor->SetFontFactor(0.5);

    d->axes = vtkAxesActor::New();
    d->axes->SetConeRadius(0.3);
    d->axes->SetCylinderRadius(0.005);
    d->axes->SetAxisLabels(true);

    d->marker = vtkOrientationMarkerWidget::New();
    d->marker->SetOutlineColor(0.9300, 0.5700, 0.1300);
    d->marker->SetOrientationMarker(d->axes);
    d->marker->SetViewport(0.0, 0.0, 0.25, 0.25);

    d->show_axes_cb = new QCheckBox;
    d->show_cube_axes_cb = new QCheckBox;

    //////////
    // Inspectors connections

    connect(d->show_axes_cb, &QCheckBox::stateChanged, [=](int state) {
        this->saveSettings("show_axes", state == Qt::Checked);
        d->marker->SetEnabled(state == Qt::Checked);
        this->draw();
    });

    connect(d->show_cube_axes_cb, &QCheckBox::stateChanged, [=](int state) {
        this->saveSettings("show_cube_axes", state == Qt::Checked);
        d->cube_axes_actor->SetVisibility(state == Qt::Checked);
        this->draw();
    });

    this->setObjectName("Axes");
    d->show_axes_cb->setObjectName("Display Orientation");
    d->show_cube_axes_cb->setObjectName("Display Cube Axes");

    d->inspectors << d->show_axes_cb << d->show_cube_axes_cb;
}

dtkVisualizationDecoratorAxes::~dtkVisualizationDecoratorAxes(void)
{
    this->unsetCanvas();

    delete d;
    d = nullptr;
}

void dtkVisualizationDecoratorAxes::setData(const QVariant &data)
{
    d->dataset = data.value<vtkDataSet *>();
    if (!d->dataset) {
        dtkWarn() << Q_FUNC_INFO << "vtkDataSet is expected. Input data is not stored.";
        return;
    }

    if (!this->isDecorating()) {
        dtkWarn() << Q_FUNC_INFO << "vtkDataSet has no field to decorate. Nothing is done.";
        return;
    }
    this->restoreSettings();

    d->cube_axes_actor->SetBounds(d->dataset->GetBounds());

    if (this->canvas()) {
        d->marker->SetEnabled(d->show_axes_cb->checkState() == Qt::Checked);
        //        this->canvas()->renderer()->AddActor(d->actor);
    }
}

void dtkVisualizationDecoratorAxes::setCanvas(dtkVisualizationCanvas *canvas)
{
    this->unsetCanvas();

    d->view = dynamic_cast<dtkVisualizationView2D *>(canvas);
    if (!d->view) {
        qWarning() << Q_FUNC_INFO
                   << "View 2D or view 3D expected as canvas. Canvas is reset "
                      "to nullptr.";
        return;
    }

    d->marker->SetInteractor(d->view->renderer()->GetRenderWindow()->GetInteractor());

    d->view->renderer()->AddActor(d->cube_axes_actor);
    d->marker->SetEnabled(d->show_axes_cb->checkState() == Qt::Checked);

    d->cube_axes_actor->SetCamera(d->view->renderer()->GetActiveCamera());

    if (!d->dataset) {
        d->cube_axes_actor->SetBounds(d->view->renderer()->ComputeVisiblePropBounds());
    } else {
        d->cube_axes_actor->SetBounds(d->dataset->GetBounds());
    }
}

void dtkVisualizationDecoratorAxes::unsetCanvas(void)
{
    if (d->view) {
        d->view->renderer()->RemoveActor(d->cube_axes_actor);

        d->marker->SetEnabled(false);
        d->marker->SetInteractor(nullptr);
    }

    d->view = nullptr;
}

void dtkVisualizationDecoratorAxes::setVisibility(bool visible)
{
    d->cube_axes_actor->SetVisibility(visible);
}

QVariant dtkVisualizationDecoratorAxes::data(void) const
{
    if (d->dataset) {
        return dtk::variantFromValue(d->dataset);

    } else {
        return QVariant();
    }
}

void dtkVisualizationDecoratorAxes::touch(void)
{
    if (!this->canvas()) {
        dtkWarn() << Q_FUNC_INFO << "No canvas was set, call setCanvas to call draw on a canvas.";
        return;
    }
    d->cube_axes_actor->SetBounds(d->dataset->GetBounds());

    this->draw();
}

bool dtkVisualizationDecoratorAxes::isDecorating(void)
{
    return d->dataset;
}

dtkVisualizationCanvas *dtkVisualizationDecoratorAxes::canvas(void) const
{
    return d->view;
}

void dtkVisualizationDecoratorAxes::restoreSettings(void)
{
    QString name = this->objectName();
    if (name.isEmpty())
        return;

    QSettings settings;
    settings.beginGroup("canvas");
    bool show_axes = settings.value(name + "_show_axes", true).toBool();
    bool show_cube_axes = settings.value(name + "_show_cube_axes", true).toBool();
    settings.endGroup();

    d->show_axes_cb->blockSignals(true);
    d->show_axes_cb->setChecked(show_axes);
    d->show_axes_cb->blockSignals(false);

    d->cube_axes_actor->SetVisibility(show_cube_axes);
    d->show_cube_axes_cb->blockSignals(true);
    d->show_cube_axes_cb->setChecked(show_cube_axes);
    d->show_cube_axes_cb->blockSignals(false);
}

QList<QWidget *> dtkVisualizationDecoratorAxes::inspectors(void) const
{
    return d->inspectors;
}

//
// dtkVisualizationDecoratorAxes.cpp ends here
