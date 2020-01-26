
#include "dtkVisualizationViewer.h"
#include "dtkVisualizationViewerStack.h"

#include <dtkCore>

#include <dtkWidgets>

#include <dtkVisualization>

#include <dtkWidgets/dtkVisualizationWidgetsClutEditor>

#include <vtkCamera.h>
#include <vtkCellData.h>
#include <vtkDataSet.h>
#include <vtkDataArray.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>

#include <QtCore>

#include <math.h>

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationViewerPrivate
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationViewerPrivate
{
public:
    dtkVisualizationViewerStack *stack = nullptr;
    dtkVisualizationDecoratorCollection collection;
    vtkDataSet *dataset = nullptr;
    double v_max = 100.;

public:
    void createPlot2DData(dtkVisualizationPlot2D *plot);
    void drunkData(double time);
};


void dtkVisualizationViewerPrivate::drunkData(double val)
{
    
    if(!dataset) {
        qInfo() << "import a dataset before calling this method";
    }

    //qDebug() << "val" << val;

    if(val == 1.) {
        qDebug() << "nb points array" << dataset->GetPointData()->GetNumberOfArrays();
        qDebug() << "nb cells  array" << dataset->GetCellData()->GetNumberOfArrays();
    }

    vtkIdType nb_points = dataset->GetNumberOfPoints();
    vtkIdType nb_cells = dataset->GetNumberOfCells();

    double xyz[3];
    double new_value[3] = {val, 1.5*val, -1.*val};

    double bounds[6]; //x_min , x_max , y_min , y_max , z_min , z_max
    dataset->GetBounds(bounds);

    {
        vtkPointData *point_data = dataset->GetPointData();
        vtkIdType nb_arrays = point_data->GetNumberOfArrays();

        for (vtkIdType i = 0; i < nb_arrays; ++i) {
            vtkDataArray *array = point_data->GetArray(i);
            vtkIdType nb_tuples = array->GetNumberOfTuples();
            vtkIdType nb_cmpts = array->GetNumberOfComponents();

            if (nb_cmpts == 1) {
                for(vtkIdType t=0; t < nb_tuples; ++t) {
                    dataset->GetPoint(t, xyz);
                    double value = val * std::cos(val + ((xyz[0] - bounds[0])/bounds[1]));
                    array->SetTuple1(t, value);
                }
            } else if (nb_cmpts == 3) {
                for(vtkIdType t=0; t < nb_tuples; ++t) {
                    dataset->GetPoint(t, xyz);
                    new_value[0] = val * std::cos(val + ((xyz[0] - bounds[0])/bounds[1]));
                    new_value[1] = val * std::cos(val + ((xyz[1] - bounds[2])/bounds[3]));
                    new_value[2] = val * std::cos(val + ((xyz[2] - bounds[4])/bounds[5]));
                    array->SetTuple3(t, new_value[0], new_value[1], new_value[2]);
                }
            }
        }
    }
    {
        vtkCellData *cell_data = dataset->GetCellData();
        vtkIdType nb_arrays = cell_data->GetNumberOfArrays();

        for (vtkIdType i = 0; i < nb_arrays; ++i) {
            vtkDataArray *array = cell_data->GetArray(i);
            vtkIdType nb_tuples = array->GetNumberOfTuples();
            vtkIdType nb_cmpts = array->GetNumberOfComponents();

            if (nb_cmpts == 1) {
                for(vtkIdType t=0; t < nb_tuples; ++t) {
                    /*
                    double cell_bounds[6];
                    dataset->GetCell(t)->GetBounds(cell_bounds);
                    double value = val * std::cos(val + ((cell_bounds[0] - bounds[0])/bounds[1]));
                    array->SetTuple1(t, value);
                    */
                }
            } else if (nb_cmpts == 3) {
                for(vtkIdType t=0; t < nb_tuples; ++t) {
                    double cell_bounds[6];
                   /*
                    dataset->GetCell(t)->GetBounds(cell_bounds);

                    new_value[0] = val * std::cos(val + ((cell_bounds[0] - bounds[0])/bounds[1]));
                    new_value[1] = val * std::cos(val + ((cell_bounds[2] - bounds[2])/bounds[3]));
                    new_value[2] = val * std::cos(val + ((cell_bounds[4] - bounds[4])/bounds[5]));
                    array->SetTuple3(t, new_value[0], new_value[1], new_value[2]);
                    */
                }
            }
        }
    }

}



void dtkVisualizationViewerPrivate::createPlot2DData(dtkVisualizationPlot2D *plot)
{
    
    static bool plot_is_init = false;
    if(plot_is_init)
        return;

    int MAX = 5;
    double pi = 3.1516;
    vtkSmartPointer<vtkDoubleArray> field_1 = vtkSmartPointer<vtkDoubleArray>::New();
    field_1->SetName("Linear");
    field_1->SetNumberOfComponents(1);
    field_1->SetNumberOfTuples(MAX * 1e2);

    vtkSmartPointer<vtkDoubleArray> field_2 = vtkSmartPointer<vtkDoubleArray>::New();
    field_2->SetName("Fake EEG");
    field_2->SetNumberOfComponents(1);
    field_2->SetNumberOfTuples(MAX * 1e2);

    vtkSmartPointer<vtkDoubleArray> field_3 = vtkSmartPointer<vtkDoubleArray>::New();
    field_3->SetName("third_field");
    field_3->SetNumberOfComponents(1);
    field_3->SetNumberOfTuples(MAX * 1e2);

    for(double i = 0; i < MAX; i += 1e-2) {
        field_1->SetValue(i * 1e2, i);
        field_2->SetValue(i * 1e2, cos(M_PI*6*i) + 2*sin(M_PI*2*i));
        field_3->SetValue(i * 1e2, rand() % 10);
    }

    plot->addFieldX(field_1);
    plot->addFieldY(field_2, true, vtkChart::LINE, vtkPlotPoints::NONE);
    plot->addFieldY(field_3);

    plot_is_init = true;
    
}
    

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationViewer
// ///////////////////////////////////////////////////////////////////

dtkVisualizationViewer::dtkVisualizationViewer(QWidget *parent) 
    : QMainWindow(parent), d(new dtkVisualizationViewerPrivate)
{
    
    dtk::visualization::initialize();
    QWidget *widget = new QWidget(this);
    d->stack = new dtkVisualizationViewerStack(this);

    QHBoxLayout *layout = new QHBoxLayout(widget);
    //layout->setContentsMargins(this->menubar()->width(), 0, 0, 0);
    layout->setSpacing(0);

    layout->addWidget(d->stack);

    
    //dtkWidgetsParameterMenuBarGenerator menubar_generator(":menu.json", ":parameters.json");
    //menubar_generator.populate(this->menubar());
    //this->populate();

    //this->menubar()->touch();

  //  QHash<QString, dtkCoreParameter *> parameters = menubar_generator.parameters();

    // Restore last path used from the settings
  /*
    QSettings settings;
    settings.beginGroup("viewer");
    QString default_path = settings.value("last_import_dir").toString();
//    if (!default_path.isEmpty())
  //      parameters["open"]->setValue(default_path);
    settings.endGroup();

    dtkCoreParameter *open_parameter = parameters["open"];
    if(open_parameter) {
        open_parameter->connect([=] (QVariant v) {
                QString path = v.value<dtk::d_path>().path();
                if(path.isEmpty()) return;
                importDataSet(v.value<dtk::d_path>().path());
            });
    }
    dtkCoreParameter *plotdata_parameter = parameters["plotdata"];
    if(plotdata_parameter) {
        plotdata_parameter->connect([=] (QVariant v) {
                dtkVisualizationPlot2D *plot = dynamic_cast<dtkVisualizationPlot2D *>(d->stack->layout()->current()->view());

                if(plot)
                    d->createPlot2DData(plot);
            });
    }

    dtkCoreParameter *zscale_parameter = parameters["zscale"];
    if (zscale_parameter) {
        zscale_parameter->connect([=] (QVariant v) {
                                      applyScale(1.0, 1.0, v.value<dtk::d_real>().value());
                                  });
        }
    //for animating data
    // 20s to go from 0 to d->v_max
    QTimeLine *timeline = new QTimeLine(20000, this); // 20 s
    timeline->setFrameRange(0, d->v_max);
    connect(timeline, &QTimeLine::frameChanged,
            [=](int time) {
                d->drunkData(time);
                emit updated();
            });


    dtkCoreParameter *drunkdata_parameter = parameters["drunkdata"];
    if(drunkdata_parameter) {
        drunkdata_parameter->connect([=] (QVariant v) {
                 if(v.value<dtk::d_bool>().value())
                     timeline->start();
            });
    }

*/
    
    setMinimumSize (800, 600);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setCentralWidget(widget);
    this->setup();
}

dtkVisualizationViewer::~dtkVisualizationViewer(void)
{
//    this->setdw();

  //  delete d;
}

void dtkVisualizationViewer::applyScale(double x, double y, double z)
{
    
    dtkVisualizationCanvas *canvas = dynamic_cast<dtkVisualizationCanvas *>(d->stack->layout()->current()->view());
    if (!canvas)
            return;

    canvas->scale(x,y,z);
    
}

void dtkVisualizationViewer::importDataSet(const QString& path)
{
    
    d->collection.clear();

    QFileInfo info(path);

    QSettings settings;
    settings.beginGroup("viewer");
    settings.setValue("last_import_dir", info.absoluteDir().path());
    settings.endGroup();

    dtkVisualizationCanvas *canvas = dynamic_cast<dtkVisualizationCanvas *>(d->stack->layout()->current()->view());

    if(!canvas)
        return;

    d->dataset = dtkVisualizationVTKReader(path);

    if (!d->dataset) {
        QMessageBox msgBox;
        msgBox.setText("Fail to read file, check file extension");
        msgBox.exec();
        return;
    }
/*
    QVariant data = dtk::variantFromValue(d->dataset);

    dtkVisualizationDecoratorInfo              *decorator_info               = new dtkVisualizationDecoratorInfo;
    dtkVisualizationDecoratorAxes              *decorator_axes               = new dtkVisualizationDecoratorAxes;
    dtkVisualizationDecoratorDelaunay2D        *decorator_delaunay_2D        = new dtkVisualizationDecoratorDelaunay2D;
    dtkVisualizationDecoratorPoints            *decorator_points             = new dtkVisualizationDecoratorPoints;
    dtkVisualizationDecoratorScalarColorMap    *decorator_scalar_colormap    = new dtkVisualizationDecoratorScalarColorMap;
    dtkVisualizationDecoratorScalarGlyphs      *decorator_scalar_glyphs      = new dtkVisualizationDecoratorScalarGlyphs;
    dtkVisualizationDecoratorScalarIsolines    *decorator_scalar_isolines    = new dtkVisualizationDecoratorScalarIsolines;
    dtkVisualizationDecoratorSlices            *decorator_slices             = new dtkVisualizationDecoratorSlices;
    dtkVisualizationDecoratorSnapshot          *decorator_snapshot           = new dtkVisualizationDecoratorSnapshot;
    dtkVisualizationDecoratorVectorStreamlines *decorator_vector_streamlines = new dtkVisualizationDecoratorVectorStreamlines;
    dtkVisualizationDecoratorVolume            *decorator_volume             = new dtkVisualizationDecoratorVolume;

    QList<dtkVisualizationDecorator *> decorators;
    decorators << decorator_points
               << decorator_delaunay_2D
               << decorator_scalar_colormap
               << decorator_scalar_isolines
               << decorator_scalar_glyphs
               << decorator_vector_streamlines
               << decorator_volume
               << decorator_slices
               << decorator_axes
               << decorator_snapshot
        ;

    for (auto dec : decorators) {
        dec->setData(data);
        if (dec->isDecorating()) {
            d->collection << dec;
        } else {
            delete dec;
        }
    }
    decorators.clear();

    d->collection.setCanvas(canvas);

    decorator_info->setData(data);
    decorator_info->setCanvas(canvas);
    decorator_info->setFileInfo(info);

    dtkWidgetsMenuBar *menu_bar = dtkApp->window()->menubar();

    dtkWidgetsMenu *canvas_menu = menu_bar->menu("View " + canvas->objectName());

    Q_ASSERT(canvas_menu);

    if (decorator_info->isDecorating()) {
        for(auto inspector : decorator_info->inspectors()) {
            dtkWidgetsMenuItemDIY *item = new dtkWidgetsMenuItemDIY(inspector->objectName(), inspector);

            item->setSizePolicy(QSizePolicy::Expanding);

            canvas_menu->addItem(item);
        }
    }

    for(auto decorator : d->collection) {

        dtkWidgetsMenu *menu = canvas_menu->menu(decorator->objectName());

        if(!menu) {
            menu = canvas_menu->addMenu(0, decorator->objectName());
        }

        for(auto inspector : decorator->inspectors()) {
            dtkWidgetsMenuItemDIY *item = new dtkWidgetsMenuItemDIY(inspector->objectName(), inspector);

            menu->addItem(item);
        }

    }

    menu_bar->touch();

    d->collection.draw();


    //TODO complete
    connect(this, &dtkVisualizationViewer::updated,
            [=] (void) {
                d->collection.draw();
            });
    */
}

void dtkVisualizationViewer::setup(void)
{
    QSettings settings;
    settings.beginGroup("viewer");
    this->move(settings.value("position").toPoint());
    this->resize(settings.value("size").toSize());
    settings.endGroup();
}

void dtkVisualizationViewer::setdw(void)
{
    QSettings settings;
    settings.beginGroup("viewer");
    settings.setValue("position", this->pos());
    settings.setValue("size", this->size());
    settings.endGroup();
}

void dtkVisualizationViewer::keyPressEvent(QKeyEvent *event)
{
    QMainWindow::keyPressEvent(event);
}

void dtkVisualizationViewer::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
}

void dtkVisualizationViewer::mouseMoveEvent(QMouseEvent *event)
{
    // do not show title bar when over the menu. use hardcoded values of menu :-(
    if (event->pos().x() < 300 + 32 + 12)
        return;

    QMainWindow::mouseMoveEvent(event);
}
//
// dtkVisualizationViewer.cpp ends here
