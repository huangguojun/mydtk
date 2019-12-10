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

#include "dtkVisualizationDecoratorClutEditorSimple.h"

#include "dtkVisualizationMetaType.h"
#include "dtkVisualizationView2D.h"

#include <dtkVisualizationWidgets/dtkVisualizationWidgetsColorMapEditor>

#include <dtkLog>

#include <QtGui>
#include <QtWidgets>

#include <vtkCellData.h>
#include <vtkColorTransferFunction.h>
#include <vtkDataSet.h>
#include <vtkLookupTable.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkScalarBarActor.h>

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorClutEditorSimplePrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationDecoratorClutEditorSimplePrivate
{
public:
    enum Support {
        Unknown, //so that if not specified, it is unknown per defaut
        Point,
        Cell
    };

public:
    void updateColormap(const QMap<double, QColor>& = QMap<double, QColor>());

public:
    QStringList eligible_field_names;
    QString current_field_name;

public:
    vtkDataSet *dataset = nullptr;
    dtkVisualizationView2D *view = nullptr;

public:
    QList<QWidget *> inspectors;

public:
    dtkVisualizationWidgetsColorMapEditor *colormap_editor;

    vtkSmartPointer<vtkColorTransferFunction> color_function;
    vtkSmartPointer<vtkPiecewiseFunction>     opacity;
    vtkSmartPointer<vtkScalarBarActor>        scalar_bar;

public:
    QHash<QString, QString> color_transfer_functions; // <field; colormap_name>
    QHash<QString, vtkSmartPointer<vtkPiecewiseFunction>>     opacity_functions;
    QHash<QString, QPair<double, double>>                     ranges;
    QHash<QString, Support>                                   supports;

public:
    QCheckBox *display_scalarbar = nullptr;
    QComboBox *fields_box = nullptr;
    QCheckBox *fixed_range = nullptr;
    QLineEdit *min_range = nullptr;
    QLineEdit *max_range = nullptr;
};


void dtkVisualizationDecoratorClutEditorSimplePrivate::updateColormap(const QMap<double, QColor>& new_colormap)
{
    QMap<double, QColor> colormap_map = new_colormap;

    //if there is no new colormap, take the previous one
    if(new_colormap.isEmpty()) {
        QString colormap_name = this->color_transfer_functions[this->current_field_name];
        this->colormap_editor->setValue(colormap_name);
        colormap_map = this->colormap_editor->value();
    }

    // recompute the vtkColorMap based on range;
    QPair<double, double> range = this->ranges[this->current_field_name];

    this->color_function->RemoveAllPoints();
    for (const auto& val : colormap_map.keys()) {
        double node = val*range.second + (1-val)*range.first;
        this->color_function->AddRGBPoint(node,
                                          colormap_map[val].red()/255.,
                                          colormap_map[val].green()/255.,
                                          colormap_map[val].blue()/255.);
    }

    this->color_function->ClampingOn();
    this->color_function->Modified();

    if(this->scalar_bar) {
        this->scalar_bar->SetLookupTable(this->color_function);
        this->scalar_bar->Modified();
    }
}

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorClutEditorSimple implementation
// ///////////////////////////////////////////////////////////////////

dtkVisualizationDecoratorClutEditorSimple::dtkVisualizationDecoratorClutEditorSimple(void): dtkVisualizationDecoratorClutEditorBase(), d(new dtkVisualizationDecoratorClutEditorSimplePrivate())
{
    this->setObjectName("Clut Editor Simple");

    d->scalar_bar = vtkSmartPointer<vtkScalarBarActor>::New();
    d->scalar_bar->SetWidth(0.08);
    d->scalar_bar->SetHeight(0.6);
    d->scalar_bar->SetVisibility(true);

    //////////
    // Inspectors creation and setup

    d->colormap_editor = new dtkVisualizationWidgetsColorMapEditor;
    d->colormap_editor->setValue("inferno");

    d->fields_box = new QComboBox;
    d->fields_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    d->display_scalarbar = new QCheckBox;
    d->display_scalarbar->setChecked(true);
    d->fixed_range = new QCheckBox;
    d->fixed_range->setChecked(Qt::Unchecked);

    QDoubleValidator *double_validator = new QDoubleValidator;
    double_validator->setDecimals(9);
    d->min_range = new QLineEdit;
    d->max_range = new QLineEdit;
    d->min_range->setValidator(double_validator);
    d->max_range->setValidator(double_validator);

    QHBoxLayout *h_layout_range = new QHBoxLayout;
    h_layout_range->setAlignment(Qt::AlignLeft);
    h_layout_range->addWidget(new QLabel("Fixed Range:"));
    h_layout_range->addWidget(d->fixed_range);
    h_layout_range->addWidget(d->min_range);
    h_layout_range->addWidget(d->max_range);

    QWidget *range_widget = new QWidget;
    range_widget->setLayout(h_layout_range);

    //////////
    // Inspectors connections

    connect(d->min_range, &QLineEdit::editingFinished, [=] () {
            d->fixed_range->setChecked(Qt::Checked);
            this->setCurrentRange(d->min_range->text().toDouble(), d->max_range->text().toDouble());
        });

    connect(d->max_range, &QLineEdit::editingFinished, [=] () {
            d->fixed_range->setChecked(Qt::Checked);
            this->setCurrentRange(d->min_range->text().toDouble(), d->max_range->text().toDouble());
        });
    // Restores default ranges when unchecked
    connect(d->fixed_range, &QCheckBox::stateChanged, [=] (int state) {
            if(state == Qt::Unchecked) {
                double *range;
                if (d->supports[d->current_field_name] == dtkVisualizationDecoratorClutEditorSimplePrivate::Support::Point) {
                    d->dataset->GetPointData()->SetActiveScalars(qPrintable(d->current_field_name));
                    range = d->dataset->GetPointData()->GetScalars()->GetRange();
                } else if (d->supports[d->current_field_name] == dtkVisualizationDecoratorClutEditorSimplePrivate::Support::Cell) {
                    d->dataset->GetCellData()->SetActiveScalars(qPrintable(d->current_field_name));
                    range = d->dataset->GetCellData()->GetScalars()->GetRange();
                }

                d->min_range->setText(QString::number(range[0]));
                d->max_range->setText(QString::number(range[1]));
                this->setCurrentRange(range[0], range[1]);
            } else {
                this->setCurrentRange(d->min_range->text().toDouble(), d->max_range->text().toDouble());
            }
        });

    connect(d->fields_box, &QComboBox::currentTextChanged,
            this, &dtkVisualizationDecoratorClutEditorSimple::setCurrentFieldName);

    d->display_scalarbar->setObjectName("Display Scalar Bar");
    d->fields_box->setObjectName("Field");
    d->colormap_editor->setObjectName("Color map");
    range_widget->setObjectName("");

    d->inspectors << d->display_scalarbar << d->fields_box << d->colormap_editor << range_widget;

    d->color_function = vtkSmartPointer<vtkColorTransferFunction>::New();
}

dtkVisualizationDecoratorClutEditorSimple::~dtkVisualizationDecoratorClutEditorSimple(void)
{
    d->dataset = nullptr;
    d->view = nullptr;
    delete d;
    d = nullptr;
}

void dtkVisualizationDecoratorClutEditorSimple::touch(void)
{
    if (!d->dataset || d->current_field_name.isEmpty()) {
        dtkWarn() << Q_FUNC_INFO << "Set dataset and field name before updating actor.";
        return;
    }

    if(!this->canvas()) {
        dtkWarn() << Q_FUNC_INFO << "No canvas was set, call setCanvas to call draw on a canvas.";
        return;
    }

    Q_ASSERT(this->canvas()->renderer());

    // Updates the color depending of the range
    if(!d->fixed_range->isChecked()) {
        double *range;
        if (d->supports[d->current_field_name] == dtkVisualizationDecoratorClutEditorSimplePrivate::Support::Point) {
            range = d->dataset->GetPointData()->GetArray(qPrintable(d->current_field_name))->GetRange();
        } else if (d->supports[d->current_field_name] == dtkVisualizationDecoratorClutEditorSimplePrivate::Support::Cell) {
            range = d->dataset->GetCellData()->GetScalars(qPrintable(d->current_field_name))->GetRange();
        }

        d->min_range->setText(QString::number(range[0]));
        d->max_range->setText(QString::number(range[1]));
        this->setCurrentRange(range[0], range[1]);
    }

}

bool dtkVisualizationDecoratorClutEditorSimple::isDecorating(void)
{
    return d->dataset;
}

void dtkVisualizationDecoratorClutEditorSimple::setData(const QVariant& data)
{
    d->dataset = data.value<vtkDataSet *>();
    if (!d->dataset) {
        dtkWarn() << Q_FUNC_INFO << "vtkDataSet is expected. Input data is not stored.";
        return;
    }

    d->current_field_name.clear();
    d->eligible_field_names.clear();
    d->color_transfer_functions.clear();
    d->opacity_functions.clear();
    d->ranges.clear();
    d->supports.clear();

    vtkIdType number_of_point_arrays = d->dataset->GetPointData()->GetNumberOfArrays();
    for (vtkIdType i = 0; i < number_of_point_arrays; ++i) {
        if (d->dataset->GetPointData()->GetArray(i)->GetNumberOfComponents() == 1) {
            QString field_name = QString::fromUtf8(d->dataset->GetPointData()->GetArrayName(i));
            d->eligible_field_names << field_name;
            d->supports[field_name] = dtkVisualizationDecoratorClutEditorSimplePrivate::Support::Point;

            d->dataset->GetPointData()->SetActiveScalars(qPrintable(field_name));
            double *range = static_cast<vtkDataArray *>(d->dataset->GetPointData()->GetScalars())->GetRange();
            d->ranges[field_name] = qMakePair(range[0], range[1]);
            d->color_transfer_functions[field_name] = "inferno";

            vtkSmartPointer<vtkPiecewiseFunction> opacity_function = vtkSmartPointer<vtkPiecewiseFunction>::New();
            opacity_function->RemoveAllPoints();
            opacity_function->AddPoint(0,   0.);
            opacity_function->AddPoint(255, 1.);

            d->opacity_functions[field_name] = opacity_function;
        }
    }

    vtkIdType number_of_cell_arrays = d->dataset->GetCellData()->GetNumberOfArrays();
    for (vtkIdType i = 0; i < number_of_cell_arrays; ++i) {
        if (d->dataset->GetCellData()->GetArray(i)->GetNumberOfComponents() == 1) {
            QString field_name = QString::fromUtf8(d->dataset->GetCellData()->GetArrayName(i));
            d->eligible_field_names << field_name;
            d->supports[field_name] = dtkVisualizationDecoratorClutEditorSimplePrivate::Support::Cell;

            d->dataset->GetCellData()->SetActiveScalars(qPrintable(field_name));
            double *range = static_cast<vtkDataArray *>(d->dataset->GetCellData()->GetScalars())->GetRange();
            d->ranges[field_name] = qMakePair(range[0], range[1]);
            d->color_transfer_functions[field_name] = "inferno";

            vtkSmartPointer<vtkPiecewiseFunction> opacity_function = vtkSmartPointer<vtkPiecewiseFunction>::New();
            opacity_function->RemoveAllPoints();
            opacity_function->AddPoint(0,   0.);
            opacity_function->AddPoint(255, 1.);

            d->opacity_functions[field_name] = opacity_function;
        }
    }

    if (d->eligible_field_names.size() > 0) {
        d->eligible_field_names.sort();

        if(d->fields_box) {
            d->fields_box->clear();
            d->fields_box->addItems(d->eligible_field_names);
        }

        this->setCurrentFieldName(d->eligible_field_names.first());
    }

    if (this->canvas()) {
        this->canvas()->addScalarBar(d->scalar_bar);
    }
}

void dtkVisualizationDecoratorClutEditorSimple::setCanvas(dtkVisualizationCanvas *canvas)
{
    this->unsetCanvas();

    d->view = dynamic_cast<dtkVisualizationView2D *>(canvas);
    if (!d->view) {
        qWarning() << Q_FUNC_INFO << "View 2D or view 3D expected as canvas. Canvas is reset to nullptr.";
        return;
    }

    d->display_scalarbar->disconnect();

    connect(d->display_scalarbar, &QCheckBox::stateChanged, [=] (int value) {
            if (value == Qt::Checked)
                this->canvas()->renderer()->AddActor2D(d->scalar_bar);
            else
                this->canvas()->renderer()->RemoveActor2D(d->scalar_bar);

            this->canvas()->renderer()->GetRenderWindow()->Render();
        });

    d->colormap_editor->disconnect();

    connect(d->colormap_editor, &dtkVisualizationWidgetsColorMapEditor::valueChanged,
            [=] (const QMap<double, QColor>& val) {
                d->color_transfer_functions[d->current_field_name] = d->colormap_editor->name();
                d->colormap_editor->blockSignals(true);
                d->updateColormap(val);
                d->colormap_editor->blockSignals(false);
                emit updated();

                this->canvas()->renderer()->GetRenderWindow()->Render();
            });

    if(d->dataset) {
        d->view->renderer()->AddActor2D(d->scalar_bar);
    }
}

void dtkVisualizationDecoratorClutEditorSimple::unsetCanvas(void)
{
    if (d->view) {
        d->view->renderer()->RemoveActor2D(d->scalar_bar);
    }
    d->view = nullptr;
}


QStringList dtkVisualizationDecoratorClutEditorSimple::eligibleFieldNames(void) const
{
    return d->eligible_field_names;
}

QString dtkVisualizationDecoratorClutEditorSimple::currentFieldName(void) const
{
    return d->current_field_name;
}

QPair<double, double> dtkVisualizationDecoratorClutEditorSimple::currentRange(void) const
{
    return d->ranges[d->current_field_name];
}

vtkSmartPointer<vtkColorTransferFunction> dtkVisualizationDecoratorClutEditorSimple::currentColorTransferFunction(void) const
{
    return d->color_function;
}

vtkSmartPointer<vtkPiecewiseFunction> dtkVisualizationDecoratorClutEditorSimple::currentOpacityTransferFunction(void) const
{
    return d->opacity_functions[d->current_field_name];
}

bool dtkVisualizationDecoratorClutEditorSimple::setCurrentFieldName(const QString& scalar_field_name)
{
    if (scalar_field_name.isEmpty()) {
        dtkWarn() << Q_FUNC_INFO << "Scalar field name is empty, nothing is done.";
        return false;
    }

    if (!d->dataset) {
        dtkWarn() << Q_FUNC_INFO << "Before calling setScalarFieldName, setDataSet must be called.";
        return false;
    }

    if(!d->eligible_field_names.contains(scalar_field_name)) {
        dtkWarn() << Q_FUNC_INFO << "The field name :" << scalar_field_name << "that was specified doesn't match any of the eligible scalar field names";

        return false;
    }

    d->current_field_name = scalar_field_name;

    if(d->fields_box)
        d->fields_box->setCurrentText(d->current_field_name);

    if (d->supports[d->current_field_name] == dtkVisualizationDecoratorClutEditorSimplePrivate::Support::Point) {
        d->dataset->GetPointData()->SetActiveScalars(qPrintable(d->current_field_name));
    } else if (d->supports[d->current_field_name] == dtkVisualizationDecoratorClutEditorSimplePrivate::Support::Cell) {
        d->dataset->GetCellData()->SetActiveScalars(qPrintable(d->current_field_name));
    }

    d->scalar_bar->SetTitle(qPrintable(d->current_field_name));

    auto range = d->ranges[d->current_field_name];
    d->min_range->setText(QString::number(range.first));
    d->max_range->setText(QString::number(range.second));

    d->updateColormap();

    d->scalar_bar->Modified();

    emit updated();

    return true;
}

void dtkVisualizationDecoratorClutEditorSimple::setCurrentColorTransferFunction(vtkSmartPointer<vtkColorTransferFunction> color_function)
{
    //TODO Delete this function
    qWarning() << Q_FUNC_INFO;
}

void dtkVisualizationDecoratorClutEditorSimple::setCurrentOpacityTransferFunction(vtkSmartPointer<vtkPiecewiseFunction> opacity_function)
{
    // TODO keep or delete
    if (d->current_field_name.isEmpty()) {
        return;
    }

    d->opacity_functions[d->current_field_name]->DeepCopy(opacity_function);
    /*
    int size = opacity_function->GetSize();

    double val[4];
    double opacity_min = 1.;
    for(std::size_t i = 0; i < size; ++i) {
        opacity_function->GetNodeValue(i, val);
        if(val[1] < opacity_min) opacity_min = val[1];
    }

    d->actor->GetProperty()->SetOpacity(opacity_min);
    d->actor->Modified();

    if (this->canvas() && this->canvas()->interactor()) {
            this->canvas()->interactor()->Render();
    }
*/
    //emit this->currentOpacityFunctionChanged(d->opacity_functions[d->current_field_name]);
}

void dtkVisualizationDecoratorClutEditorSimple::setCurrentRange(double min, double max)
{
    if (d->current_field_name.isEmpty()) {
        return;
    }

    if(min < max) {
        d->ranges[d->current_field_name] = qMakePair(min, max);
        d->updateColormap();

        emit updated();
    } else {
        qWarning() << Q_FUNC_INFO << " min >= max : " << min << max;
    }
}

QVariant dtkVisualizationDecoratorClutEditorSimple::data(void) const
{
    if (d->dataset) {
        return dtk::variantFromValue(d->dataset);

    } else {
        return QVariant();
    }
}

dtkVisualizationCanvas *dtkVisualizationDecoratorClutEditorSimple::canvas(void) const
{
    return d->view;
}

QList<QWidget *> dtkVisualizationDecoratorClutEditorSimple::inspectors(void) const
{
    return d->inspectors;
}

//
// dtkVisualizationDecoratorClutEditorSimple.cpp ends here
