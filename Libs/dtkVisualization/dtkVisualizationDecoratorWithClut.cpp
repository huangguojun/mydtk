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

#include "dtkVisualizationDecoratorWithClut.h"
#include "dtkVisualizationDecoratorWithClut_p.h"
#include "dtkVisualizationMetaType.h"
#include "dtkVisualizationView2D.h"
#include <dtkVisualizationWidgets/dtkVisualizationWidgetsColorMapEditor.h>

#include <dtkLog>

#include <QtGui>
#include <QtWidgets>

#include <vtkCellData.h>
#include <vtkColorTransferFunction.h>
#include <vtkDataArray.h>
#include <vtkDataSet.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkScalarBarActor.h>
#include <vtkSmartPointer.h>

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorWithClutPrivate implementation
// ///////////////////////////////////////////////////////////////////

dtkVisualizationDecoratorWithClutPrivate::dtkVisualizationDecoratorWithClutPrivate(void)
{
    this->color_function = vtkSmartPointer<vtkColorTransferFunction>::New();

    this->scalar_bar = vtkSmartPointer<vtkScalarBarActor>::New();
    this->scalar_bar->SetWidth(0.08);
    this->scalar_bar->SetHeight(0.6);
    this->scalar_bar->SetVisibility(false);

    //////////
    // Inspectors creation

    this->fields_box = new QComboBox;
    this->fixed_range = new QCheckBox;
    this->min_range = new QLineEdit;
    this->max_range = new QLineEdit;

    this->colormap_editor = new dtkVisualizationWidgetsColorMapEditor;

    this->show_scalar_bar = new QCheckBox;

    QHBoxLayout *h_layout_range = new QHBoxLayout;
    QWidget *range_widget = new QWidget;

    //////////
    // Inspectors setup

    QDoubleValidator *double_validator = new QDoubleValidator;
    double_validator->setDecimals(9);
    this->min_range->setValidator(double_validator);
    this->max_range->setValidator(double_validator);

    h_layout_range->setAlignment(Qt::AlignLeft);
    h_layout_range->addWidget(new QLabel("Fixed Range:"));
    h_layout_range->addWidget(this->fixed_range);
    h_layout_range->addWidget(this->min_range);
    h_layout_range->addWidget(this->max_range);

    range_widget->setLayout(h_layout_range);

    this->show_scalar_bar->setChecked(false);

    this->fields_box->setObjectName("Field");
    range_widget->setObjectName("");
    this->colormap_editor->setObjectName("Color Map");
    this->show_scalar_bar->setObjectName("Scalar Bar");

    this->inspectors << this->fields_box << this->colormap_editor << range_widget
                     << this->show_scalar_bar;
}

void dtkVisualizationDecoratorWithClutPrivate::clear(void)
{
    this->current_field_name.clear();
    this->eligible_field_names.clear();
    this->color_transfer_functions.clear();
    this->opacity_functions.clear();
    this->ranges.clear();
    this->supports.clear();
}

void dtkVisualizationDecoratorWithClutPrivate::enableScalarBar(void)
{
    if (this->view && this->dataset) {
        this->view->addScalarBar(this->scalar_bar);
    }
}

void dtkVisualizationDecoratorWithClutPrivate::disableScalarBar(void)
{
    if (this->view) {
        this->view->removeScalarBar(this->scalar_bar);
    }
}

void dtkVisualizationDecoratorWithClutPrivate::retrieveScalarData(vtkDataSetAttributes *field_data,
                                                                  Support support)
{
    vtkIdType nb_arrays = field_data->GetNumberOfArrays();

    for (vtkIdType i = 0; i < nb_arrays; ++i) {
        std::size_t nb_components = field_data->GetArray(i)->GetNumberOfComponents();
        if (nb_components == 1) {
            QString field_name = QString::fromUtf8(field_data->GetArrayName(i));
            this->eligible_field_names << field_name;
            this->supports[field_name] = support;
            this->kinds[field_name] = Kind::Scalar;

            auto &&range = this->ranges[field_name];
            field_data->GetArray(i)->GetRange(range.data());
            this->color_transfer_functions[field_name] = this->default_color_map;

            auto opacity_function = vtkSmartPointer<vtkPiecewiseFunction>::New();
            opacity_function->RemoveAllPoints();
            opacity_function->AddPoint(0, 0.);
            opacity_function->AddPoint(255, 1.);

            this->opacity_functions[field_name] = opacity_function;
        }
    }
}

void dtkVisualizationDecoratorWithClutPrivate::retrieveVectorData(vtkDataSetAttributes *field_data,
                                                                  Support support)
{
    vtkIdType nb_arrays = field_data->GetNumberOfArrays();
    for (vtkIdType i = 0; i < nb_arrays; ++i) {
        std::size_t nb_components = field_data->GetArray(i)->GetNumberOfComponents();
        if (nb_components == 3) {
            QString field_name = QString::fromUtf8(field_data->GetArrayName(i));
            this->eligible_field_names << field_name;
            this->supports[field_name] = support;
            this->kinds[field_name] = Kind::Vector;

            auto &&range = this->ranges[field_name];
            field_data->GetArray(i)->GetRange(range.data(), -1);
            this->color_transfer_functions[field_name] = this->default_color_map;

            auto opacity_function = vtkSmartPointer<vtkPiecewiseFunction>::New();
            opacity_function->RemoveAllPoints();
            opacity_function->AddPoint(0, 0.);
            opacity_function->AddPoint(255, 1.);

            this->opacity_functions[field_name] = opacity_function;
        }
    }
}

void dtkVisualizationDecoratorWithClutPrivate::retrieveScalarPoints(vtkDataSet *dataset)
{
    vtkPointData *point_data = dataset->GetPointData();
    if (!point_data)
        return;

    this->retrieveScalarData(point_data, Support::Point);
}

void dtkVisualizationDecoratorWithClutPrivate::retrieveScalarCells(vtkDataSet *dataset)
{
    vtkCellData *cell_data = dataset->GetCellData();
    if (!cell_data)
        return;

    this->retrieveScalarData(cell_data, Support::Cell);
}

void dtkVisualizationDecoratorWithClutPrivate::retrieveVectorPoints(vtkDataSet *dataset)
{
    vtkPointData *point_data = dataset->GetPointData();
    if (!point_data)
        return;

    this->retrieveVectorData(point_data, Support::Point);
}

void dtkVisualizationDecoratorWithClutPrivate::retrieveVectorCells(vtkDataSet *dataset)
{
    vtkCellData *cell_data = dataset->GetCellData();
    if (!cell_data)
        return;

    this->retrieveVectorData(cell_data, Support::Cell);
}

void dtkVisualizationDecoratorWithClutPrivate::sortEligibleFields(void)
{
    if (this->eligible_field_names.size() > 0) {
        this->eligible_field_names.sort();

        if (this->fields_box->count() > 0) {
            this->fields_box->clear();
        }
        this->fields_box->blockSignals(true);
        this->fields_box->addItems(this->eligible_field_names);
        this->fields_box->blockSignals(false);
        this->current_field_name = this->default_field_name;
        if (this->default_field_name.isEmpty()
            || !this->eligible_field_names.contains(this->default_field_name)) {
            this->current_field_name = this->eligible_field_names.first();
        }
        this->fields_box->setCurrentText(this->current_field_name);
    }
}

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorWithClut implementation
// ///////////////////////////////////////////////////////////////////

dtkVisualizationDecoratorWithClut::dtkVisualizationDecoratorWithClut(void)
    : dtkVisualizationDecorator(), d(new dtkVisualizationDecoratorWithClutPrivate)
{
    //////////
    // Inspectors connections

    connect(d->fields_box, &QComboBox::currentTextChanged, [=](const QString &field_name) {
        this->setCurrentFieldName(field_name);
        this->saveSettings("field_name", field_name);
        this->touch();
    });

    connect(d->min_range, &QLineEdit::editingFinished, [=]() {
        d->fixed_range->blockSignals(true);
        d->fixed_range->setChecked(Qt::Checked);
        d->fixed_range->blockSignals(false);
        this->setCurrentRange(d->min_range->text().toDouble(), d->max_range->text().toDouble());
        this->saveSettings("range_min", d->min_range->text().toDouble());
        this->saveSettings("range_max", d->max_range->text().toDouble());
        this->touch();
    });

    connect(d->max_range, &QLineEdit::editingFinished, [=]() {
        d->fixed_range->blockSignals(true);
        d->fixed_range->setChecked(Qt::Checked);
        d->fixed_range->blockSignals(false);
        this->setCurrentRange(d->min_range->text().toDouble(), d->max_range->text().toDouble());
        this->saveSettings("range_min", d->min_range->text().toDouble());
        this->saveSettings("range_max", d->max_range->text().toDouble());
        this->touch();
    });

    // Restores default ranges when unchecked
    connect(d->fixed_range, &QCheckBox::stateChanged, [=](int state) {
        this->touch();
        this->saveSettings("fixed_range", state == Qt::Checked);
        this->draw();
    });

    connect(d->colormap_editor, &dtkVisualizationWidgetsColorMapEditor::valueChanged,
            [=](const QMap<double, QColor> &val) {
                d->color_transfer_functions[d->current_field_name] = d->colormap_editor->name();
                d->colormap_editor->blockSignals(true);
                this->setColorMap(val);
                this->saveSettings("colormap", d->colormap_editor->name());
                d->colormap_editor->blockSignals(false);
                this->draw();
            });

    connect(d->show_scalar_bar, &QCheckBox::stateChanged, [=](int state) {
        d->scalar_bar->SetVisibility(state == Qt::Checked);
        this->saveSettings("show_scalarbar", state == Qt::Checked);
        this->draw();
    });
}

void dtkVisualizationDecoratorWithClut::restoreSettings(void)
{
    QString name = this->objectName();
    if (name.isEmpty())
        return;

    QSettings settings;
    settings.beginGroup("canvas");
    d->default_visibility = settings.value(name + "_visibility", true).toBool();
    d->default_field_name = settings.value(name + "_field_name").toString();
    d->default_color_map = settings.value(name + "_colormap", "inferno").toString();
    d->default_show_scalar_bar = settings.value(name + "_show_scalarbar", false).toBool();
    double range_min = settings.value(name + "_range_min", -1.).toDouble();
    double range_max = settings.value(name + "_range_max", -1.).toDouble();
    bool fixed_range = settings.value(name + "_fixed_range", false).toBool();
    settings.endGroup();

    if (fixed_range && !d->default_field_name.isEmpty() && range_min != -1.0 && range_max != -1.0) {
        this->setCurrentRange(range_min, range_max);
    }

    d->fixed_range->setChecked(fixed_range);
    d->colormap_editor->blockSignals(true);
    d->colormap_editor->setValue(d->default_color_map);
    d->colormap_editor->blockSignals(false);
    this->setColorMap(d->colormap_editor->value());

    d->show_scalar_bar->blockSignals(true);
    d->show_scalar_bar->setChecked(d->default_show_scalar_bar);
    d->show_scalar_bar->blockSignals(false);
    if (d->default_visibility) {
        d->scalar_bar->SetVisibility(d->default_show_scalar_bar);
    }
}

QString dtkVisualizationDecoratorWithClut::fieldName(void) const
{
    return d->current_field_name;
}

void dtkVisualizationDecoratorWithClut::touch(void)
{
    if (d->current_field_name.isEmpty() || !d->dataset) {
        return;
    }

    // Update range
    auto &&range = d->ranges[d->current_field_name];
    if (!d->fixed_range->isChecked()) {
        using Support = dtkVisualizationDecoratorWithClutPrivate::Support;
        switch (d->supports[d->current_field_name]) {
        case Support::Point:
            d->dataset->GetPointData()->GetArray(qPrintable(d->current_field_name))->Modified();
            d->dataset->GetPointData()
                    ->GetArray(qPrintable(d->current_field_name))
                    ->GetRange(range.data(), -1);
            break;
        case Support::Cell:
            d->dataset->GetCellData()->GetArray(qPrintable(d->current_field_name))->Modified();
            d->dataset->GetCellData()
                    ->GetArray(qPrintable(d->current_field_name))
                    ->GetRange(range.data(), -1);
            break;
        default:
            return;
        }
    }
    d->min_range->blockSignals(true);
    d->min_range->setText(QString::number(range[0]));
    d->min_range->blockSignals(false);

    d->max_range->blockSignals(true);
    d->max_range->setText(QString::number(range[1]));
    d->max_range->blockSignals(false);

    d->colormap_editor->blockSignals(true);
    d->colormap_editor->setValue(d->color_transfer_functions[d->current_field_name]);
    d->colormap_editor->blockSignals(false);
    this->setColorMap(d->colormap_editor->value());
}

void dtkVisualizationDecoratorWithClut::setVisibility(bool visible)
{
    d->show_scalar_bar->setEnabled(visible);
    d->scalar_bar->SetVisibility(visible && d->show_scalar_bar->isChecked()
                                 && d->show_scalar_bar->isEnabled());
}

dtkVisualizationDecoratorWithClut::~dtkVisualizationDecoratorWithClut(void)
{
    d->clear();
    delete d;
    d = nullptr;
}

bool dtkVisualizationDecoratorWithClut::setCurrentFieldName(const QString &field_name)
{
    if (field_name.isEmpty()) {
        dtkWarn() << Q_FUNC_INFO << "Field name is empty, nothing is done.";
        return false;
    }

    if (!d->dataset) {
        dtkWarn() << Q_FUNC_INFO
                  << "Before calling setCurrentFieldName, setDataSet must be called.";
        return false;
    }

    if (!d->eligible_field_names.contains(field_name)) {
        dtkWarn() << Q_FUNC_INFO << "The field name :" << field_name
                  << "that was specified doesn't match any of the eligible "
                     "scalar field names";

        return false;
    }

    d->current_field_name = field_name;

    d->fields_box->blockSignals(true);
    d->fields_box->setCurrentText(d->current_field_name);
    d->fields_box->blockSignals(false);

    d->scalar_bar->UnconstrainedFontSizeOn();
    d->scalar_bar->SetTitle(qPrintable(this->objectName() + "/" + d->current_field_name));
    d->scalar_bar->Modified();

    this->touch();

    return true;
}

void dtkVisualizationDecoratorWithClut::setCurrentRange(double min, double max)
{
    if (d->current_field_name.isEmpty() && d->default_field_name.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "Field not selected";
        return;
    }
    if (min >= max) {
        qWarning() << Q_FUNC_INFO << " min >= max : " << min << max;
        return;
    }
    QString field_name =
            d->current_field_name.isEmpty() ? d->default_field_name : d->current_field_name;
    auto &&range = d->ranges[field_name];
    range[0] = min;
    range[1] = max;

    this->touch();
}

void dtkVisualizationDecoratorWithClut::setColorMap(const QMap<double, QColor> &new_colormap)
{
    QMap<double, QColor> colormap_map = new_colormap;

    // If there is no new colormap, take the previous one.
    if (new_colormap.isEmpty()) {
        QString colormap_name = d->color_transfer_functions[d->current_field_name];
        d->colormap_editor->setValue(colormap_name);
        colormap_map = d->colormap_editor->value();
    }

    // Recomputes the vtkColorMap based on range.
    auto &&range = d->ranges[d->current_field_name];

    d->color_function->RemoveAllPoints();
    for (auto it = colormap_map.cbegin(); it != colormap_map.cend(); ++it) {
        auto &&val = it.key();
        auto &&color = it.value();
        double node = val * range[1] + (1 - val) * range[0];
        d->color_function->AddRGBPoint(node, color.red() / 255., color.green() / 255.,
                                       color.blue() / 255.);
    }

    d->colormap_editor->setValue(new_colormap);

    d->color_function->ClampingOn();
    d->color_function->Modified();

    d->scalar_bar->SetLookupTable(d->color_function);
    d->scalar_bar->Modified();
}

bool dtkVisualizationDecoratorWithClut::isDecorating(void)
{
    return d->eligible_field_names.size();
}

QVariant dtkVisualizationDecoratorWithClut::data(void) const
{
    if (d->dataset) {
        return dtk::variantFromValue(d->dataset);

    } else {
        return QVariant();
    }
}

dtkVisualizationCanvas *dtkVisualizationDecoratorWithClut::canvas(void) const
{
    return d->view;
}

QList<QWidget *> dtkVisualizationDecoratorWithClut::inspectors(void) const
{
    return d->inspectors;
}

dtkVisualizationDecoratorWithClutPrivate *dtkVisualizationDecoratorWithClut::d_func(void)
{
    return d;
}

const dtkVisualizationDecoratorWithClutPrivate *
dtkVisualizationDecoratorWithClut::d_func(void) const
{
    return d;
}

//
// dtkVisualizationDecoratorWithClut.cpp ends here
