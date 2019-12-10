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

#include "dtkVisualizationViewHybrid.h"

#include "dtkVisualizationViewOverlay.h"

#include <dtkVisualizationWidgets/dtkVisualizationWidgetsClutEditor>

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QSlider>

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCellPicker.h>
#include <vtkColorTransferFunction.h>
#include <vtkContourFilter.h>
#include <vtkDataArray.h>
#include <vtkDataSetMapper.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkImageActor.h>
#include <vtkImageBlend.h>
#include <vtkImageCast.h>
#include <vtkImageData.h>
#include <vtkImageMapToColors.h>
#include <vtkImageMapper3D.h>
#include <vtkImagePlaneWidget.h>
#include <vtkImageViewer2.h>
#include <vtkImageMapToColors.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleImage.h>
#include <vtkLookupTable.h>
#include <vtkObjectFactory.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkSphereSource.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkMetaImageReader.h>

#include <QVTKInteractor.h>
#include <QVTKOpenGLWidget.h>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationViewHybridPrivate : public QVTKOpenGLWidget
{
    Q_OBJECT

public:
    enum Orientation {
        SLICE_ORIENTATION_XY = 2,
        SLICE_ORIENTATION_XZ = 1,
        SLICE_ORIENTATION_YZ = 0
    };

public:
     dtkVisualizationViewHybridPrivate(QWidget *parent = nullptr);
    ~dtkVisualizationViewHybridPrivate(void);

public slots:
    void enableInteractor(void);
    void disableInteractor(void);

public:
    QSize sizeHint(void) const;

protected:
    void resizeEvent(QResizeEvent *);

public:
    Orientation orientation(void);

public:
    void setSliceOrientation(Orientation orientation);

public:
    void toggleChannel(const QString&);
    void activateChannel(const QString&);

public:
    void addImageActors(vtkImageData *);

public:
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> window;
    vtkSmartPointer<vtkRenderer> renderer2D;
    vtkSmartPointer<vtkRenderer> renderer3D;

public:
    dtkVisualizationViewHybrid *q = nullptr;

public:
    vtkSmartPointer<vtkImagePlaneWidget> planeWidget[3];

public:
    vtkSmartPointer<vtkImageMapToColors> image_color = nullptr;
    vtkSmartPointer<vtkImageViewer2>     viewer      = nullptr;

public:
    dtkVisualizationViewOverlay *renderer2D_button = nullptr;
    dtkVisualizationViewOverlay *renderer3D_button = nullptr;
    dtkVisualizationViewOverlay *renderer2D_XY     = nullptr;
    dtkVisualizationViewOverlay *renderer2D_XZ     = nullptr;
    dtkVisualizationViewOverlay *renderer2D_YZ     = nullptr;
    dtkVisualizationViewOverlay *sync              = nullptr;

public:
    int syncing_count     = 0;
    QTimer *syncing_timer = nullptr;
    bool synced           = false;

public:
    dtkVisualizationViewOverlay *bottom_left_button = nullptr;
    QWidget                     *bottom_left_widget = nullptr;

    dtkVisualizationViewOverlay *bottom_right_button = nullptr;
    QWidget                     *bottom_right_widget = nullptr;

    dtkVisualizationWidgetsClutEditor *clut_editor = nullptr;

public:
    QSlider     *slice_slider;

public:
    int x = 0, c_x = 0;
    int y = 0, c_y = 0;
    int z = 0, c_z = 0;

signals:
    void sliceOrientationChanged(int);
};

dtkVisualizationViewHybridPrivate::dtkVisualizationViewHybridPrivate(QWidget *parent) : QVTKOpenGLWidget(parent)
{
    this->setFormat(QVTKOpenGLWidget::defaultFormat());

    this->renderer2D = vtkSmartPointer<vtkRenderer>::New();
    this->renderer2D->SetBackground(0.290, 0.295, 0.300);

    this->renderer3D = vtkSmartPointer<vtkRenderer>::New();
    this->renderer3D->SetBackground(0.290, 0.295, 0.300);

    this->window = vtkGenericOpenGLRenderWindow::New();
    this->window->AddRenderer(this->renderer2D);
    this->window->AddRenderer(this->renderer3D);

    this->SetRenderWindow(this->window);
    this->setEnableHiDPI(true);

    this->renderer2D_button = new dtkVisualizationViewOverlay(fa::square, "", this);
    this->renderer2D_button->toggle(true);
    this->renderer3D_button = new dtkVisualizationViewOverlay(fa::cube, "", this);
    this->renderer2D_XY = new dtkVisualizationViewOverlay(fa::cube, "", this);
    this->renderer2D_XY->toggle(true);
    this->renderer2D_XZ = new dtkVisualizationViewOverlay(fa::cube, "", this);
    this->renderer2D_XZ->toggle(false);
    this->renderer2D_YZ = new dtkVisualizationViewOverlay(fa::cube, "", this);
    this->renderer2D_YZ->toggle(false);
    this->sync = new dtkVisualizationViewOverlay(fa::unlock, "", this);
    this->sync->toggle(false);

    // ///////////////////////////////////////////////////////////////////

    connect(this->sync, &dtkVisualizationViewOverlay::iconClicked, [=] () {

            this->sync->toggle(!this->sync->isToggled());

            if (this->sync->isToggled())
                emit q->linking();
            else
                emit q->unlinking();

            if (this->sync->isToggled() && !this->synced) {
                this->syncing_count = 0;
                if(!this->syncing_timer)
                    this->syncing_timer = new QTimer(this);
                connect(this->syncing_timer, &QTimer::timeout, [=] () {
                        this->sync->changeColor((this->syncing_count++ % 2) ? Qt::gray : Qt::white);
                        this->sync->update();
                        if (this->syncing_count == 11) {
                            this->sync->toggle(false);
                            this->syncing_timer->stop();
                            this->syncing_timer->disconnect();
                            delete this->syncing_timer;
                            this->syncing_timer = nullptr;
                            emit q->unlinking();
                        }
                    });
                this->syncing_timer->start(500);
            }
    });
}

dtkVisualizationViewHybridPrivate::~dtkVisualizationViewHybridPrivate(void)
{
}

void dtkVisualizationViewHybridPrivate::enableInteractor(void)
{
    this->GetInteractor()->Enable();
}

void dtkVisualizationViewHybridPrivate::disableInteractor(void)
{
    this->GetInteractor()->Disable();
}

QSize dtkVisualizationViewHybridPrivate::sizeHint(void) const
{
    return QSize(800, 600);
}

void dtkVisualizationViewHybridPrivate::resizeEvent(QResizeEvent *event)
{
    this->renderer2D_button->move(10, 10);
    this->renderer3D_button->move(50, 10);
    this->renderer2D_XY->move(10,  50);
    this->renderer2D_XZ->move(10,  90);
    this->renderer2D_YZ->move(10, 130);

    if(this->bottom_left_button) {
        this->bottom_left_button->move(10, event->size().height() - 40);
    }
    if(this->bottom_left_widget) {
        this->bottom_left_widget->move(10, event->size().height() - this->bottom_left_widget->sizeHint().height() - 50);
    }
    if(this->bottom_right_button) {
        this->bottom_right_button->move(event->size().width() - this->bottom_right_button->width() - 10, event->size().height() - 40);
    }
    if(this->bottom_right_widget) {
        this->bottom_right_widget->move(event->size().width() - this->bottom_right_widget->sizeHint().width() - 10, event->size().height() - this->bottom_right_widget->sizeHint().height() - 50);
    }

    this->sync->move(event->size().width() - 50, 10);

    QVTKOpenGLWidget::resizeEvent(event);
}

dtkVisualizationViewHybridPrivate::Orientation dtkVisualizationViewHybridPrivate::orientation(void)
{
    switch(this->viewer->GetSliceOrientation()) {
    case 2:
        return SLICE_ORIENTATION_XY;
    case 1:
        return SLICE_ORIENTATION_XZ;
    case 0:
        return SLICE_ORIENTATION_YZ;
    default:
        return SLICE_ORIENTATION_XY;
    }
}

void dtkVisualizationViewHybridPrivate::setSliceOrientation(Orientation orientation)
{
    if(!this->viewer) return;

    switch(orientation) {
    case SLICE_ORIENTATION_YZ:
        this->viewer->SetSliceOrientation(vtkImageViewer2::SLICE_ORIENTATION_YZ);
        emit sliceOrientationChanged(SLICE_ORIENTATION_YZ);
        break;
    case SLICE_ORIENTATION_XZ:
        this->viewer->SetSliceOrientation(vtkImageViewer2::SLICE_ORIENTATION_XZ);
        emit sliceOrientationChanged(SLICE_ORIENTATION_XZ);
        break;
    case SLICE_ORIENTATION_XY:
        this->viewer->SetSliceOrientation(vtkImageViewer2::SLICE_ORIENTATION_XY);
        emit sliceOrientationChanged(SLICE_ORIENTATION_XY);
        break;
    default:
        break;
    };
}

void dtkVisualizationViewHybridPrivate::addImageActors(vtkImageData *image)
{
    double bounds[2];
    image->GetPointData()->GetScalars()->GetRange(bounds);

    vtkColorTransferFunction *color_function = nullptr;
    if(this->clut_editor) {
        this->clut_editor->setRange(bounds[0], bounds[1]);
        color_function = static_cast<vtkColorTransferFunction*>(this->clut_editor->colorTransferFunction());
        if(!color_function)
            color_function = vtkColorTransferFunction::New();
    } else {
        color_function = vtkColorTransferFunction::New();
    }

    ////////// 2D
    if(!this->viewer) this->viewer = vtkSmartPointer<vtkImageViewer2>::New();
    this->viewer->SetSliceOrientationToXY();
    this->viewer->SetRenderWindow(this->window);
    this->viewer->SetRenderer(this->renderer2D);
    this->viewer->SetupInteractor(this->GetInteractor());
    this->viewer->GetWindowLevel()->SetOutputFormatToRGB();

    this->x = image->GetDimensions()[0];
    this->y = image->GetDimensions()[1];
    this->z = image->GetDimensions()[2];

    this->c_x = this->x/2;
    this->c_y = this->y/2;
    this->c_z = this->z/2;

    if(!this->image_color) this->image_color = vtkSmartPointer<vtkImageMapToColors>::New();
    this->image_color->SetLookupTable(color_function);
    this->image_color->SetOutputFormatToRGBA();
    this->image_color->SetInputData(image);
    this->image_color->Update();

    this->viewer->SetInputConnection(this->image_color->GetOutputPort());

    ////////// 3D
    int imageDims[3]; image->GetDimensions(imageDims);

    for(int i = 0; i < 3; i++) {

        if(!this->planeWidget[i])
            this->planeWidget[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();

        double color[3] = { 0, 0, 0 }; color[i] = 1;
        this->planeWidget[i]->SetInteractor(this->GetInteractor());
        this->planeWidget[i]->SetInputData(image);
        this->planeWidget[i]->SetPlaneOrientation(i);
        this->planeWidget[i]->RestrictPlaneToVolumeOn();
        this->planeWidget[i]->GetPlaneProperty()->SetColor(color);
        this->planeWidget[i]->SetLeftButtonAction(vtkImagePlaneWidget::VTK_SLICE_MOTION_ACTION);
        this->planeWidget[i]->SetMarginSizeX(0);
        this->planeWidget[i]->SetMarginSizeY(0);

        this->planeWidget[i]->SetSliceIndex(imageDims[i]/2);
        this->planeWidget[i]->DisplayTextOn();
    }

    if (this->renderer2D_XY->isToggled()) {
        this->planeWidget[0]->On();
        this->planeWidget[1]->On();
        this->planeWidget[2]->Off();

        this->viewer->SetSlice(this->z/2);
        this->slice_slider->setMaximum(this->z);
        this->slice_slider->blockSignals(true);
        this->slice_slider->setValue(this->z/2);
        this->slice_slider->blockSignals(false);
    }

    if (this->renderer2D_XZ->isToggled()) {
        this->planeWidget[0]->On();
        this->planeWidget[1]->Off();
        this->planeWidget[2]->On();

        this->viewer->SetSlice(this->y/2);
        this->slice_slider->setMaximum(this->y);
        this->slice_slider->blockSignals(true);
        this->slice_slider->setValue(this->y/2);
        this->slice_slider->blockSignals(false);
    }

    if (this->renderer2D_YZ->isToggled()) {
        this->planeWidget[0]->Off();
        this->planeWidget[1]->On();
        this->planeWidget[2]->On();

        this->viewer->SetSlice(this->x/2);
        this->slice_slider->setMaximum(this->x);
        this->slice_slider->blockSignals(true);
        this->slice_slider->setValue(this->x/2);
        this->slice_slider->blockSignals(false);
    }

    vtkSmartPointer<vtkSmartVolumeMapper> volume_mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();

    volume_mapper->SetInputData(image);
    volume_mapper->SetRequestedRenderMode(vtkSmartVolumeMapper::DefaultRenderMode);
    volume_mapper->Modified();
    volume_mapper->Update();

    vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();

    vtkSmartPointer<vtkPiecewiseFunction> opacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    opacity->AddPoint(   bounds[0],                0.00);
    opacity->AddPoint(1*(bounds[1]-bounds[0])/2/4, 0.00);
    opacity->AddPoint(   bounds[1],                1.00);

    vtkSmartPointer<vtkVolumeProperty> property = vtkSmartPointer<vtkVolumeProperty>::New();
    property->SetScalarOpacity(opacity);
    property->SetColor(color_function);
    property->ShadeOff();
    property->SetInterpolationType(VTK_LINEAR_INTERPOLATION);

    volume->SetMapper(volume_mapper);
    volume->SetProperty(property);
    volume->Modified();
    volume->Update();

    this->renderer3D->AddActor(volume);
    this->renderer2D->ResetCamera();
    this->renderer3D->ResetCamera();

    this->update();
}


dtkVisualizationViewHybrid::dtkVisualizationViewHybrid(QWidget *parent) : dtkWidgetsWidget(parent)
{
    d = new dtkVisualizationViewHybridPrivate;
    d->q = this;

    connect(d->renderer2D_button, SIGNAL(iconClicked()), this, SLOT(switchTo2D()));
    connect(d->renderer3D_button, SIGNAL(iconClicked()), this, SLOT(switchTo3D()));
    connect(d->renderer2D_XY, SIGNAL(iconClicked()), this, SLOT(switchTo2DXY()));
    connect(d->renderer2D_XZ, SIGNAL(iconClicked()), this, SLOT(switchTo2DXZ()));
    connect(d->renderer2D_YZ, SIGNAL(iconClicked()), this, SLOT(switchTo2DYZ()));

    d->slice_slider = new QSlider(this);
    d->slice_slider->setObjectName("Slider");
    d->slice_slider->setOrientation(Qt::Vertical);
    d->slice_slider->setMinimum(0);
    d->slice_slider->setMaximum(1);
    d->slice_slider->setValue(0);

    connect(d->slice_slider, SIGNAL(valueChanged(int)), this, SLOT(sliceChange(int)));


    // connect(d, static_cast<void(dtkVisualizationViewHybridPrivate::*)(int)>(&dtkVisualizationViewHybridPrivate::sliceOrientationChanged), this, &dtkVisualizationViewHybrid::sliceOrientationChanged);
    connect(d, &dtkVisualizationViewHybridPrivate::sliceOrientationChanged, this, &dtkVisualizationViewHybrid::sliceOrientationChanged);

    QGridLayout *layout  = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->slice_slider, 0, 0, 1, 1);
    layout->addWidget(d, 0, 1, 1, 1);

    this->setAcceptDrops(true);
}

dtkVisualizationViewHybrid::~dtkVisualizationViewHybrid(void)
{
    delete d;
}

QWidget *dtkVisualizationViewHybrid::widget(void)
{
    return d;
}

QWidget *dtkVisualizationViewHybrid::bottomLeftWidget(void)
{
    return d->bottom_left_widget;
}

void dtkVisualizationViewHybrid::setBottomLeftWidget(fa::icon icon, QWidget *widget)
{
    if(d->bottom_left_button) delete d->bottom_left_button;

    d->bottom_left_button = new dtkVisualizationViewOverlay(icon, "", this);

    d->bottom_left_widget = widget;
    d->bottom_left_widget->setParent(d);
    d->bottom_left_widget->hide();

    connect(d->bottom_left_button, &dtkVisualizationViewOverlay::iconClicked, [=] () {
            if(d->bottom_left_widget) {
                d->bottom_left_widget->setVisible(!d->bottom_left_button->isToggled());
                d->bottom_left_button->toggle(!d->bottom_left_button->isToggled());
            }
        });
}

QWidget *dtkVisualizationViewHybrid::bottomRightWidget(void)
{
    return d->bottom_right_widget;
}

void dtkVisualizationViewHybrid::setBottomRightWidget(fa::icon icon, QWidget *widget)
{
    if(d->bottom_right_button) delete d->bottom_right_button;

    d->bottom_right_button = new dtkVisualizationViewOverlay(icon, "", this);

    d->bottom_right_widget = widget;
    d->bottom_right_widget->setParent(d);
    d->bottom_right_widget->hide();

    connect(d->bottom_right_button, &dtkVisualizationViewOverlay::iconClicked, [=] () {
            if(d->bottom_right_widget) {
                d->bottom_right_widget->setVisible(!d->bottom_right_button->isToggled());
                d->bottom_right_button->toggle(!d->bottom_right_button->isToggled());
            }
        });
}

void dtkVisualizationViewHybrid::setClutEditor(dtkVisualizationWidgetsClutEditor *clut_editor)
{
    if (d->clut_editor) d->clut_editor->disconnect();

    d->clut_editor = clut_editor;

    QObject::connect(clut_editor, &dtkVisualizationWidgetsClutEditor::updated, [=] () {
            if(d->image_color) {
                d->image_color->SetLookupTable(static_cast<vtkColorTransferFunction *>(d->clut_editor->colorTransferFunction()));
                d->image_color->Update();
            }

            vtkVolumeCollection *volumes = d->renderer3D->GetVolumes();
            vtkVolume *volume = nullptr;
            for (int i = 0; i < volumes->GetNumberOfItems(); ++i) {
                if ((volume = vtkVolume::SafeDownCast(volumes->GetItemAsObject(i)))) {
                    volume->GetProperty()->SetColor(static_cast<vtkColorTransferFunction *>(d->clut_editor->colorTransferFunction()));
                    volume->GetProperty()->SetScalarOpacity(static_cast<vtkPiecewiseFunction *>(d->clut_editor->opacityTransferFunction()));
                }
            }

            d->GetInteractor()->Render();
        });
}

void dtkVisualizationViewHybrid::importVTI(const QString& path)
{
    qDebug() << Q_FUNC_INFO << path;
}

void dtkVisualizationViewHybrid::importVTP(const QString& path)
{
    qDebug() << Q_FUNC_INFO << path;
}

void dtkVisualizationViewHybrid::importVTR(const QString& path)
{
    qDebug() << Q_FUNC_INFO << path;
}

void dtkVisualizationViewHybrid::importVTS(const QString& path)
{
    qDebug() << Q_FUNC_INFO << path;
}

void dtkVisualizationViewHybrid::importVTU(const QString& path)
{
    qDebug() << Q_FUNC_INFO << path;
}

void dtkVisualizationViewHybrid::importMHA(const QString& path)
{
    vtkSmartPointer<vtkMetaImageReader> reader = vtkSmartPointer<vtkMetaImageReader>::New();
    reader->SetFileName(path.toStdString().c_str());
    reader->Update();
    vtkImageData *image = reader->GetOutput();
    if (!image) {
        qWarning() << Q_FUNC_INFO << "Resulting image is void.";
        return;
    }

    d->addImageActors(image);
}


void dtkVisualizationViewHybrid::switchCameraView(void)
{
    qDebug() << Q_FUNC_INFO << "to be implemented as an iteration over 3D/2D+X/2D-X/2D+Y/2D-Y/2D+Z/@D-Z";
}

void dtkVisualizationViewHybrid::setCameraViewNegativeX(void)
{
    double bounds[6]; d->renderer3D->ComputeVisiblePropBounds(bounds);

    double side = qAbs(bounds[1]-bounds[0]);

    double center[3];
    center[0] = (bounds[1]+bounds[0])/2;
    center[1] = (bounds[3]+bounds[2])/2;
    center[2] = (bounds[5]+bounds[4])/2;

    vtkCamera *camera = d->renderer3D->GetActiveCamera();
    camera->SetPosition(center[0]+side, center[1], center[2]);
    camera->SetFocalPoint(center[0], center[1], center[2]);
    camera->SetViewUp(0, 1, 0);

    d->renderer3D->ResetCamera();

    this->update();
}

void dtkVisualizationViewHybrid::setCameraViewPositiveX(void)
{
    double bounds[6]; d->renderer3D->ComputeVisiblePropBounds(bounds);

    double side = qAbs(bounds[1]-bounds[0]);

    double center[3];
    center[0] = (bounds[1]+bounds[0])/2;
    center[1] = (bounds[3]+bounds[2])/2;
    center[2] = (bounds[5]+bounds[4])/2;

    vtkCamera *camera = d->renderer3D->GetActiveCamera();
    camera->SetPosition(center[0]-side, center[1], center[2]);
    camera->SetFocalPoint(center[0], center[1], center[2]);
    camera->SetViewUp(0, 1, 0);

    d->renderer3D->ResetCamera();

    this->update();
}

void dtkVisualizationViewHybrid::setCameraViewNegativeY(void)
{
    double bounds[6]; d->renderer3D->ComputeVisiblePropBounds(bounds);

    double side = qAbs(bounds[3]-bounds[2]);

    double center[3];
    center[0] = (bounds[1]+bounds[0])/2;
    center[1] = (bounds[3]+bounds[2])/2;
    center[2] = (bounds[5]+bounds[4])/2;

    vtkCamera *camera = d->renderer3D->GetActiveCamera();
    camera->SetPosition(center[0], center[1]+side, center[2]);
    camera->SetFocalPoint(center[0], center[1], center[2]);
    camera->SetViewUp(0, 0, 1);

    d->renderer3D->ResetCamera();

    this->update();
}

void dtkVisualizationViewHybrid::setCameraViewPositiveY(void)
{
    double bounds[6]; d->renderer3D->ComputeVisiblePropBounds(bounds);

    double side = qAbs(bounds[3]-bounds[2]);

    double center[3];
    center[0] = (bounds[1]+bounds[0])/2;
    center[1] = (bounds[3]+bounds[2])/2;
    center[2] = (bounds[5]+bounds[4])/2;

    vtkCamera *camera = d->renderer3D->GetActiveCamera();
    camera->SetPosition(center[0], center[1]-side, center[2]);
    camera->SetFocalPoint(center[0], center[1], center[2]);
    camera->SetViewUp(0, 0, 1);

    d->renderer3D->ResetCamera();

    this->update();
}

void dtkVisualizationViewHybrid::setCameraViewNegativeZ(void)
{
    double bounds[6]; d->renderer3D->ComputeVisiblePropBounds(bounds);

    double side = qAbs(bounds[5]-bounds[4]);

    double center[3];
    center[0] = (bounds[1]+bounds[0])/2;
    center[1] = (bounds[3]+bounds[2])/2;
    center[2] = (bounds[5]+bounds[4])/2;

    vtkCamera *camera = d->renderer3D->GetActiveCamera();
    camera->SetPosition(center[0], center[1], center[2]+side);
    camera->SetFocalPoint(center[0], center[1], center[2]);
    camera->SetViewUp(0, 1, 0);

    d->renderer3D->ResetCamera();

    this->update();
}

void dtkVisualizationViewHybrid::setCameraViewPositiveZ(void)
{
    double bounds[6]; d->renderer3D->ComputeVisiblePropBounds(bounds);

    double side = qAbs(bounds[5]-bounds[4]);

    double center[3];
    center[0] = (bounds[1]+bounds[0])/2;
    center[1] = (bounds[3]+bounds[2])/2;
    center[2] = (bounds[5]+bounds[4])/2;

    vtkCamera *camera = d->renderer3D->GetActiveCamera();
    camera->SetPosition(center[0], center[1], center[2]-side);
    camera->SetFocalPoint(center[0], center[1], center[2]);
    camera->SetViewUp(0, 1, 0);

    d->renderer3D->ResetCamera();

    this->update();
}

void dtkVisualizationViewHybrid::link(dtkVisualizationViewHybrid *other)
{
    if (d->syncing_timer)
        d->syncing_timer->stop();

    d->sync->toggle(true);
    d->sync->changeIcon(fa::lock);

    d->synced = true;

    // ///////////////////////////////////////////////////////////////

    d->renderer2D->SetActiveCamera(other->d->renderer2D->GetActiveCamera());
    d->renderer3D->SetActiveCamera(other->d->renderer3D->GetActiveCamera());

    other->d->GetRenderWindow()->AddObserver(vtkCommand::RenderEvent, this, &dtkVisualizationViewHybrid::render);

    connect(other, SIGNAL(switchedTo3D()), this, SLOT(switchTo3D()));
    connect(other, SIGNAL(switchedTo2D()), this, SLOT(switchTo2D()));
    connect(other, SIGNAL(switchedTo2DXY()), this, SLOT(switchTo2DXY()));
    connect(other, SIGNAL(switchedTo2DXZ()), this, SLOT(switchTo2DXZ()));
    connect(other, SIGNAL(switchedTo2DYZ()), this, SLOT(switchTo2DYZ()));
    connect(other, SIGNAL(sliceChanged(int)), this, SLOT(sliceChange(int)));
}

void dtkVisualizationViewHybrid::unlink(dtkVisualizationViewHybrid *other)
{
    if (d->syncing_timer) {
        d->syncing_timer->stop();
        d->syncing_timer->disconnect();
        delete d->syncing_timer;
        d->syncing_timer = nullptr;
    }

    d->sync->toggle(false);
    d->sync->changeIcon(fa::unlock);

    d->synced = false;

    // ///////////////////////////////////////////////////////////////

    vtkSmartPointer<vtkCamera> camera2D = vtkCamera::New();
    camera2D->ShallowCopy(d->renderer2D->GetActiveCamera());

    vtkSmartPointer<vtkCamera> camera3D = vtkCamera::New();
    camera3D->ShallowCopy(d->renderer2D->GetActiveCamera());

    d->renderer2D->SetActiveCamera(camera2D);
    d->renderer3D->SetActiveCamera(camera3D);

    // ///////////////////////////////////////////////////////////////

    disconnect(other, SIGNAL(switchedTo3D()), this, SLOT(switchTo3D()));
    disconnect(other, SIGNAL(switchedTo2D()), this, SLOT(switchTo2D()));
    disconnect(other, SIGNAL(switchedTo2DXY()), this, SLOT(switchTo2DXY()));
    disconnect(other, SIGNAL(switchedTo2DXZ()), this, SLOT(switchTo2DXZ()));
    disconnect(other, SIGNAL(switchedTo2DYZ()), this, SLOT(switchTo2DYZ()));
    disconnect(other, SIGNAL(sliceChanged(int)), this, SLOT(sliceChange(int)));
}

void dtkVisualizationViewHybrid::update(void)
{
    bool enabled = d->GetInteractor()->GetEnabled();

    if(!enabled)
        d->GetInteractor()->Enable();

    d->GetInteractor()->Render();

    if(!enabled)
        d->GetInteractor()->Disable();
}

void dtkVisualizationViewHybrid::switchTo3D(void)
{
    d->renderer3D_button->toggle(true);

    d->renderer2D_button->toggle(false);
    d->renderer2D_XY->setVisible(false);
    d->renderer2D_XZ->setVisible(false);
    d->renderer2D_YZ->setVisible(false);

    d->renderer2D->DrawOff();
    d->renderer2D->InteractiveOff();

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();

    d->GetInteractor()->SetInteractorStyle(style);

    d->renderer3D->InteractiveOn();
    d->renderer3D->DrawOn();

    d->slice_slider->setEnabled(false);

    if(d->planeWidget[0]) d->planeWidget[0]->Off();
    if(d->planeWidget[1]) d->planeWidget[1]->Off();
    if(d->planeWidget[2]) d->planeWidget[2]->Off();

    if(d->planeWidget[0]) d->planeWidget[0]->On();
    if(d->planeWidget[1]) d->planeWidget[1]->On();
    if(d->planeWidget[2]) d->planeWidget[2]->On();

    this->render();

    emit switchedTo3D();
}

void dtkVisualizationViewHybrid::switchTo2D(void)
{
    d->renderer3D_button->toggle(false);

    d->renderer2D_button->toggle(true);
    d->renderer2D_XY->setVisible(true);
    d->renderer2D_XZ->setVisible(true);
    d->renderer2D_YZ->setVisible(true);

    d->renderer3D->DrawOff();
    d->renderer3D->InteractiveOff();

    vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
    d->GetInteractor()->SetInteractorStyle(style);

    d->renderer2D->InteractiveOn();
    d->renderer2D->DrawOn();

    d->slice_slider->setEnabled(true);

    if(d->planeWidget[0]) d->planeWidget[0]->Off();
    if(d->planeWidget[1]) d->planeWidget[1]->Off();
    if(d->planeWidget[2]) d->planeWidget[2]->Off();

    if (d->renderer2D_XY->isToggled()) {
        if(d->viewer) d->viewer->SetSlice(d->c_z);
        if(d->planeWidget[0]) d->planeWidget[0]->On();
        if(d->planeWidget[1]) d->planeWidget[1]->On();
        if(d->planeWidget[2]) d->planeWidget[2]->Off();
    }

    if (d->renderer2D_XZ->isToggled()) {
        if(d->viewer) d->viewer->SetSlice(d->c_y);
        if(d->planeWidget[0]) d->planeWidget[0]->On();
        if(d->planeWidget[1]) d->planeWidget[1]->Off();
        if(d->planeWidget[2]) d->planeWidget[2]->On();
    }

    if (d->renderer2D_YZ->isToggled()) {
        if(d->viewer) d->viewer->SetSlice(d->c_x);
        if(d->planeWidget[0]) d->planeWidget[0]->Off();
        if(d->planeWidget[1]) d->planeWidget[1]->On();
        if(d->planeWidget[2]) d->planeWidget[2]->On();
    }

    this->render();

    emit switchedTo2D();
}

void dtkVisualizationViewHybrid::switchTo2DXY(void)
{
    if (d->renderer2D_XY->isToggled()) return;

    d->renderer2D_XY->toggle(true);
    d->renderer2D_XZ->toggle(false);
    d->renderer2D_YZ->toggle(false);

    if(d->viewer) d->setSliceOrientation(dtkVisualizationViewHybridPrivate::SLICE_ORIENTATION_XY);

    if(d->viewer) d->viewer->SetSlice(d->c_z);

    if(d->planeWidget[0]) d->planeWidget[0]->On();
    if(d->planeWidget[1]) d->planeWidget[1]->On();
    if(d->planeWidget[2]) d->planeWidget[2]->Off();

    d->slice_slider->blockSignals(true);
    d->slice_slider->setMaximum(d->z);
    d->slice_slider->setValue(d->c_z);
    d->slice_slider->blockSignals(false);

    emit switchedTo2DXY();
}

void dtkVisualizationViewHybrid::switchTo2DXZ(void)
{
    if (d->renderer2D_XZ->isToggled()) return;

    d->renderer2D_XY->toggle(false);
    d->renderer2D_XZ->toggle(true);
    d->renderer2D_YZ->toggle(false);

    if(d->viewer) d->setSliceOrientation(dtkVisualizationViewHybridPrivate::SLICE_ORIENTATION_XZ);

    if(d->viewer) d->viewer->SetSlice(d->c_y);

    if(d->planeWidget[0]) d->planeWidget[0]->On();
    if(d->planeWidget[1]) d->planeWidget[1]->Off();
    if(d->planeWidget[2]) d->planeWidget[2]->On();

    d->slice_slider->blockSignals(true);
    d->slice_slider->setMaximum(d->y);
    d->slice_slider->setValue(d->c_y);
    d->slice_slider->blockSignals(false);

    emit switchedTo2DXZ();
}

void dtkVisualizationViewHybrid::switchTo2DYZ(void)
{
    if (d->renderer2D_YZ->isToggled()) return;

    d->renderer2D_XY->toggle(false);
    d->renderer2D_XZ->toggle(false);
    d->renderer2D_YZ->toggle(true);

    if(d->viewer) d->setSliceOrientation(dtkVisualizationViewHybridPrivate::SLICE_ORIENTATION_YZ);

    if(d->viewer) d->viewer->SetSlice(d->c_x);

    if(d->planeWidget[0]) d->planeWidget[0]->Off();
    if(d->planeWidget[1]) d->planeWidget[1]->On();
    if(d->planeWidget[2]) d->planeWidget[2]->On();

    d->slice_slider->blockSignals(true);
    d->slice_slider->setMaximum(d->x);
    d->slice_slider->setValue(d->c_x);
    d->slice_slider->blockSignals(false);

    emit switchedTo2DYZ();
}

void dtkVisualizationViewHybrid::sliceChange(int value)
{
    if (d->viewer) d->viewer->SetSlice(value);

    if (d->renderer2D_XY->isToggled()) {
        if(d->planeWidget[2]) d->planeWidget[2]->SetSliceIndex(value);
        d->c_z = value;
    }

    if (d->renderer2D_XZ->isToggled()) {
        if(d->planeWidget[1]) d->planeWidget[1]->SetSliceIndex(value);
        d->c_y = value;
    }

    if (d->renderer2D_YZ->isToggled()) {
        if(d->planeWidget[0]) d->planeWidget[0]->SetSliceIndex(value);
        d->c_x = value;
    }

    d->GetInteractor()->Render();

    d->slice_slider->blockSignals(true);
    d->slice_slider->setValue(value);
    d->slice_slider->blockSignals(false);

    emit sliceChanged(value);
}

vtkRenderWindowInteractor *dtkVisualizationViewHybrid::interactor(void)
{
    return d->GetInteractor();
}

vtkRenderer *dtkVisualizationViewHybrid::renderer2D(void)
{
    return d->renderer2D;
}

vtkRenderer *dtkVisualizationViewHybrid::renderer3D(void)
{
    return d->renderer3D;
}

void dtkVisualizationViewHybrid::render(void)
{
    d->GetInteractor()->Render();
}

void dtkVisualizationViewHybrid::onSliceChanged(int slice)
{
    d->slice_slider->setValue(slice);
}

void dtkVisualizationViewHybrid::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText()) {
        event->accept();
        return;
    }

    event->ignore();
}

void dtkVisualizationViewHybrid::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void dtkVisualizationViewHybrid::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}

void dtkVisualizationViewHybrid::dropEvent(QDropEvent *event)
{
    QString path = event->mimeData()->text();

    if(path.startsWith("file://")) path.remove("file://");
    if(path.endsWith("mha")) {
        this->importMHA(path);
    }

    event->accept();
}

QSlider *dtkVisualizationViewHybrid::sliceSlider(void)
{
    return d->slice_slider;
}

dtkVisualizationViewOverlay *dtkVisualizationViewHybrid::renderer2DXY(void)
{
    return d->renderer2D_XY;
}

dtkVisualizationViewOverlay *dtkVisualizationViewHybrid::renderer2DXZ(void)
{
    return d->renderer2D_XZ;
}

dtkVisualizationViewOverlay *dtkVisualizationViewHybrid::renderer2DYZ(void)
{
    return d->renderer2D_YZ;
}

dtkVisualizationViewOverlay *dtkVisualizationViewHybrid::renderer2DButton(void)
{
    return d->renderer2D_button;
}

dtkVisualizationViewOverlay *dtkVisualizationViewHybrid::renderer3DButton(void)
{
    return d->renderer3D_button;
}

// ///////////////////////////////////////////////////////////////////

#include "dtkVisualizationViewHybrid.moc"

//
// dtkVisualizationViewHybrid.cpp ends here
