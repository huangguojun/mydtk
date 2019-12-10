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

#include "dtkVisualizationDecoratorDelaunay2D.h"

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
#include <vtkDelaunay2D.h>
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
// dtkVisualizationDecoratorDelaunay2DPrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationDecoratorDelaunay2DPrivate
{

public:
    vtkPoints *points = nullptr;
    dtkVisualizationView2D *view = nullptr;

public:
    QList<QWidget *> inspectors;

public:
    bool default_visibility;

public:
    QCheckBox *show_actor_cb = nullptr;
    QPushButton *color_button = nullptr;
    QDoubleSpinBox *opacity_spinbox = nullptr;

public:
    vtkSmartPointer<vtkActor> actor;
    vtkSmartPointer<vtkPolyData> data;
    vtkSmartPointer<vtkDelaunay2D> delaunay;
    vtkSmartPointer<vtkPolyDataMapper> mapper;
};

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorDelaunay2D implementation
// ///////////////////////////////////////////////////////////////////

dtkVisualizationDecoratorDelaunay2D::dtkVisualizationDecoratorDelaunay2D(void): d(new dtkVisualizationDecoratorDelaunay2DPrivate())
{
    this->setObjectName("Delaunay 2D");

    d->data = vtkSmartPointer<vtkPolyData>::New();

    d->delaunay = vtkSmartPointer< vtkDelaunay2D >::New();

    d->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    d->mapper->SetInputConnection(d->delaunay->GetOutputPort());

    d->mapper->SetColorModeToMapScalars();
    d->mapper->SetScalarVisibility(false);

    d->actor = vtkSmartPointer<vtkActor>::New();
    d->actor->SetMapper(d->mapper);
    d->actor->SetVisibility(d->default_visibility);
    d->actor->GetProperty()->SetPointSize(30);
    d->actor->GetProperty()->SetColor(0.5,0.5,0.5);

    //////////
    // Inspectors creation

    d->color_button    = new QPushButton("Color");
    d->opacity_spinbox = new QDoubleSpinBox;
    d->show_actor_cb   = new QCheckBox;

    d->opacity_spinbox->setObjectName("Opacity");
    d->opacity_spinbox->setRange(0, 1);
    d->opacity_spinbox->setDecimals(2);
    d->opacity_spinbox->setSingleStep(0.01);
    d->opacity_spinbox->setValue(1.);

    d->show_actor_cb->setObjectName("Display");
    d->show_actor_cb->setChecked(d->default_visibility);

    //////////
    // Inspectors connections

    connect(d->color_button, &QPushButton::released, [=] (void) {
        double *vtk_color = d->actor->GetProperty()->GetColor();
        QColor qt_color = QColor(vtk_color[0], vtk_color[1], vtk_color[2]);
        qt_color = QColorDialog::getColor(qt_color, d->color_button);
        this->setColor(qt_color);
    });

    connect(d->opacity_spinbox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &dtkVisualizationDecoratorDelaunay2D::setOpacity);

    connect(d->show_actor_cb, &QCheckBox::stateChanged, [=] (int state) {
            this->saveSettings("visibility",state == Qt::Checked);
            this->setVisibility(state == Qt::Checked);
            this->draw();
        });

    d->inspectors << d->color_button << d->opacity_spinbox << d->show_actor_cb;
}

dtkVisualizationDecoratorDelaunay2D::~dtkVisualizationDecoratorDelaunay2D(void)
{
    d->points = nullptr;
    d->view = nullptr;
    delete d;
    d = nullptr;
}

void dtkVisualizationDecoratorDelaunay2D::restoreSettings(void)
{
    QString name = this->objectName();
    if (name.isEmpty())
        return;

    QSettings settings;
    settings.beginGroup("canvas");
    d->default_visibility = settings.value(name+"_visibility", false).toBool();
    settings.endGroup();
}

void dtkVisualizationDecoratorDelaunay2D::touch(void)
{
    if(!this->canvas()) {
        dtkWarn() << Q_FUNC_INFO << "No canvas was set, call setCanvas to call draw on a canvas.";
        return;
    }

    this->draw();
}


bool dtkVisualizationDecoratorDelaunay2D::isDecorating(void)
{
    return d->points;
}

void dtkVisualizationDecoratorDelaunay2D::setData(const QVariant& data)
{
    d->points = data.value<vtkPoints *>();
    if (!d->points) {
        vtkPointSet *point_set = data.value<vtkPointSet *>();
        if (point_set) {
            d->points = point_set->GetPoints();
        } else {
            dtkWarn() << Q_FUNC_INFO << "vtkPoints or vtkPointSet are expected. Input data is not stored.";
            return;
        }
    }

    d->data->SetPoints(d->points);
    this->restoreSettings();

    d->delaunay->SetInputData(d->data);
    d->delaunay->Modified();

    d->mapper->SetInputConnection(d->delaunay->GetOutputPort());
    d->mapper->Modified();

    if(this->canvas()) {
        this->canvas()->renderer()->AddActor(d->actor);
    }
}

void dtkVisualizationDecoratorDelaunay2D::setCanvas(dtkVisualizationCanvas *canvas)
{
    this->unsetCanvas();

    d->view = dynamic_cast<dtkVisualizationView2D *>(canvas);
    if (!d->view) {
        qWarning() << Q_FUNC_INFO << "View 2D or view 3D expected as canvas. Canvas is reset to nullptr.";
        return;
    }

    if (d->delaunay->GetInput()) {
        d->view->renderer()->AddActor(d->actor);
    }
}

void dtkVisualizationDecoratorDelaunay2D::unsetCanvas(void)
{
    if (d->view) {
        d->view->renderer()->RemoveActor(d->actor);
    }
    d->view = nullptr;
}

void dtkVisualizationDecoratorDelaunay2D::setVisibility(bool visible)
{
    d->actor->SetVisibility(visible);

    d->show_actor_cb->blockSignals(true);
    d->show_actor_cb->setCheckState(visible ? Qt::Checked : Qt::Unchecked);
    d->show_actor_cb->blockSignals(false);
}

void dtkVisualizationDecoratorDelaunay2D::setColor(const QColor& color)
{
    d->actor->GetProperty()->SetColor(color.red(), color.green(), color.blue());

    this->draw();
}

void dtkVisualizationDecoratorDelaunay2D::setOpacity(const double& alpha)
{
    d->actor->GetProperty()->SetOpacity(alpha);

    d->opacity_spinbox->blockSignals(true);
    d->opacity_spinbox->setValue(alpha);
    d->opacity_spinbox->blockSignals(false);

    this->draw();
}

QVariant dtkVisualizationDecoratorDelaunay2D::data(void) const
{
    if (d->points) {
        return dtk::variantFromValue(d->points);

    } else {
        return QVariant();
    }
}

dtkVisualizationCanvas *dtkVisualizationDecoratorDelaunay2D::canvas(void) const
{
    return d->view;
}

QList<QWidget *> dtkVisualizationDecoratorDelaunay2D::inspectors(void) const
{
    return d->inspectors;
}

//
// dtkVisualizationDecoratorDelaunay2D.cpp ends here
