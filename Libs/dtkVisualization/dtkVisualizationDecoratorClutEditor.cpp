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

#include "dtkVisualizationDecoratorClutEditor.h"

#include "dtkVisualizationMetaType.h"
#include "dtkVisualizationView2D.h"

#include <dtkVisualizationWidgets/dtkVisualizationWidgetsClutEditor>

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
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRendererCollection.h>
#include <vtkScalarBarActor.h>

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorClutEditorPrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationDecoratorClutEditorPrivate
{
public:
    enum Support { Unknown = 0, Point = 1, Cell = 2 };

public:
    QStringList eligible_field_names;
    QString current_field_name;

public:
    vtkDataSet *dataset = nullptr;
    dtkVisualizationView2D *view = nullptr;

public:
    QList<QWidget *> inspectors;

public:
    vtkSmartPointer<vtkPiecewiseFunction> opacity;
    vtkSmartPointer<vtkScalarBarActor> scalar_bar;

public:
    QHash<QString, vtkSmartPointer<vtkColorTransferFunction>> color_transfer_functions;
    QHash<QString, vtkSmartPointer<vtkPiecewiseFunction>> opacity_functions;
    QHash<QString, QPair<double, double>> ranges;
    QHash<QString, int> supports;

public:
    QCheckBox *display_scalarbar = nullptr;
    QComboBox *fields_box = nullptr;
    QCheckBox *fixed_range = nullptr;
    QLineEdit *min_range = nullptr;
    QLineEdit *max_range = nullptr;

    dtkVisualizationWidgetsClutEditor *clut_editor = nullptr;
};

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorClutEditor implementation
// ///////////////////////////////////////////////////////////////////

dtkVisualizationDecoratorClutEditor::dtkVisualizationDecoratorClutEditor(void)
    : dtkVisualizationDecoratorClutEditorBase(), d(new dtkVisualizationDecoratorClutEditorPrivate())
{
    this->setObjectName("Clut Editor");

    d->scalar_bar = vtkSmartPointer<vtkScalarBarActor>::New();
    d->scalar_bar->SetWidth(0.08);
    d->scalar_bar->SetHeight(0.6);
    d->scalar_bar->SetVisibility(true);

    //////////
    // Inspectors creation and setup

    d->clut_editor = new dtkVisualizationWidgetsClutEditor;

    d->fields_box = new QComboBox;
    d->fields_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    d->display_scalarbar = new QCheckBox;
    d->display_scalarbar->setChecked(true);

    d->fixed_range = new QCheckBox;
    d->fixed_range->setChecked(Qt::Unchecked);

    QDoubleValidator *double_validator = new QDoubleValidator;
    double_validator->setDecimals(3);
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

    connect(d->min_range, &QLineEdit::textEdited, [=](QString new_min) {
        d->fixed_range->setChecked(Qt::Checked);
        this->setCurrentRange(new_min.toDouble(), d->max_range->text().toDouble());
    });

    connect(d->max_range, &QLineEdit::textEdited, [=](QString new_max) {
        d->fixed_range->setChecked(Qt::Checked);
        this->setCurrentRange(d->min_range->text().toDouble(), new_max.toDouble());
    });
    // Restores default ranges when unchecked
    connect(d->fixed_range, &QCheckBox::stateChanged, [=](int state) {
        if (state == Qt::Unchecked) {
            double *range;
            if (d->supports[d->current_field_name]
                == dtkVisualizationDecoratorClutEditorPrivate::Support::Point) {
                d->dataset->GetPointData()->SetActiveScalars(qPrintable(d->current_field_name));
                range = d->dataset->GetPointData()->GetScalars()->GetRange();
            } else if (d->supports[d->current_field_name]
                       == dtkVisualizationDecoratorClutEditorPrivate::Support::Cell) {
                d->dataset->GetCellData()->SetActiveScalars(qPrintable(d->current_field_name));
                range = d->dataset->GetCellData()->GetScalars()->GetRange();
            }

            this->setCurrentRange(range[0], range[1]);
        } else {
            this->setCurrentRange(d->min_range->text().toDouble(), d->max_range->text().toDouble());
        }
    });

    connect(d->fields_box, &QComboBox::currentTextChanged, this,
            &dtkVisualizationDecoratorClutEditor::setCurrentFieldName);

    d->display_scalarbar->setObjectName("Display Scalar Bar");
    d->fields_box->setObjectName("Field");
    d->clut_editor->setObjectName("");
    range_widget->setObjectName("Range");

    d->inspectors << d->display_scalarbar << d->fields_box << d->clut_editor << range_widget;
}

dtkVisualizationDecoratorClutEditor::~dtkVisualizationDecoratorClutEditor(void)
{
    d->dataset = nullptr;
    d->view = nullptr;
    delete d;
    d = nullptr;
}

void dtkVisualizationDecoratorClutEditor::touch(void)
{
    if (!d->dataset || d->current_field_name.isEmpty()) {
        dtkWarn() << Q_FUNC_INFO << "Set dataset and field name before updating actor.";
        return;
    }

    if (!this->canvas()) {
        dtkWarn() << Q_FUNC_INFO << "No canvas was set, call setCanvas to call draw on a canvas.";
        return;
    }

    Q_ASSERT(this->canvas()->renderer());

    // Updates the color depending of the range
    if (!d->fixed_range->isChecked()) {

        double *range;
        if (d->supports[d->current_field_name]
            == dtkVisualizationDecoratorClutEditorPrivate::Support::Point) {
            range = d->dataset->GetPointData()
                            ->GetArray(qPrintable(d->current_field_name))
                            ->GetRange();
        } else if (d->supports[d->current_field_name]
                   == dtkVisualizationDecoratorClutEditorPrivate::Support::Cell) {
            range = d->dataset->GetCellData()
                            ->GetArray(qPrintable(d->current_field_name))
                            ->GetRange();
        }

        this->setCurrentRange(range[0], range[1]);
    }
}

bool dtkVisualizationDecoratorClutEditor::isDecorating(void)
{
    return d->dataset;
}

void dtkVisualizationDecoratorClutEditor::setData(const QVariant &data)
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
            d->supports[field_name] = dtkVisualizationDecoratorClutEditorPrivate::Support::Point;

            d->dataset->GetPointData()->SetActiveScalars(qPrintable(field_name));
            double *range = static_cast<vtkDataArray *>(d->dataset->GetPointData()->GetScalars())
                                    ->GetRange();
            d->ranges[field_name] = qMakePair(range[0], range[1]);

            vtkSmartPointer<vtkColorTransferFunction> color_function =
                    vtkSmartPointer<vtkColorTransferFunction>::New();
            QColor black = QColor(0., 0., 0., 255);
            color_function->AddRGBPoint(range[0], black.red(), black.green(), black.blue());
            QColor white = QColor(1., 1., 1., 255);
            color_function->AddRGBPoint(range[1], white.red(), white.green(), white.blue());
            color_function->Build();
            d->color_transfer_functions[field_name] = color_function;

            vtkSmartPointer<vtkPiecewiseFunction> opacity_function =
                    vtkSmartPointer<vtkPiecewiseFunction>::New();
            opacity_function->RemoveAllPoints();
            opacity_function->AddPoint(0, 0.);
            opacity_function->AddPoint(255, 1.);

            d->opacity_functions[field_name] = opacity_function;
        }
    }

    vtkIdType number_of_cell_arrays = d->dataset->GetCellData()->GetNumberOfArrays();
    for (vtkIdType i = 0; i < number_of_cell_arrays; ++i) {
        if (d->dataset->GetCellData()->GetArray(i)->GetNumberOfComponents() == 1) {
            QString field_name = QString::fromUtf8(d->dataset->GetCellData()->GetArrayName(i));
            d->eligible_field_names << field_name;
            d->supports[field_name] = dtkVisualizationDecoratorClutEditorPrivate::Support::Cell;

            d->dataset->GetCellData()->SetActiveScalars(qPrintable(field_name));
            double *range = static_cast<vtkDataArray *>(d->dataset->GetCellData()->GetScalars())
                                    ->GetRange();
            d->ranges[field_name] = qMakePair(range[0], range[1]);

            vtkSmartPointer<vtkColorTransferFunction> color_function =
                    vtkSmartPointer<vtkColorTransferFunction>::New();
            QColor black = QColor(0., 0., 0., 255);
            color_function->AddRGBPoint(range[0], black.red(), black.green(), black.blue());
            QColor white = QColor(1., 1., 1., 255);
            color_function->AddRGBPoint(range[1], white.red(), white.green(), white.blue());
            color_function->Build();
            d->color_transfer_functions[field_name] = color_function;

            vtkSmartPointer<vtkPiecewiseFunction> opacity_function =
                    vtkSmartPointer<vtkPiecewiseFunction>::New();
            opacity_function->RemoveAllPoints();
            opacity_function->AddPoint(0, 0.);
            opacity_function->AddPoint(255, 1.);

            d->opacity_functions[field_name] = opacity_function;
        }
    }

    if (d->eligible_field_names.size() > 0) {
        d->eligible_field_names.sort();

        if (d->fields_box) {
            d->fields_box->clear();
            d->fields_box->addItems(d->eligible_field_names);
        }

        this->setCurrentFieldName(d->eligible_field_names.first());
    }

    if (this->canvas()) {
        this->canvas()->addScalarBar(d->scalar_bar);
    }
}

void dtkVisualizationDecoratorClutEditor::setCanvas(dtkVisualizationCanvas *canvas)
{
    this->unsetCanvas();

    d->view = dynamic_cast<dtkVisualizationView2D *>(canvas);
    if (!d->view) {
        qWarning() << Q_FUNC_INFO
                   << "View 2D or view 3D expected as canvas. Canvas is reset "
                      "to nullptr.";
        return;
    }

    d->display_scalarbar->disconnect();

    connect(d->display_scalarbar, &QCheckBox::stateChanged, [=](int value) {
        if (value == Qt::Checked)
            this->canvas()->renderer()->AddActor2D(d->scalar_bar);
        else
            this->canvas()->renderer()->RemoveActor2D(d->scalar_bar);

        this->canvas()->renderer()->GetRenderWindow()->Render();
    });

    // clut_editor connections
    // connect(this, &dtkVisualizationDecoratorClutEditor::currentRangeChanged,
    // [=] (double min, double max) {
    //         clut_editor->setRange(min, max);
    //         if(!d->fixed_range->isChecked()) {
    //             d->min_range->setText(QString::number(min));
    //             d->max_range->setText(QString::number(max));
    //         }
    //     });

    // connect(this,
    // &dtkVisualizationDecoratorClutEditor::currentColorTransferFunctionChanged,
    // [=] (vtkColorTransferFunction *color_transfer_function) {
    //         clut_editor->blockSignals(true);
    //         clut_editor->setColorTransferFunction(color_transfer_function);
    //         clut_editor->blockSignals(false);
    //     });

    d->clut_editor->disconnect();

    connect(d->clut_editor, &dtkVisualizationWidgetsClutEditor::updated, [=]() {
        // need to block signal to avoid calling the previous connect
        this->blockSignals(true);
        this->setCurrentColorTransferFunction(reinterpret_cast<vtkColorTransferFunction *>(
                d->clut_editor->colorTransferFunction()));
        this->setCurrentOpacityTransferFunction(reinterpret_cast<vtkPiecewiseFunction *>(
                d->clut_editor->opacityTransferFunction()));
        this->blockSignals(false);

        if (this->canvas())
            this->canvas()->renderer()->GetRenderWindow()->Render();

        emit updated();
    });

    if (d->dataset) {
        d->view->renderer()->AddActor2D(d->scalar_bar);
    }
}

void dtkVisualizationDecoratorClutEditor::unsetCanvas(void)
{
    if (d->view) {
        d->view->renderer()->RemoveActor2D(d->scalar_bar);
    }
    d->view = nullptr;
}

QStringList dtkVisualizationDecoratorClutEditor::eligibleFieldNames(void) const
{
    return d->eligible_field_names;
}

QString dtkVisualizationDecoratorClutEditor::currentFieldName(void) const
{
    return d->current_field_name;
}

QPair<double, double> dtkVisualizationDecoratorClutEditor::currentRange(void) const
{
    return d->ranges[d->current_field_name];
}

vtkSmartPointer<vtkColorTransferFunction>
dtkVisualizationDecoratorClutEditor::currentColorTransferFunction(void) const
{
    return d->color_transfer_functions[d->current_field_name];
}

vtkSmartPointer<vtkPiecewiseFunction>
dtkVisualizationDecoratorClutEditor::currentOpacityTransferFunction(void) const
{
    return d->opacity_functions[d->current_field_name];
}

bool dtkVisualizationDecoratorClutEditor::setCurrentFieldName(const QString &scalar_field_name)
{
    if (scalar_field_name.isEmpty()) {
        dtkWarn() << Q_FUNC_INFO << "Scalar field name is empty, nothing is done.";
        return false;
    }

    if (!d->dataset) {
        dtkWarn() << Q_FUNC_INFO << "Before calling setScalarFieldName, setDataSet must be called.";
        return false;
    }

    if (!d->eligible_field_names.contains(scalar_field_name)) {
        dtkWarn() << Q_FUNC_INFO << "The field name :" << scalar_field_name
                  << "that was specified doesn't match any of the eligible "
                     "scalar field names";

        return false;
    }

    d->current_field_name = scalar_field_name;

    if (d->fields_box)
        d->fields_box->setCurrentText(d->current_field_name);

    if (d->supports[d->current_field_name]
        == dtkVisualizationDecoratorClutEditorPrivate::Support::Point) {
        d->dataset->GetPointData()->SetActiveScalars(qPrintable(d->current_field_name));
    } else if (d->supports[d->current_field_name]
               == dtkVisualizationDecoratorClutEditorPrivate::Support::Cell) {
        d->dataset->GetCellData()->SetActiveScalars(qPrintable(d->current_field_name));
    }

    vtkSmartPointer<vtkColorTransferFunction> color_function =
            d->color_transfer_functions[d->current_field_name];
    const QPair<double, double> &range = d->ranges[d->current_field_name];

    d->clut_editor->blockSignals(true);
    d->clut_editor->setColorTransferFunction(color_function);
    d->clut_editor->setRange(range.first, range.second);
    d->clut_editor->blockSignals(false);

    if (!d->fixed_range->isChecked()) {
        d->min_range->setText(QString::number(range.first));
        d->max_range->setText(QString::number(range.second));
    }

    d->scalar_bar->SetTitle(qPrintable(d->current_field_name));
    d->scalar_bar->SetLookupTable(color_function);
    // TODO range SetMapper2D
    d->scalar_bar->Modified();

    emit updated();

    return true;
}

void dtkVisualizationDecoratorClutEditor::setCurrentColorTransferFunction(
        vtkSmartPointer<vtkColorTransferFunction> color_function)
{
    if (d->current_field_name.isEmpty()) {
        return;
    }

    d->color_transfer_functions[d->current_field_name]->DeepCopy(color_function);

    d->scalar_bar->SetLookupTable(d->color_transfer_functions[d->current_field_name]);
    // TODO range SetMapper2D
    d->scalar_bar->Modified();

    d->clut_editor->blockSignals(true);
    d->clut_editor->setColorTransferFunction(d->color_transfer_functions[d->current_field_name]);
    d->clut_editor->blockSignals(false);

    emit updated();
}

void dtkVisualizationDecoratorClutEditor::setCurrentOpacityTransferFunction(
        vtkSmartPointer<vtkPiecewiseFunction> opacity_function)
{
    // TODO keep or delete
    if (d->current_field_name.isEmpty()) {
        return;
    }

    d->opacity_functions[d->current_field_name]->DeepCopy(opacity_function);

    // set opacity of d->clut_editor

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
    emit updated();
}

void dtkVisualizationDecoratorClutEditor::setCurrentRange(double min, double max)
{
    if (d->current_field_name.isEmpty()) {
        return;
    }

    if (min <= max) {
        d->ranges[d->current_field_name] = qMakePair(min, max);
        d->clut_editor->setRange(min, max);
    } else {
        qWarning() << "Error in range : min > max : " << min << max;
    }
}

QVariant dtkVisualizationDecoratorClutEditor::data(void) const
{
    if (d->dataset) {
        return dtk::variantFromValue(d->dataset);

    } else {
        return QVariant();
    }
}

dtkVisualizationCanvas *dtkVisualizationDecoratorClutEditor::canvas(void) const
{
    return d->view;
}

QList<QWidget *> dtkVisualizationDecoratorClutEditor::inspectors(void) const
{
    return d->inspectors;
}

//
// dtkVisualizationDecoratorClutEditor.cpp ends here
