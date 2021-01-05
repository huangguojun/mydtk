
#pragma once

#include <dtkVisualizationExport>
#include <dtkWidgets/dtkWidgetsWidget>

class vtkRenderer;
class vtkRenderWindowInteractor;
class vtkScalarBarActor;

class dtkWidgetsHUD;
class QWidget;

class DTKVISUALIZATION_EXPORT dtkVisualizationCanvas : public dtkWidgetsWidget
{
    Q_OBJECT

public:
    dtkVisualizationCanvas(QWidget *parent = nullptr);
    ~dtkVisualizationCanvas(void);

public:
    QWidget *widget(void);

public slots:
    void link(dtkVisualizationCanvas *other);
    void unlink(void);

public:
    dtkWidgetsHUD *hud(void);

public:
    void reset(void);
    void draw(void);
    void scale(double x, double y, double z);

public:
    vtkRenderer *renderer(void);
    vtkRenderWindowInteractor *interactor(void);

public:
    void addScalarBar(vtkScalarBarActor *);
    void removeScalarBar(vtkScalarBarActor *);

private:
    class dtkVisualizationCanvasPrivate *d;
};

//
// dtkVisualizationCanvas.h ends here
