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

#include "dtkVisualizationCanvas.h"

#include <dtkWidgets>

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

//#include <QVTKOpenGLWidget.h>
#include <QVTKWidget.h>

#include <vtkCamera.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkScalarBarActor.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>

#include <vtkAutoInit.h> 

//VTK_MODULE_INIT(vtkRenderingOpenGL)
//VTK_MODULE_INIT(vtkRenderingContextOpenGL)

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationCanvasPrivate
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationCanvasPrivate : public QVTKWidget
{
    Q_OBJECT

public:
     dtkVisualizationCanvasPrivate(QWidget *parent = nullptr);
    ~dtkVisualizationCanvasPrivate(void);

public:
    QSize sizeHint(void) const;

protected:
    void mousePressEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

public slots:
    void enableInteractor(void);
    void disableInteractor(void);

public slots:
    void onFocus(void);

public:
    //vtkSmartPointer<vtkGenericOpenGLRenderWindow> window;
    vtkSmartPointer<vtkRenderWindow> window;
    vtkSmartPointer<vtkRenderer> renderer;

public:
    dtkVisualizationCanvas *q = nullptr;

public:
    dtkWidgetsHUD *hud = nullptr;

public:
    std::size_t nb_scalar_bars = 0;
};

// ///////////////////////////////////////////////////////////////////

dtkVisualizationCanvasPrivate::dtkVisualizationCanvasPrivate(QWidget *parent) : QVTKWidget(parent)
{
   // this->setFormat(QVTKWidget::defaultFormat());
    this->renderer = vtkSmartPointer<vtkRenderer>::New();
    this->renderer->SetBackground(0.290, 0.295, 0.300);

    this->window = vtkRenderWindow::New();
    this->window->AddRenderer(this->renderer);

    this->SetRenderWindow(this->window);
   // this->setEnableHiDPI(true);
   // this->hud = new dtkWidgetsHUD(parent);
    this->setAcceptDrops(true);
    
}

dtkVisualizationCanvasPrivate::~dtkVisualizationCanvasPrivate()
{
}

void dtkVisualizationCanvasPrivate::enableInteractor(void)
{
    this->GetInteractor()->Enable();
}

void dtkVisualizationCanvasPrivate::disableInteractor(void)
{
    this->GetInteractor()->Disable();
}

QSize dtkVisualizationCanvasPrivate::sizeHint(void) const
{
    return QSize(800, 600);
}

void dtkVisualizationCanvasPrivate::resizeEvent(QResizeEvent *event)
{
    QVTKWidget::resizeEvent(event);

    //this->hud->resize(event->size());
}

void dtkVisualizationCanvasPrivate::mousePressEvent(QMouseEvent *event)
{
    q->emit focused();

    QVTKWidget::mousePressEvent(event);
}

void dtkVisualizationCanvasPrivate::onFocus(void)
{

}

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationCanvas
// ///////////////////////////////////////////////////////////////////

dtkVisualizationCanvas::dtkVisualizationCanvas(QWidget *parent) : dtkWidgetsWidget(parent), d(new dtkVisualizationCanvasPrivate(this))
{
    d->q = this;

    static int count = 1;
    this->setObjectName(QString("Canvas - %1").arg(count++));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(d);

   // connect(this, &dtkVisualizationCanvas::focused, d, &dtkVisualizationCanvasPrivate::onFocus);
}

dtkVisualizationCanvas::~dtkVisualizationCanvas(void)
{
    delete d;
}

QWidget *dtkVisualizationCanvas::widget(void)
{
    return d;
}

void dtkVisualizationCanvas::link(dtkVisualizationCanvas *other)
{
    
    d->renderer->SetActiveCamera(other->d->renderer->GetActiveCamera());

    other->d->window->AddObserver(vtkCommand::RenderEvent, this, &dtkVisualizationCanvas::update);

    //d->hud->addInfo("Linked");

    this->update();
    
}

void dtkVisualizationCanvas::unlink(void)
{
    
    vtkSmartPointer<vtkCamera> camera = vtkCamera::New();
    camera->ShallowCopy(d->renderer->GetActiveCamera());

    d->renderer->SetActiveCamera(camera);

   // d->hud->addInfo("Unlinked");

    this->update();
}

dtkWidgetsHUD *dtkVisualizationCanvas::hud(void)
{
    return d->hud;
}

void dtkVisualizationCanvas::reset(void)
{
    
    if (d->renderer) {
        d->renderer->ResetCamera();
    }
}

void dtkVisualizationCanvas::draw(void)
{
    if (d->GetInteractor()) {
        d->GetInteractor()->Render();
    }

    if (d->renderer) {
         d->renderer->GetRenderWindow()->Render();
     }
     
}

vtkRenderer *dtkVisualizationCanvas::renderer(void)
{
    return d->renderer;
}

vtkRenderWindowInteractor *dtkVisualizationCanvas::interactor(void)
{
    return d->GetInteractor();
}

void dtkVisualizationCanvas::addScalarBar(vtkScalarBarActor *scalar_bar)
{
    
    ++d->nb_scalar_bars;

    std::size_t orientation = d->nb_scalar_bars % 2; // 0: horizontal, 1: vertical
    std::size_t side        = (d->nb_scalar_bars % 4) >= 2; // 0: right, 1: left
    std::size_t loop        = d->nb_scalar_bars / 4;

    scalar_bar->SetOrientation(orientation);

    double xref = 0.9;
    double yref = 1;
    double width  = 0.4;
    double height = 0.1;
    if(orientation) {
        width = 0.1;
        height = 0.4;
        xref = 1.;
        yref = 0.9;
    }

    scalar_bar->SetWidth(width);
    scalar_bar->SetHeight(height);

    double x = (xref - width) * double(!side && !orientation) + (xref - width) * double(!side && orientation);
    double y = (yref - height) * double(!side && orientation)  + (yref - height) * double(side && !orientation);

    scalar_bar->SetPosition(x, y);

    if(this->renderer()) {
        this->renderer()->AddActor2D(scalar_bar);
    }
    
}

void dtkVisualizationCanvas::scale(double x, double y, double z)
{
    
    if (x >= 0.01 && y >=0.01 && z >= 0.01)  {
        vtkCamera *camera = d->renderer->GetActiveCamera();
        vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
        transform->Scale(x, y, z);
        camera->SetModelTransformMatrix(transform->GetMatrix());
        this->draw();
    }
}

void dtkVisualizationCanvas::removeScalarBar(vtkScalarBarActor *scalar_bar)
{
    
    if(this->renderer()) {
        this->renderer()->RemoveActor2D(scalar_bar);
    }
}

// ///////////////////////////////////////////////////////////////////

#include "dtkVisualizationCanvas.moc"

//
// dtkVisualizationPlot2D.cpp ends here
