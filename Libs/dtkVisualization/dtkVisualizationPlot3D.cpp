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

#include "dtkVisualizationPlot3D.h"

//#include <dtkWidgets/dtkWidgetsHUD>
//#include <dtkWidgets/dtkWidgetsHUDItem>
//#include <dtkWidgets/dtkWidgetsOverlayPane>

#include <QtWidgets>

#include <vtkCamera.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>

//#include <QVTKOpenGLWidget.h>
#include <QVTKWidget.h>

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationPlot3DPrivate
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationPlot3DPrivate : public QVTKWidget
{
    Q_OBJECT

public:
     dtkVisualizationPlot3DPrivate(QWidget *parent = nullptr);
    ~dtkVisualizationPlot3DPrivate(void);

public:
    QSize sizeHint(void) const;

public:
    vtkSmartPointer<vtkRenderWindow> window;
    vtkSmartPointer<vtkRenderer> renderer;

protected:
    void mousePressEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

public:
    dtkVisualizationPlot3D *q = nullptr;

public:
    //dtkWidgetsHUD *hud;
    //dtkWidgetsOverlayPane *overlay;
};

dtkVisualizationPlot3DPrivate::dtkVisualizationPlot3DPrivate(QWidget *parent) : QVTKWidget(parent)
{
   // this->setFormat(QVTKOpenGLWidget::defaultFormat());

    this->renderer = vtkSmartPointer<vtkRenderer>::New();
    this->renderer->SetBackground(0.290, 0.295, 0.300);

    this->window = vtkRenderWindow::New();
    this->window->AddRenderer(this->renderer);

    this->SetRenderWindow(this->window);
    //this->setEnableHiDPI(true);

    //this->hud = new dtkWidgetsHUD(this);
    //this->overlay = new dtkWidgetsOverlayPane(this);
}

dtkVisualizationPlot3DPrivate::~dtkVisualizationPlot3DPrivate(void)
{

}

QSize dtkVisualizationPlot3DPrivate::sizeHint(void) const
{
    return QSize(800, 600);
}

void dtkVisualizationPlot3DPrivate::mousePressEvent(QMouseEvent *event)
{
    q->emit focused();

    QVTKWidget::mousePressEvent(event);
}

void dtkVisualizationPlot3DPrivate::resizeEvent(QResizeEvent *event)
{
    QVTKWidget::resizeEvent(event);

    //this->hud->resize(event->size());

    //this->overlay->setFixedHeight(event->size().height());
}

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationPlot3D
// ///////////////////////////////////////////////////////////////////

dtkVisualizationPlot3D::dtkVisualizationPlot3D(QWidget *parent) : dtkWidgetsWidget(parent), d(new dtkVisualizationPlot3DPrivate(this))
{
    d->q = this;

    static int count = 1;
    this->setObjectName(QString("Plot3D - %1").arg(count++));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(d);

    //dtkWidgetsHUDItem *settings = d->hud->addItem(fa::sliders);
    //settings->setToolTip("Settings");

    //connect(settings, SIGNAL(clicked()), d->overlay, SLOT(toggle()));

    this->setAcceptDrops(true);
}

dtkVisualizationPlot3D::~dtkVisualizationPlot3D(void)
{
    delete d;
}

QWidget *dtkVisualizationPlot3D::widget(void)
{
    return d;
}

void dtkVisualizationPlot3D::update(void)
{
    d->GetInteractor()->Render();
}

/*
dtkWidgetsOverlayPane *dtkVisualizationPlot3D::overlay(void)
{
    return NULL;
    //return d->overlay;
}

dtkWidgetsHUD *dtkVisualizationPlot3D::hud(void)
{
    return NULL;
  //  return d->hud;
}
*/
vtkRenderer *dtkVisualizationPlot3D::renderer(void)
{
    return d->renderer;
}

// ///////////////////////////////////////////////////////////////////

#include "dtkVisualizationPlot3D.moc"

//
// dtkVisualizationPlot3D.cpp ends here
