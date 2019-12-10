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

#include "dtkVisualizationDecoratorScalarGlyphs.h"

#include "dtkVisualizationDecoratorWithClut_p.h"

#include "dtkVisualizationMetaType.h"

#include "dtkVisualizationView2D.h"

#include <dtkLog>

#include <QtGui>
#include <QtWidgets>

#include <vtkActor.h>
#include <vtkArrowSource.h>
#include <vtkCellCenters.h>
#include <vtkCellData.h>
#include <vtkColorTransferFunction.h>
#include <vtkCubeSource.h>
#include <vtkCylinderSource.h>
#include <vtkDataSet.h>
#include <vtkGlyph3D.h>
#include <vtkLookupTable.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPlaneSource.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRegularPolygonSource.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkTextProperty.h>

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorScalarGlyphsPrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationDecoratorScalarGlyphsPrivate
{
public:
    vtkSmartPointer<vtkActor> actor;
    vtkSmartPointer<vtkPolyDataMapper> mapper;

public:
    vtkSmartPointer<vtkArrowSource> source_arrow;
    vtkSmartPointer<vtkCubeSource> source_cube;
    vtkSmartPointer<vtkCylinderSource> source_cylinder;
    vtkSmartPointer<vtkSphereSource> source_sphere;
    vtkSmartPointer<vtkGlyph3D> glyphs;
    vtkSmartPointer<vtkRegularPolygonSource> source_polygon;
    vtkSmartPointer<vtkCellCenters> cell_centers;

public:
    QCheckBox *show_actor_cb = nullptr;
    QDoubleSpinBox *glyphs_size_sb = nullptr;
    QComboBox *glyphs_source_cb = nullptr;

public:
    double default_glyph_size;
    QString default_glyph_shape ;
    double size_multiplier = 1.;
    QHash<QString, double> glyphs_sizes;
    QHash<QString, QString> glyphs_sources;
};

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorScalarGlyphs implementation
// ///////////////////////////////////////////////////////////////////

dtkVisualizationDecoratorScalarGlyphs::dtkVisualizationDecoratorScalarGlyphs(void): dtkVisualizationDecoratorWithClut(), d(new dtkVisualizationDecoratorScalarGlyphsPrivate())
{
    d->source_arrow    = vtkSmartPointer<vtkArrowSource>::New();
    d->source_cube     = vtkSmartPointer<vtkCubeSource>::New();
    d->source_cylinder = vtkSmartPointer<vtkCylinderSource>::New();
    d->source_polygon  = vtkSmartPointer<vtkRegularPolygonSource>::New();
    d->source_sphere   = vtkSmartPointer<vtkSphereSource>::New();

    d->source_sphere->SetThetaResolution(12.);
    d->source_sphere->SetPhiResolution(12.);
    d->source_sphere->Update();

    d->source_arrow->SetShaftRadius(d->default_glyph_size);
    d->source_arrow->SetTipRadius(d->default_glyph_size);
    d->source_arrow->Update();

    d->source_cube->SetXLength(d->default_glyph_size);
    d->source_cube->SetYLength(d->default_glyph_size);
    d->source_cube->SetZLength(d->default_glyph_size);
    d->source_cube->Update();

    d->source_cylinder->SetRadius(d->default_glyph_size);
    d->source_cylinder->Update();

    d->source_polygon->SetRadius(d->default_glyph_size);
    d->source_polygon->Update();

    d->source_sphere->SetRadius(d->default_glyph_size);
    d->source_sphere->Update();

    d->cell_centers = vtkSmartPointer<vtkCellCenters>::New();

    d->glyphs = vtkSmartPointer<vtkGlyph3D>::New();
    d->glyphs->SetSourceData(d->source_sphere->GetOutput());
    d->glyphs->SetScaleModeToDataScalingOff();
    d->glyphs->SetColorModeToColorByScalar();
    d->glyphs->GeneratePointIdsOn();

    d->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    d->mapper->SetInputConnection(d->glyphs->GetOutputPort());
    d->mapper->SetColorModeToMapScalars();
    d->mapper->SetInterpolateScalarsBeforeMapping(true);
    d->mapper->SetScalarModeToUsePointFieldData();

    d->actor = vtkSmartPointer<vtkActor>::New();
    d->actor->SetMapper(d->mapper);

    //////////
    // Inspectors creation

    d->show_actor_cb = new QCheckBox;
    d->glyphs_size_sb = new QDoubleSpinBox;
    d->glyphs_source_cb = new QComboBox;

    //////////
    // Inspectors setup

    d->glyphs_size_sb->setMaximum(1000);
    d->glyphs_size_sb->setKeyboardTracking(false);

    d->glyphs_source_cb->addItem("Arrow");
    d->glyphs_source_cb->addItem("Cube");
    d->glyphs_source_cb->addItem("Cylinder");
    d->glyphs_source_cb->addItem("Sphere");
    d->glyphs_source_cb->addItem("Polygon");

    //////////
    // Inspectors connections

    connect(d->show_actor_cb, &QCheckBox::stateChanged, [=] (int state) {
            this->saveSettings("visibility",state == Qt::Checked);
            this->setVisibility(state == Qt::Checked);
            this->draw();
        });

    connect(d->glyphs_size_sb, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [=] (double value) {
            this->blockSignals(true);
            this->saveSettings("glyph_size", value);
            this->setCurrentGlyphsSize(value);
            this->blockSignals(false);
            this->draw();
        });

    connect(this, &dtkVisualizationDecoratorScalarGlyphs::currentGlyphsSizeChanged, [=] (double value) {
            d->glyphs_size_sb->blockSignals(true);
            d->glyphs_size_sb->setValue(value);
            d->glyphs_size_sb->blockSignals(false);
            this->draw();
        });

    connect(d->glyphs_source_cb, &QComboBox::currentTextChanged, [=] (const QString& source) {
            this->blockSignals(true);
            this->saveSettings("glyph_shape", source);
            this->setCurrentGlyphsSource(source);
            this->blockSignals(false);
            this->draw();
        });

    connect(this, &dtkVisualizationDecoratorScalarGlyphs::currentGlyphsSourceChanged, [=] (const QString& source) {
            d->glyphs_source_cb->blockSignals(true);
            int index = d->glyphs_source_cb->findText(source);
            if ( index != -1 ) {
                d->glyphs_source_cb->setCurrentIndex(index);
            }
            d->glyphs_source_cb->blockSignals(false);
            this->draw();
        });

    this->setObjectName("Scalar Glyphs");
    d->show_actor_cb->setObjectName("Display");
    d->glyphs_size_sb->setObjectName("Glyphs Size");
    d->glyphs_source_cb->setObjectName("Source Shape");

    d_func()->inspectors << d->show_actor_cb << d->glyphs_size_sb << d->glyphs_source_cb;
}

dtkVisualizationDecoratorScalarGlyphs::~dtkVisualizationDecoratorScalarGlyphs(void)
{
    dtkVisualizationDecoratorScalarGlyphs::unsetCanvas();

    delete d;
    d = nullptr;
}

void dtkVisualizationDecoratorScalarGlyphs::restoreSettings(void)
{
    QString name = this->objectName();
    if (name.isEmpty())
        return;

    dtkVisualizationDecoratorWithClut::restoreSettings();

    QSettings settings;
    settings.beginGroup("canvas");
    d->default_glyph_size  = settings.value(name+"_glyph_size", 1.0).toDouble();
    d->default_glyph_shape = settings.value(name+"_glyph_shape", "Sphere").toString();
    settings.endGroup();

    d->actor->SetVisibility(d_func()->default_visibility);
    d->show_actor_cb->blockSignals(true);
    d->show_actor_cb->setChecked(d_func()->default_visibility);
    d->show_actor_cb->blockSignals(false);
}

void dtkVisualizationDecoratorScalarGlyphs::setData(const QVariant& data)
{
    vtkDataSet *dataset = data.value<vtkDataSet *>();
    if (!dataset) {
        dtkWarn() << Q_FUNC_INFO << "vtkDataSet is expected. Input data is not stored.";
        return;
    }

    //size_multiplier to be able to see glyph for big datasets
    // from 1 to 10 % of max_dim

    double bounds[6];
    dataset->GetBounds(bounds);
    double max_dim = bounds[1] - bounds[0];
    max_dim = std::max(max_dim, bounds[3] - bounds[2]);
    max_dim = std::max(max_dim, bounds[5] - bounds[4]);

    d->size_multiplier = max_dim/100;

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

    d->cell_centers->SetInputData(dataset);

    d->glyphs_sizes.clear();
    d->glyphs_sources.clear();
    for(auto field_name : d_func()->eligible_field_names) {
            d->glyphs_sizes[field_name]   = d->default_glyph_size;
            d->glyphs_sources[field_name] = d->default_glyph_shape;
    }
    emit this->currentGlyphsSizeChanged(d->default_glyph_size);
    emit this->currentGlyphsSourceChanged(d->default_glyph_shape);

    d_func()->sortEligibleFields();
    this->setCurrentFieldName(d_func()->current_field_name);

    if (this->canvas()) {
        this->canvas()->renderer()->AddActor(d->actor);
    }

    d_func()->enableScalarBar();
}

void dtkVisualizationDecoratorScalarGlyphs::setCanvas(dtkVisualizationCanvas *canvas)
{
    this->unsetCanvas();

    d_func()->view = dynamic_cast<dtkVisualizationView2D *>(canvas);
    if (!d_func()->view) {
        qWarning() << Q_FUNC_INFO << "View 2D or view 3D expected as canvas. Canvas is reset to nullptr.";
        return;
    }

    if (d->glyphs->GetInput()) {
        d_func()->view->renderer()->AddActor(d->actor);
    }

    d_func()->enableScalarBar();
}

void dtkVisualizationDecoratorScalarGlyphs::unsetCanvas(void)
{
    if (d_func()->view) {
        d_func()->view->renderer()->RemoveActor(d->actor);
    }

    d_func()->disableScalarBar();

    d_func()->view = nullptr;
}

void dtkVisualizationDecoratorScalarGlyphs::setVisibility(bool visible)
{
    d->actor->SetVisibility(visible);
    dtkVisualizationDecoratorWithClut::setVisibility(visible);
}

void dtkVisualizationDecoratorScalarGlyphs::setCurrentGlyphsSize(double size)
{
    QString& current_field_name = d_func()->current_field_name;

    if (current_field_name.isEmpty()) {
        return;
    }

    d->glyphs_sizes[current_field_name] = size;

    double scaled_size = size*d->size_multiplier;
    QString source = d->glyphs_sources[current_field_name];
    if(source == "Arrow") {
        d->source_arrow->SetShaftRadius(scaled_size);
        d->source_arrow->SetTipRadius(scaled_size);
        d->source_arrow->Modified();
    } else if (source == "Cube") {
        d->source_cube->SetXLength(scaled_size);
        d->source_cube->SetYLength(scaled_size);
        d->source_cube->SetZLength(scaled_size);
        d->source_cube->Modified();
    } else if (source == "Cylinder") {
        d->source_cylinder->SetRadius(scaled_size);
        d->source_cylinder->Modified();
    } else if (source == "Sphere") {
        d->source_sphere->SetRadius(scaled_size);
        d->source_sphere->Modified();
    } else if (source == "Polygon") {
        d->source_polygon->SetRadius(scaled_size);
        d->source_polygon->Modified();
    }

    d->glyphs->Modified();

    emit this->currentGlyphsSizeChanged(size);
}

void dtkVisualizationDecoratorScalarGlyphs::setCurrentGlyphsSource(const QString &source)
{
    QString& current_field_name = d_func()->current_field_name;

    if (current_field_name.isEmpty()) {
        return;
    }


    double scaled_size = d->glyphs_size_sb->value() * d->size_multiplier;

    if(source == "Arrow") {
        d->source_arrow->SetShaftRadius(scaled_size);
        d->source_arrow->SetTipRadius(scaled_size);
        d->source_arrow->Modified();
        d->glyphs->SetSourceConnection(d->source_arrow->GetOutputPort());
    } else if (source == "Cube") {
        d->source_cube->SetXLength(scaled_size);
        d->source_cube->SetYLength(scaled_size);
        d->source_cube->SetZLength(scaled_size);
        d->source_cube->Modified();
        d->glyphs->SetSourceConnection(d->source_cube->GetOutputPort());
    } else if (source == "Cylinder") {
        d->source_cylinder->SetRadius(scaled_size);
        d->source_cylinder->Modified();
        d->glyphs->SetSourceConnection(d->source_cylinder->GetOutputPort());
    } else if (source == "Sphere") {
        d->source_sphere->SetRadius(scaled_size);
        d->source_sphere->Modified();
        d->glyphs->SetSourceConnection(d->source_sphere->GetOutputPort());
    } else if (source == "Polygon") {
        d->source_polygon->SetRadius(scaled_size);
        d->source_polygon->Modified();
        d->glyphs->SetSourceConnection(d->source_polygon->GetOutputPort());
    }

    d->glyphs_sources[current_field_name] = source;
    d->glyphs->Modified();

    emit this->currentGlyphsSourceChanged(source);
}

bool dtkVisualizationDecoratorScalarGlyphs::setCurrentFieldName(const QString& field_name)
{
    if (!dtkVisualizationDecoratorWithClut::setCurrentFieldName(field_name)) {
        return false;
    }

    double glyphs_size = d->glyphs_sizes[field_name];
    QString glyphs_source = d->glyphs_sources[field_name];

    double scaled_size = glyphs_size * d->size_multiplier;
    if(glyphs_source == "Arrow") {
        d->source_arrow->SetShaftRadius(scaled_size);
        d->source_arrow->SetTipRadius(scaled_size);
        d->glyphs->SetSourceConnection(d->source_arrow->GetOutputPort());
        d->source_arrow->Modified();
    } else if (glyphs_source == "Cube") {
        d->source_cube->SetXLength(scaled_size);
        d->source_cube->SetYLength(scaled_size);
        d->source_cube->SetZLength(scaled_size);
        d->glyphs->SetSourceConnection(d->source_cube->GetOutputPort());
        d->source_cube->Modified();
    } else if (glyphs_source == "Cylinder") {
        d->source_cylinder->SetRadius(scaled_size);
        d->glyphs->SetSourceConnection(d->source_cylinder->GetOutputPort());
        d->source_cylinder->Modified();
    } else if (glyphs_source == "Sphere") {
        d->source_sphere->SetRadius(scaled_size);
        d->glyphs->SetSourceConnection(d->source_sphere->GetOutputPort());
        d->source_sphere->Modified();
    } else if (glyphs_source == "Polygon") {
        d->source_polygon->SetRadius(scaled_size);
        d->glyphs->SetSourceConnection(d->source_polygon->GetOutputPort());
        d->source_polygon->Modified();
    }

    using Support = dtkVisualizationDecoratorWithClut::Support;
    int support = d_func()->supports[field_name];
    if(support == Support::Point) {
        d->glyphs->SetInputData(d_func()->dataset);
    } else if(support == Support::Cell) {
        d->glyphs->SetInputConnection(d->cell_centers->GetOutputPort());
    }
    d->glyphs->Modified();

    d->mapper->SelectColorArray(qPrintable(field_name));
    d->mapper->Modified();

    emit this->currentGlyphsSizeChanged(glyphs_size);
    emit this->currentGlyphsSourceChanged(glyphs_source);

    return true;
}

void dtkVisualizationDecoratorScalarGlyphs::setColorMap(const QMap<double, QColor>& new_colormap)
{
    dtkVisualizationDecoratorWithClut::setColorMap(new_colormap);

    d->mapper->SetLookupTable(d_func()->color_function);
    d->mapper->SelectColorArray(qPrintable(d_func()->current_field_name));
    auto&& range = d_func()->ranges[d_func()->current_field_name];
    d->mapper->SetScalarRange(range[0], range[1]);
    d->mapper->Modified();

    // Set opacity
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

void *dtkVisualizationDecoratorScalarGlyphs::glyphs(void)
{
    return d->glyphs;
}

void *dtkVisualizationDecoratorScalarGlyphs::actor(void)
{
    return d->actor;
}

void dtkVisualizationDecoratorScalarGlyphs::touch(void)
{
    dtkVisualizationDecoratorWithClut::touch();

    if (!d_func()->dataset)
        return;

    d->glyphs->SetInputData(d_func()->dataset);
    d->glyphs->Update();
}

//
// dtkVisualizationDecoratorScalarGlyphs.cpp ends here
