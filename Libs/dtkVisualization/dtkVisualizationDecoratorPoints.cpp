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

#include "dtkVisualizationDecoratorPoints.h"

#include "dtkVisualizationMetaType.h"
#include "dtkVisualizationView2D.h"

#include <dtkLog>

#include <QtGui>
#include <QtWidgets>

#include <vtkActor.h>
#include <vtkColorTransferFunction.h>
#include <vtkLookupTable.h>
#include <vtkCellData.h>
#include <vtkDataSet.h>
#include <vtkDataSetMapper.h>
#include <vtkLookupTable.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkVertexGlyphFilter.h>

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorPointsPrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationDecoratorPointsPrivate
{
public:
    vtkPoints *points = nullptr;
    dtkVisualizationView2D *view = nullptr;

public:
    QList<QWidget *> inspectors;

public:
    QCheckBox *show_actor_cb = nullptr;
    QPushButton *color_button = nullptr;
    QDoubleSpinBox *opacity_spinbox = nullptr;
    QDoubleSpinBox *size_spinbox = nullptr;

public:
    vtkSmartPointer<vtkActor> actor;
    vtkSmartPointer<vtkPolyData> data;
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkVertexGlyphFilter> filter;
};

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorPoints implementation
// ///////////////////////////////////////////////////////////////////

dtkVisualizationDecoratorPoints::dtkVisualizationDecoratorPoints(void): d(new dtkVisualizationDecoratorPointsPrivate())
{
    d->data = vtkSmartPointer<vtkPolyData>::New();

    d->filter = vtkSmartPointer<vtkVertexGlyphFilter>::New();

    d->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    d->mapper->SetInputConnection(d->filter->GetOutputPort());

    d->mapper->SetColorModeToMapScalars();
    d->mapper->SetScalarVisibility(true);

    d->actor = vtkSmartPointer<vtkActor>::New();
    d->actor->SetMapper(d->mapper);
    d->actor->SetVisibility(false);
    d->actor->GetProperty()->SetPointSize(1.);
    d->actor->GetProperty()->SetColor(0.5, 0.5, 0.5);

    //////////
    // Inspectors creation

    d->color_button    = new QPushButton("Color");
    d->opacity_spinbox = new QDoubleSpinBox;
    d->size_spinbox    = new QDoubleSpinBox;
    d->show_actor_cb   = new QCheckBox;

    d->opacity_spinbox->setObjectName("Opacity");
    d->opacity_spinbox->setRange(0, 1);
    d->opacity_spinbox->setDecimals(2);
    d->opacity_spinbox->setSingleStep(0.01);
    d->opacity_spinbox->setValue(1.);

    d->size_spinbox->setObjectName("Size");
    d->size_spinbox->setRange(0, std::numeric_limits<double>::infinity());
    d->size_spinbox->setDecimals(2);
    d->size_spinbox->setSingleStep(0.01);
    d->size_spinbox->setValue(1.);

    this->setObjectName("Points");

    d->show_actor_cb->setObjectName("Display");
    d->show_actor_cb->setChecked(false);

    //////////
    // Inspectors connections

    connect(d->color_button, &QPushButton::released, [=] (void) {
        double *vtk_color = d->actor->GetProperty()->GetColor();
        QColor qt_color = QColor(vtk_color[0], vtk_color[1], vtk_color[2]);
        qt_color = QColorDialog::getColor(qt_color, d->color_button);
        this->setColor(qt_color);
    });

    connect(d->opacity_spinbox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &dtkVisualizationDecoratorPoints::setOpacity);

    connect(d->size_spinbox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &dtkVisualizationDecoratorPoints::setSize);

    connect(d->show_actor_cb, &QCheckBox::stateChanged, [=] (int state) {
            this->setVisibility(state == Qt::Checked);
            this->draw();
        });

    d->inspectors << d->color_button << d->opacity_spinbox << d->size_spinbox << d->show_actor_cb;
}

dtkVisualizationDecoratorPoints::~dtkVisualizationDecoratorPoints(void)
{
    d->points = nullptr;
    d->view = nullptr;
    delete d;
    d = nullptr;
}

void dtkVisualizationDecoratorPoints::touch(void)
{
    if(!this->canvas()) {
        dtkWarn() << Q_FUNC_INFO << "No canvas was set, call setCanvas to call draw on a canvas.";
        return;
    }

    Q_ASSERT(this->canvas()->renderer());

    if (this->canvas()->interactor()) {
        this->canvas()->interactor()->Render();
    }
}

bool dtkVisualizationDecoratorPoints::isDecorating(void)
{
    return d->points;
}

void dtkVisualizationDecoratorPoints::setData(const QVariant& data)
{
    d->points = data.value<vtkPoints *>();
    if (!d->points) {
        vtkPointSet *point_set = data.value<vtkPointSet *>();
        if (point_set) {
            d->points = point_set->GetPoints();
        } else {
            dtkWarn() << Q_FUNC_INFO << "vtkPoint is expected. Input data is not stored.";
            return;
        }
    }

    d->data->SetPoints(d->points);

    d->filter->SetInputData(d->data);

    d->mapper->SetInputConnection(d->filter->GetOutputPort());

    d->filter->Modified();
    d->mapper->Modified();

    this->draw();
}

void dtkVisualizationDecoratorPoints::setCanvas(dtkVisualizationCanvas *canvas)
{
    this->unsetCanvas();

    d->view = dynamic_cast<dtkVisualizationView2D *>(canvas);
    if (!d->view) {
        qWarning() << Q_FUNC_INFO << "View 2D or view 3D expected as canvas. Canvas is reset to nullptr.";
        return;
    }

    if (d->filter->GetInput()) {
        d->view->renderer()->AddActor(d->actor);

        if(d->view->interactor()) {
            d->view->interactor()->Render();
        }
    }
}

void dtkVisualizationDecoratorPoints::unsetCanvas(void)
{
    if (d->view) {
        d->view->renderer()->RemoveActor(d->actor);
    }
    d->view = nullptr;
}

void dtkVisualizationDecoratorPoints::setVisibility(bool visible)
{
    d->actor->SetVisibility(visible);

    d->show_actor_cb->blockSignals(true);
    d->show_actor_cb->setCheckState(visible ? Qt::Checked : Qt::Unchecked);
    d->show_actor_cb->blockSignals(false);

}

void dtkVisualizationDecoratorPoints::setColor(const QColor& color)
{
    d->actor->GetProperty()->SetColor(color.red(), color.green(), color.blue());

    this->draw();
}

void dtkVisualizationDecoratorPoints::setOpacity(const double& alpha)
{
    d->actor->GetProperty()->SetOpacity(alpha);

    d->opacity_spinbox->blockSignals(true);
    d->opacity_spinbox->setValue(alpha);
    d->opacity_spinbox->blockSignals(false);

    this->draw();
}

void dtkVisualizationDecoratorPoints::setSize(const double& size)
{
    d->actor->GetProperty()->SetPointSize(size);

    d->size_spinbox->blockSignals(true);
    d->size_spinbox->setValue(size);
    d->size_spinbox->blockSignals(false);

    this->draw();
}

QVariant dtkVisualizationDecoratorPoints::data(void) const
{
    if (d->points) {
        return dtk::variantFromValue(d->points);

    } else {
        return QVariant();
    }
}

dtkVisualizationCanvas *dtkVisualizationDecoratorPoints::canvas(void) const
{
    return d->view;
}

QList<QWidget *> dtkVisualizationDecoratorPoints::inspectors(void) const
{
    return d->inspectors;
}

//
// dtkVisualizationDecoratorPoints.cpp ends here
