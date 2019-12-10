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

#include "dtkVisualizationDecoratorSlices.h"
#include "dtkVisualizationDecoratorWithClut_p.h"
#include "dtkVisualizationMetaType.h"
#include "dtkVisualizationView2D.h"

#include <dtkLog>
#include <dtkVisualizationWidgets>

#include <QtGui>
#include <QtWidgets>

#include <vtkActor.h>
#include <vtkCellDataToPointData.h>
#include <vtkColorTransferFunction.h>
#include <vtkCommand.h>
#include <vtkCellData.h>
#include <vtkCubeAxesActor.h>
#include <vtkDataSet.h>
#include <vtkDataSetMapper.h>
#include <vtkImageMapToColors.h>
#include <vtkImagePlaneWidget.h>
#include <vtkLookupTable.h>
#include <vtkOutlineFilter.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkTextProperty.h>
#include <vtkTransformFilter.h>
#include <vtkTransformPolyDataFilter.h>


// /////////////////////////////////////////////////////////////////
// dtkSliceActorObserver
// /////////////////////////////////////////////////////////////////

class dtkSliceActorObserver : public vtkCommand
{
private:
    dtkSliceActorObserver(void) {
        control = nullptr;
        origin = 0.0;
    };

public:
    static dtkSliceActorObserver *New(void)
    {
        return new dtkSliceActorObserver;
    };

    virtual void Execute(vtkObject *caller, unsigned long event, void *)
    {
        vtkImagePlaneWidget *w = reinterpret_cast<vtkImagePlaneWidget*>(caller);

        if (control && event == vtkCommand::InteractionEvent) {
            double pos = w->GetSlicePosition() - origin;
            control->blockSignals(true);
            control->setValue(pos);
            control->blockSignals(false);
        }
    };

public:
    dtkVisualizationWidgetsSliceControls *control;
    double origin;
};

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorSlicesPrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationDecoratorSlicesPrivate
{
public:
    vtkSmartPointer<vtkImagePlaneWidget> volume_slice_x;
    vtkSmartPointer<vtkImagePlaneWidget> volume_slice_y;
    vtkSmartPointer<vtkImagePlaneWidget> volume_slice_z;
    vtkSmartPointer<vtkActor> actor;
    vtkSmartPointer<vtkCellDataToPointData> c2p_filter;

public:
    QCheckBox *show_actor_cb     = nullptr;
    dtkVisualizationWidgetsSliceControls *enable_slicing_x = nullptr;
    dtkVisualizationWidgetsSliceControls *enable_slicing_y = nullptr;
    dtkVisualizationWidgetsSliceControls *enable_slicing_z = nullptr;
    dtkSliceActorObserver *observer_x = nullptr;
    dtkSliceActorObserver *observer_y = nullptr;
    dtkSliceActorObserver *observer_z = nullptr;

public:
    void setupSlices(void);

public slots:
    void onCanvasFocused(void);
};


void dtkVisualizationDecoratorSlicesPrivate::setupSlices(void)
{
    double *origin;
    origin = vtkImageData::SafeDownCast(this->volume_slice_z->GetInput())->GetOrigin();

    this->volume_slice_x->PlaceWidget();
    this->volume_slice_x->SetPlaneOrientationToXAxes();
    this->volume_slice_x->SetSlicePosition(origin[0]);

    this->volume_slice_y->PlaceWidget();
    this->volume_slice_y->SetPlaneOrientationToYAxes();
    this->volume_slice_y->SetSlicePosition(origin[1]);

    this->volume_slice_z->PlaceWidget();
    this->volume_slice_z->SetPlaneOrientationToZAxes();
    this->volume_slice_z->SetSlicePosition(origin[2]);

    this->volume_slice_x->On();
    this->volume_slice_x->InteractionOn();

    this->volume_slice_y->On();
    this->volume_slice_y->InteractionOn();

    this->volume_slice_z->On();
    this->volume_slice_z->InteractionOn();

    this->observer_x->origin = origin[0];
    this->observer_y->origin = origin[1];
    this->observer_z->origin = origin[2];
    }

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorSlices implementation
// ///////////////////////////////////////////////////////////////////

dtkVisualizationDecoratorSlices::dtkVisualizationDecoratorSlices(void): dtkVisualizationDecoratorWithClut(), d(new dtkVisualizationDecoratorSlicesPrivate())
{
    d->c2p_filter = vtkSmartPointer<vtkCellDataToPointData>::New();

    d->volume_slice_x = vtkImagePlaneWidget::New();
    d->volume_slice_y = vtkImagePlaneWidget::New();
    d->volume_slice_z = vtkImagePlaneWidget::New();

    d->show_actor_cb    = new QCheckBox;
    d->enable_slicing_x = new dtkVisualizationWidgetsSliceControls;
    d->enable_slicing_y = new dtkVisualizationWidgetsSliceControls;
    d->enable_slicing_z = new dtkVisualizationWidgetsSliceControls;

    d->observer_x = dtkSliceActorObserver::New();
    d->observer_x->control = d->enable_slicing_x;
    d->volume_slice_x->AddObserver(vtkCommand::InteractionEvent, d->observer_x);

    d->observer_y = dtkSliceActorObserver::New();
    d->observer_y->control = d->enable_slicing_y;
    d->volume_slice_y->AddObserver(vtkCommand::InteractionEvent, d->observer_y);

    d->observer_z = dtkSliceActorObserver::New();
    d->observer_z->control = d->enable_slicing_z;
    d->volume_slice_z->AddObserver(vtkCommand::InteractionEvent, d->observer_z);

    //////////
    // Inspectors connections

    connect(d->show_actor_cb, &QCheckBox::stateChanged, [=] (int state) {
            this->saveSettings("visibility", state == Qt::Checked);
            this->setVisibility(state == Qt::Checked);
            this->draw();
        });

    connect(d->enable_slicing_x, &dtkVisualizationWidgetsSliceControls::toggled, [=] (bool checked) {
        d->volume_slice_x->SetEnabled(checked);
        this->draw();
    });
    connect(d->enable_slicing_x, &dtkVisualizationWidgetsSliceControls::reset, [=] () {
            double *origin;
            origin = vtkImageData::SafeDownCast(d_func()->dataset)->GetOrigin();
            d->volume_slice_x->SetPlaneOrientationToXAxes();
            d->volume_slice_x->SetSlicePosition(origin[0]);
            this->draw();
    });
    connect(d->enable_slicing_x, &dtkVisualizationWidgetsSliceControls::valueChanged, [=] (double value) {
            double *origin;
            origin = vtkImageData::SafeDownCast(d_func()->dataset)->GetOrigin();
            d->volume_slice_x->SetSlicePosition(value + origin[0]);
            this->draw();
    });

    connect(d->enable_slicing_y, &dtkVisualizationWidgetsSliceControls::toggled, [=] (bool checked) {
        d->volume_slice_y->SetEnabled(checked);
        this->draw();
    });
    connect(d->enable_slicing_y, &dtkVisualizationWidgetsSliceControls::reset, [=] () {
            double *origin;
            origin = vtkImageData::SafeDownCast(d_func()->dataset)->GetOrigin();
            d->volume_slice_y->SetPlaneOrientationToYAxes();
            d->volume_slice_y->SetSlicePosition(origin[1]);
            this->draw();
    });
    connect(d->enable_slicing_y, &dtkVisualizationWidgetsSliceControls::valueChanged, [=] (double value) {
            double *origin;
            origin = vtkImageData::SafeDownCast(d_func()->dataset)->GetOrigin();
            d->volume_slice_y->SetSlicePosition(value + origin[1]);
            this->draw();
    });

    connect(d->enable_slicing_z, &dtkVisualizationWidgetsSliceControls::toggled, [=] (bool checked) {
        d->volume_slice_z->SetEnabled(checked);
        this->draw();
    });
    connect(d->enable_slicing_z, &dtkVisualizationWidgetsSliceControls::reset, [=] () {
            double *origin;
            origin = vtkImageData::SafeDownCast(d_func()->dataset)->GetOrigin();
            d->volume_slice_z->SetPlaneOrientationToZAxes();
            d->volume_slice_z->SetSlicePosition(origin[2]);
            this->draw();
    });
    connect(d->enable_slicing_z, &dtkVisualizationWidgetsSliceControls::valueChanged, [=] (double value) {
            double *origin;
            origin = vtkImageData::SafeDownCast(d_func()->dataset)->GetOrigin();
            d->volume_slice_z->SetSlicePosition((value + origin[2]));
            this->draw();
    });


    this->setObjectName("Slices");
    d->show_actor_cb->setObjectName("Display");
    d->enable_slicing_x->setObjectName("X");
    d->enable_slicing_y->setObjectName("Y");
    d->enable_slicing_z->setObjectName("Z");

    d_func()->inspectors << d->show_actor_cb << d->enable_slicing_x << d->enable_slicing_y << d->enable_slicing_z ;
}

dtkVisualizationDecoratorSlices::~dtkVisualizationDecoratorSlices(void)
{
    this->unsetCanvas();

    delete d;
    d = nullptr;
}

void dtkVisualizationDecoratorSlices::setData(const QVariant& data)
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
    d->c2p_filter->SetInputData(dataset);
    this->restoreSettings();

    d_func()->sortEligibleFields();
    this->setCurrentFieldName(d_func()->current_field_name);

    if (this->canvas()) {
        d->volume_slice_x->SetInteractor(d_func()->view->renderer()->GetRenderWindow()->GetInteractor());
        d->volume_slice_y->SetInteractor(d_func()->view->renderer()->GetRenderWindow()->GetInteractor());
        d->volume_slice_z->SetInteractor(d_func()->view->renderer()->GetRenderWindow()->GetInteractor());

        d->setupSlices();
    }

    d_func()->enableScalarBar();
}

void dtkVisualizationDecoratorSlices::setCanvas(dtkVisualizationCanvas *canvas)
{
    this->unsetCanvas();

    d_func()->view = dynamic_cast<dtkVisualizationView2D *>(canvas);
    if (!d_func()->view) {
        qWarning() << Q_FUNC_INFO << "View 2D or view 3D expected as canvas. Canvas is reset to nullptr.";
        return;
    }

    d->volume_slice_x->SetInteractor(d_func()->view->renderer()->GetRenderWindow()->GetInteractor());
    d->volume_slice_y->SetInteractor(d_func()->view->renderer()->GetRenderWindow()->GetInteractor());
    d->volume_slice_z->SetInteractor(d_func()->view->renderer()->GetRenderWindow()->GetInteractor());

    if (d->volume_slice_x->GetInput()) {
        d->setupSlices();
    }


    d_func()->enableScalarBar();
}

void dtkVisualizationDecoratorSlices::unsetCanvas(void)
{
    if (d_func()->view) {
        //        d_func()->view->renderer()->RemoveActor(d->actor);
    }

    d->volume_slice_x->SetInteractor(nullptr);
    d->volume_slice_y->SetInteractor(nullptr);
    d->volume_slice_z->SetInteractor(nullptr);

    d_func()->disableScalarBar();

    d_func()->view = nullptr;
}

void dtkVisualizationDecoratorSlices::setVisibility(bool visible)
{
    d->volume_slice_x->SetEnabled(visible);
    d->volume_slice_y->SetEnabled(visible);
    d->volume_slice_z->SetEnabled(visible);
    dtkVisualizationDecoratorWithClut::setVisibility(visible);
}

void dtkVisualizationDecoratorSlices::restoreSettings(void)
{
    QString name = this->objectName();
    if (name.isEmpty())
        return;

    dtkVisualizationDecoratorWithClut::restoreSettings();

    d->show_actor_cb->blockSignals(true);
    d->show_actor_cb->setChecked(d_func()->default_visibility);
    d->show_actor_cb->blockSignals(false);
}

void dtkVisualizationDecoratorSlices::touch(void)
{
    dtkVisualizationDecoratorWithClut::touch();

    vtkSmartPointer<vtkImageData> data = vtkImageData::SafeDownCast(d->c2p_filter->GetOutput());
    d->c2p_filter->Update();
    data->GetPointData()->SetActiveScalars(qPrintable(d_func()->current_field_name));

    d->volume_slice_x->SetInputData(data);
    d->volume_slice_y->SetInputData(data);
    d->volume_slice_z->SetInputData(data);

    d->volume_slice_x->SetSlicePosition(d->enable_slicing_x->value());
    d->volume_slice_y->SetSlicePosition(d->enable_slicing_y->value());
    d->volume_slice_z->SetSlicePosition(d->enable_slicing_z->value());

}

bool dtkVisualizationDecoratorSlices::setCurrentFieldName(const QString& field_name)
{
    if (!dtkVisualizationDecoratorWithClut::setCurrentFieldName(field_name)) {
        return false;
    }

    using Support = dtkVisualizationDecoratorWithClut::Support;
    int support = d_func()->supports[field_name];
     if(support == Support::Cell) {
         d->c2p_filter->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS, qPrintable(field_name));
         vtkSmartPointer<vtkImageData> data = vtkImageData::SafeDownCast(d->c2p_filter->GetOutput());
         d->c2p_filter->Update();
         data->GetPointData()->SetActiveScalars(qPrintable(field_name));

         d->volume_slice_x->SetInputData(data);
         d->volume_slice_y->SetInputData(data);
         d->volume_slice_z->SetInputData(data);
     } else {
         d->volume_slice_x->SetInputData(d_func()->dataset);
         d->volume_slice_y->SetInputData(d_func()->dataset);
         d->volume_slice_z->SetInputData(d_func()->dataset);
     }

     d->volume_slice_x->SetSlicePosition(d->enable_slicing_x->value());
     d->volume_slice_y->SetSlicePosition(d->enable_slicing_y->value());
     d->volume_slice_z->SetSlicePosition(d->enable_slicing_z->value());

     return true;
}

void dtkVisualizationDecoratorSlices::setColorMap(const QMap<double, QColor>& new_colormap)
{
    dtkVisualizationDecoratorWithClut::setColorMap(new_colormap);

    d->volume_slice_x->GetColorMap()->SetLookupTable(d_func()->color_function);
    d->volume_slice_y->GetColorMap()->SetLookupTable(d_func()->color_function);
    d->volume_slice_z->GetColorMap()->SetLookupTable(d_func()->color_function);
}

//
// dtkVisualizationDecoratorVolume.cpp ends here
