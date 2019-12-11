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

#include "dtkVisualizationPlot2D.h"

#include <dtkWidgets>

#include <QtWidgets>

#include <vtkAbstractArray.h>
#include <vtkAxis.h>
#include <vtkBrush.h>
#include <vtkChartXY.h>
#include <vtkContextScene.h>
#include <vtkContextView.h>
#include <vtkDelimitedTextWriter.h>
#include <vtkDoubleArray.h>
#include <vtkFieldData.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkPen.h>
#include <vtkPlot.h>
#include <vtkPlotPoints.h>
#include <vtkPNGWriter.h>
#include <vtkTable.h>
#include <vtkTextProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkWindowToImageFilter.h>

//#include <QVTKOpenGLWidget.h>


//needed to override the paintEvent and put state_sunken when checked
class buttonStayDown : public QPushButton
{
public:
    buttonStayDown(const QString& text, QWidget *parent);

public:
    void paintEvent(QPaintEvent *) override;
};

buttonStayDown::buttonStayDown(const QString& text, QWidget *parent) : QPushButton(text, parent)
{
}

void buttonStayDown::paintEvent(QPaintEvent *)
{
    QStylePainter p(this);
    QStyleOptionButton option;
    initStyleOption(&option);
    if(this->isChecked())
        option.state = QStyle::State_Sunken;

    p.drawControl(QStyle::CE_PushButton, option);
}

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationPlot2DPrivate
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationPlot2DPrivate
{
public:
    dtkVisualizationPlot2DPrivate(void);
    ~dtkVisualizationPlot2DPrivate(void);
    struct FieldParameters {
        int chart_type;
        int marker_style;
        int width;
        QColor color;
    };

public:
    void clear(void);

public:
    vtkSmartPointer<vtkContextView> context_view = nullptr;
    vtkSmartPointer<vtkTable> fields_table = nullptr;
    vtkSmartPointer<vtkChartXY> chart = nullptr;
    vtkSmartPointer<vtkFieldData> old_fields = nullptr;
    QMap< QString, FieldParameters> old_fields_param;
    QString old_field_x = "";
    QString add_plot_suffix = "";

public:
    QColor random(void);

public:
    // selection
    QComboBox *field_x;
    QListWidget *field_y;
    buttonStayDown *save_field;
    buttonStayDown *remove_field;

    // range
    QCheckBox *autoscale;
    QLineEdit *x_min;
    QLineEdit *x_max;
    QLineEdit *y_min;
    QLineEdit *y_max;

    // display
    QStringListModel *fields_table_model;
    QMap<QString, FieldParameters> fields_parameters;
    bool show_legend = true;
};

dtkVisualizationPlot2DPrivate::dtkVisualizationPlot2DPrivate(void)
{
    this->fields_table = vtkSmartPointer<vtkTable>::New();
    this->fields_table_model = new QStringListModel;
    this->old_fields = vtkSmartPointer<vtkFieldData>::New();

    qsrand((uint)QTime::currentTime().msec());
}

dtkVisualizationPlot2DPrivate::~dtkVisualizationPlot2DPrivate(void)
{
    delete this->fields_table_model;
}

void dtkVisualizationPlot2DPrivate::clear(void)
{
    this->chart->ClearPlots();
}


QColor dtkVisualizationPlot2DPrivate::random(void)
{
    return QColor(QColor::colorNames().at(qrand() % QColor::colorNames().count()));
}


// ///////////////////////////////////////////////////////////////////
// dtkVisualizationPlot2D
// ///////////////////////////////////////////////////////////////////

dtkVisualizationPlot2D::dtkVisualizationPlot2D(QWidget *parent) : dtkVisualizationCanvas(parent), d(new dtkVisualizationPlot2DPrivate)
{
    static int count = 1;
    this->setObjectName(QString("Plot2D - %1").arg(count++));

    // ///////////////
    // create menu
    // ///////////////
    dtkWidgetsMenuBar *menu_bar = dtkApp->window()->menubar();
    dtkWidgetsMenu *menu = menu_bar->addMenu(fa::delicious, "View " + this->objectName());

    // ///////////////
    // fields selection
    // ///////////////
    d->save_field = new buttonStayDown("save_field", this);
    d->save_field->setCheckable(true);
    d->save_field->setChecked(false);
    d->remove_field = new buttonStayDown("remove_field", this);
    d->remove_field->setCheckable(true);
    d->remove_field->setChecked(false);

    connect(d->save_field, &QPushButton::clicked, [=](bool checked)
    {
        d->remove_field->setChecked(false);
        QGuiApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
        if(checked) {
            //unselect items
            QList<QListWidgetItem *> l = d->field_y->selectedItems();
            for(auto *i: l)
                i->setSelected(false);

            QGuiApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
            this->hud()->addInfo("Select a field or press 'Escape'");
         }
    });

    connect(d->remove_field, &QPushButton::clicked, [=](bool checked)
    {
        d->save_field->setChecked(false);
        QGuiApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
        if(checked) {
            QList<QListWidgetItem *> l = d->field_y->selectedItems();
            for(auto *i: l)
                i->setSelected(false);

            QGuiApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
            this->hud()->addInfo("Select a field or press 'Escape'");
        }
    });

    d->field_x = new QComboBox(this);
    d->field_x->setObjectName("X");
    d->field_x->setInsertPolicy(QComboBox::InsertAlphabetically);
    d->field_x->setModel(d->fields_table_model);

    d->field_y = new QListWidget(this);
    d->field_y->setObjectName("Y");
    d->field_y->setFixedHeight(200);

    QHBoxLayout *fields_buttons_layout = new QHBoxLayout();
    fields_buttons_layout->addWidget(d->save_field);
    fields_buttons_layout->addWidget(d->remove_field);
    QFormLayout *fields_layout = new QFormLayout();
    fields_layout->addRow(d->field_x->objectName(), d->field_x);
    fields_layout->addRow(d->field_y->objectName(), d->field_y);
    dtkWidgetsMenuItemDIY *fields_diy = new dtkWidgetsMenuItemDIY("fields selection");
    fields_diy->addLayout(fields_buttons_layout);
    fields_diy->addLayout(fields_layout);

    menu->addItem(fields_diy);

    // ///////////////
    // range
    // ///////////////
    d->autoscale = new QCheckBox(this);
    d->autoscale->setChecked(true);
    d->autoscale->setObjectName("autoscale");

    QDoubleValidator *double_validator = new QDoubleValidator(this);
    double_validator->setDecimals(4);

    d->x_min = new QLineEdit(this);
    d->x_min->setObjectName("x_min");
    d->x_max = new QLineEdit(this);
    d->x_max->setObjectName("x_max");
    d->y_min = new QLineEdit(this);
    d->y_min->setObjectName("y_min");
    d->y_max = new QLineEdit(this);
    d->y_max->setObjectName("y_max");

    d->x_min->setValidator(double_validator);
    d->x_max->setValidator(double_validator);
    d->y_min->setValidator(double_validator);
    d->y_max->setValidator(double_validator);

    QWidget *range_w = new QWidget(this);
    QFormLayout *range_layout = new QFormLayout(range_w);
    range_layout->addRow(d->autoscale->objectName(), d->autoscale);
    range_layout->addRow(d->x_min->objectName(), d->x_min);
    range_layout->addRow(d->x_max->objectName(), d->x_max);
    range_layout->addRow(d->y_min->objectName(), d->y_min);
    range_layout->addRow(d->y_max->objectName(), d->y_max);
    dtkWidgetsMenuItemDIY *range_diy = new dtkWidgetsMenuItemDIY("Range", range_w);
    menu->addItem(range_diy);

    connect(d->field_x, &QComboBox::currentTextChanged, [=] ()
    {
        if(d->field_x->currentText().isEmpty())
            return;

        if(d->old_field_x != d->field_x->currentText()) {
            for(vtkIdType i=0; i < d->old_fields->GetNumberOfArrays() ; ++i) {
                d->fields_table->RemoveColumnByName(d->old_fields->GetArray(i)->GetName());
                d->old_fields->RemoveArray(i);
            }
            d->old_fields_param.clear();
        }
        this->render();
    });
    connect(d->field_y, &QListWidget::itemChanged, [=] ()
    {
        this->render();
    });


    connect(d->field_y, &QListWidget::currentRowChanged, [=] (int currentRow)
    {
        // it will happen if we call clear on the qlistwidget when a row is selected
        if(!d->field_y->item(currentRow))
            return;

        QString row_text = d->field_y->item(currentRow)->text();

        if(d->save_field->isChecked()) {
            d->save_field->setChecked(false);

            //add to old_fields
            vtkAbstractArray *arr = d->fields_table->GetColumnByName(row_text.toStdString().c_str());

            QString new_name;
            if(d->add_plot_suffix.isEmpty()) {
                new_name = QString("x:" + d->field_x->currentText() + "_y:" + row_text) ;
            } else {
                new_name = QString(arr->GetName() + d->add_plot_suffix);
            }

            if(!d->old_fields->HasArray(new_name.toStdString().c_str())) {
                d->old_field_x = d->field_x->currentText();
                dtkVisualizationPlot2DPrivate::FieldParameters new_param = d->fields_parameters[row_text];
                d->old_fields_param.insert(new_name, new_param);
                vtkSmartPointer<vtkDoubleArray> new_field = vtkSmartPointer<vtkDoubleArray>::New();
                new_field->DeepCopy(arr);
                new_field->SetName(new_name.toStdString().c_str()) ;
                d->old_fields->AddArray(new_field);

                this->addFieldY(new_field,
                                false,
                                d->fields_parameters[row_text].chart_type,
                                d->fields_parameters[row_text].marker_style,
                                d->fields_parameters[row_text].color);
            }
        }

        if(d->remove_field->isChecked()) {
            d->remove_field->setChecked(false);
            d->field_y->blockSignals(true);
            //if it's an old_field, remove it
            if(d->old_fields_param.contains(d->field_y->item(currentRow)->text())) {
                d->old_fields_param.remove(d->field_y->item(currentRow)->text());
                d->old_fields->RemoveArray(d->field_y->item(currentRow)->text().toStdString().c_str());
            }

            this->removeField(d->field_y->item(currentRow)->text());
            d->field_y->blockSignals(false);
        }

    });
    connect(d->autoscale, &QCheckBox::stateChanged, [=] () {
            if(d->autoscale->isChecked()) {
                d->chart->GetAxis(vtkAxis::LEFT)->SetBehavior(vtkAxis::AUTO);
                d->chart->GetAxis(vtkAxis::LEFT)->AutoScale();
                d->chart->GetAxis(vtkAxis::LEFT)->Modified();

                d->chart->GetAxis(vtkAxis::BOTTOM)->SetBehavior(vtkAxis::AUTO);
                d->chart->GetAxis(vtkAxis::BOTTOM)->AutoScale();
                d->chart->GetAxis(vtkAxis::BOTTOM)->Modified();

                this->render();

                d->x_min->setText(QString::number(d->chart->GetAxis(vtkAxis::BOTTOM)->GetMinimum()));
                d->x_max->setText(QString::number(d->chart->GetAxis(vtkAxis::BOTTOM)->GetMaximum()));
                d->y_min->setText(QString::number(d->chart->GetAxis(vtkAxis::LEFT)->GetMinimum()));
                d->y_max->setText(QString::number(d->chart->GetAxis(vtkAxis::LEFT)->GetMaximum()));
            }

        });

    connect(d->x_min, &QLineEdit::textEdited, [=] (const QString &text) {
            d->autoscale->setChecked(false);
            d->chart->GetAxis(vtkAxis::BOTTOM)->SetBehavior(vtkAxis::FIXED);
            d->chart->GetAxis(vtkAxis::BOTTOM)->SetMinimum(text.toDouble());
            d->chart->GetAxis(vtkAxis::BOTTOM)->Modified();
            this->render();
        });

    connect(d->x_max, &QLineEdit::textEdited, [=] (const QString &text) {
            d->autoscale->setChecked(false);
            d->chart->GetAxis(vtkAxis::BOTTOM)->SetBehavior(vtkAxis::FIXED);
            d->chart->GetAxis(vtkAxis::BOTTOM)->SetMaximum(text.toDouble());
            d->chart->GetAxis(vtkAxis::BOTTOM)->Modified();
                this->render();
        });

    connect(d->y_min, &QLineEdit::textEdited, [=] (const QString &text) {
            d->autoscale->setChecked(false);
            d->chart->GetAxis(vtkAxis::LEFT)->SetBehavior(vtkAxis::FIXED);
            d->chart->GetAxis(vtkAxis::LEFT)->SetMinimum(text.toDouble());
            d->chart->GetAxis(vtkAxis::BOTTOM)->Modified();
            this->render();
        });

    connect(d->y_max, &QLineEdit::textEdited, [=] (const QString &text) {
            d->autoscale->setChecked(false);
            d->chart->GetAxis(vtkAxis::LEFT)->SetBehavior(vtkAxis::FIXED);
            d->chart->GetAxis(vtkAxis::LEFT)->SetMaximum(text.toDouble());
            d->chart->GetAxis(vtkAxis::BOTTOM)->Modified();
            this->render();
        });

    // ///////////////
    // Export
    // ///////////////

    QPushButton *export_as_png = new QPushButton(this);
    export_as_png->setObjectName("Export as png");
    export_as_png->setText("PNG");

    QPushButton *export_as_csv = new QPushButton(this);
    export_as_csv->setObjectName("Export as csv");
    export_as_csv->setText("CSV");

    QFormLayout *export_layout = new QFormLayout();
    export_layout->addRow(export_as_csv->objectName(), export_as_csv);
    export_layout->addRow(export_as_png->objectName(), export_as_png);
    dtkWidgetsMenuItemDIY *export_diy = new dtkWidgetsMenuItemDIY("Export");
    export_diy->addLayout(export_layout);
    menu->addItem(export_diy);

    connect(export_as_png, &QPushButton::clicked, [=] ()
    {
        QString path = QDir::homePath();

        QFileDialog *dialog = new QFileDialog(this, tr("Save as"), path, QString("PNG file (*.png)"));
        dialog->setAcceptMode(QFileDialog::AcceptSave);
        dialog->setFileMode(QFileDialog::AnyFile);
        dialog->open(this, SLOT(exportAsPNG(const QString&)));
    });

    connect(export_as_csv, &QPushButton::clicked, [=] ()
    {
        QString path = QDir::homePath();

        QFileDialog *dialog = new QFileDialog(this, tr("Save as"), path, QString("csv file (*.csv)"));
        dialog->setAcceptMode(QFileDialog::AcceptSave);
        dialog->setFileMode(QFileDialog::AnyFile);
        dialog->open(this, SLOT(exportAsCSV(const QString&)));
    });


    // ///////////////
    // Set up the view
    // ///////////////
    d->context_view = vtkSmartPointer<vtkContextView>::New();
    d->context_view->SetRenderWindow(this->renderer()->GetRenderWindow());
    //d->context_view->SetRenderer(this->renderer()); // this is not working
    d->context_view->GetRenderer()->SetBackground(0.290, 0.295, 0.300);

    // // Add multiple line plots, setting the colors etc
    d->chart = vtkSmartPointer<vtkChartXY>::New();

    d->chart->GetAxis(vtkAxis::LEFT)->SetMargins(0, 0);
    d->chart->GetAxis(vtkAxis::LEFT)->GetPen()->SetColor(255, 255, 255);
    d->chart->GetAxis(vtkAxis::LEFT)->GetLabelProperties()->SetColor(1.0, 1.0, 1.0);
    d->chart->GetAxis(vtkAxis::LEFT)->GetTitleProperties()->SetColor(1.0, 1.0, 1.0);

    d->chart->GetAxis(vtkAxis::BOTTOM)->SetMargins(0, 0);
    d->chart->GetAxis(vtkAxis::BOTTOM)->GetPen()->SetColor(255, 255, 255);
    d->chart->GetAxis(vtkAxis::BOTTOM)->GetLabelProperties()->SetColor(1.0, 1.0, 1.0);
    d->chart->GetAxis(vtkAxis::BOTTOM)->GetTitleProperties()->SetColor(1.0, 1.0, 1.0);

    d->chart->SetShowLegend(d->show_legend);
    d->context_view->GetScene()->AddItem(d->chart);


    d->context_view->GetRenderWindow()->Render();
    //this->interactor()->Start();

    d->context_view->GetInteractor()->Initialize();
    d->context_view->GetInteractor()->Start();

    this->hud()->addInfo("Start by selecting x-axis and y-axis");
}

dtkVisualizationPlot2D::~dtkVisualizationPlot2D(void)
{
    delete d;
}

void dtkVisualizationPlot2D::exportAsCSV(const QString &filename)
{
    vtkSmartPointer<vtkDelimitedTextWriter> text_writer = vtkSmartPointer<vtkDelimitedTextWriter>::New();
    text_writer->SetInputData(d->fields_table);
    text_writer->SetFileName(filename.toStdString().c_str());
    text_writer->Write();
}

void dtkVisualizationPlot2D::exportAsPNG(const QString &filename)
{
    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
    windowToImageFilter->SetInput(d->context_view->GetRenderWindow());
    windowToImageFilter->SetInputBufferTypeToRGBA();
    windowToImageFilter->ReadFrontBufferOff();
    windowToImageFilter->Update();

    vtkSmartPointer<vtkPNGWriter> writer = vtkSmartPointer<vtkPNGWriter>::New();
    writer->SetFileName(filename.toStdString().c_str());
    writer->SetInputConnection(windowToImageFilter->GetOutputPort());
    writer->Write();
}


void dtkVisualizationPlot2D::render(void)
{
    if(d->field_x->currentText().isEmpty())
        return;

    d->chart->ClearPlots();
    for(int row=0; row < d->field_y->count(); ++row) {

        if(d->field_y->item(row)->checkState() == Qt::Checked) {
            dtkVisualizationPlot2DPrivate::FieldParameters param = d->fields_parameters[d->field_y->item(row)->text()];

            vtkPlot *points = d->chart->AddPlot(param.chart_type);
            points->SetInputData(d->fields_table,
                                 d->field_x->currentText().toStdString(),
                                 d->field_y->item(row)->text().toStdString());

            points->SetColor(param.color.red(), param.color.green(), param.color.blue(), 255);
            points->GetBrush()->SetColor(255, 64, 64);
            points->SetWidth(param.width);
            vtkPlotPoints::SafeDownCast(points)->SetMarkerStyle(param.marker_style);
        }
    }

    d->context_view->GetInteractor()->Render();
    //to update the range coordinates
    if(d->autoscale->isChecked()) {
        d->x_min->setText(QString::number(d->chart->GetAxis(vtkAxis::BOTTOM)->GetMinimum()));
        d->x_max->setText(QString::number(d->chart->GetAxis(vtkAxis::BOTTOM)->GetMaximum()));
        d->y_min->setText(QString::number(d->chart->GetAxis(vtkAxis::LEFT)->GetMinimum()));
        d->y_max->setText(QString::number(d->chart->GetAxis(vtkAxis::LEFT)->GetMaximum()));
    }
}

void dtkVisualizationPlot2D::setGridVisible(bool visible)
{
    d->chart->GetAxis(vtkAxis::LEFT)->SetGridVisible(visible);
    d->chart->GetAxis(vtkAxis::BOTTOM)->SetGridVisible(visible);
    d->chart->Modified();
}

QStringList dtkVisualizationPlot2D::fields(void)
{
    QStringList out;
    out << d->fields_table_model->stringList();

    for(int i=0; i<d->field_y->count(); ++i) {
        out << d->field_y->item(i)->text();
    }

    return out;
}

QStringList dtkVisualizationPlot2D::selectedFields(void)
{
    QStringList out;

    for(int i=0; i<d->field_y->count(); ++i) {
        auto *it =  d->field_y->item(i);
        if(it->checkState() == Qt::Checked)
            out << it->text();
    }

    return out;
}

void dtkVisualizationPlot2D::addFieldX(vtkSmartPointer<vtkAbstractArray> field)
{
    d->fields_table->AddColumn(field);
    d->field_x->addItem(field->GetName());
    d->field_x->model()->sort(0);
}

void dtkVisualizationPlot2D::addFieldY(vtkSmartPointer<vtkAbstractArray> field,
                                       bool checked,
                                       int chart_type,
                                       int marker_style,
                                       QColor color,
                                       int width)
{
    Q_ASSERT_X(chart_type >= 0 && chart_type < 7, "setChartType", "0: Line , 1: Points , 2: Bar , 3: Stacked , 4: Bag , 5: functionalBag , 6: Area  -> as in vtkPlot");
    Q_ASSERT_X(marker_style >= 0 && marker_style < 6, "setMarkerStyle", "NONE , CROSS , PLUS , SQUARE , CIRCLE , DIAMOND ");

    d->fields_table->AddColumn(field);
    dtkVisualizationPlot2DPrivate::FieldParameters param;
    param.chart_type = chart_type;
    param.marker_style = marker_style;
    param.width = width;

    if(color == Qt::transparent) {
        param.color = d->random();
    } else {
        param.color = color;
    }

    d->fields_parameters[field->GetName()] = param;

    QListWidgetItem *new_field_item = new QListWidgetItem(field->GetName(), d->field_y);
    new_field_item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    new_field_item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);

    d->field_y->sortItems();

    if(checked)
        this->render();
}

void dtkVisualizationPlot2D::clearFields(void)
{
    d->field_x->clear();

    d->field_y->blockSignals(true);
    d->field_y->clear();
    d->field_y->blockSignals(false);

    d->fields_parameters.clear();
    for(vtkIdType i=0; i < d->fields_table->GetNumberOfColumns(); ++i) {
        d->fields_table->RemoveColumn(i);
    }

    QMap<QString, dtkVisualizationPlot2DPrivate::FieldParameters >::const_iterator i = d->old_fields_param.constBegin();
    while (i != d->old_fields_param.constEnd()) {
        this->addFieldY(d->old_fields->GetArray(i.key().toStdString().c_str()),
                        false,
                        i.value().chart_type,
                        i.value().marker_style,
                        i.value().color);
        ++i;
    }
}

void dtkVisualizationPlot2D::removeField(vtkSmartPointer<vtkAbstractArray> field)
{
    this->removeField(field->GetName());
}

void dtkVisualizationPlot2D::removeField(const QString& field)
{
    int idx = d->field_x->findText(field);
    if(idx > 0) {
        d->field_x->removeItem(idx);
    }

    qDeleteAll(d->field_y->findItems(field, Qt::MatchFixedString));
    d->fields_parameters.remove(field);

    d->old_fields->RemoveArray(field.toStdString().c_str());
    d->old_fields_param.remove(field);

    d->fields_table->RemoveColumnByName(field.toStdString().c_str());
}


// void dtkVisualizationPlot2D::keyPressEvent(QKeyEvent *e)
// {
//     qDebug() << Q_FUNC_INFO << e->key() << Qt::Key_Escape ;
//     if(e->key() == 0x01000000) { //Qt::Key_escape
//         qDebug() << "gg";
//         d->save_field->setChecked(false);
//         d->remove_field->setChecked(false);
//         QGuiApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
//     } else {
//         dtkVisualizationCanvas::keyPressEvent(e);
//     }
// }

void dtkVisualizationPlot2D::setShowLegend(bool show)
{
    d->show_legend = show;
}

void dtkVisualizationPlot2D::setAddPlotSuffix(const QString& s)
{
    d->add_plot_suffix = s;
}

// ///////////////////////////////////////////////////////////////////

//#include "dtkVisualizationPlot2D.moc"

//
// dtkVisualizationPlot2D.cpp ends here
