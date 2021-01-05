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

#pragma once

#include <dtkVisualizationExport>

#include <dtkWidgets/dtkWidgetsWidget>

class vtkRenderer;

class dtkWidgetsOverlayPane;
class dtkWidgetsHUD;

class DTKVISUALIZATION_EXPORT dtkVisualizationPlot3D : public dtkWidgetsWidget
{
    Q_OBJECT

public:
    dtkVisualizationPlot3D(QWidget *parent = nullptr);
    ~dtkVisualizationPlot3D(void);

public:
    QWidget *widget(void) override;

public slots:
    void update(void) override;

public:
    //  dtkWidgetsOverlayPane *overlay(void);
    //   dtkWidgetsHUD *hud(void);

public:
    vtkRenderer *renderer(void);

private:
    class dtkVisualizationPlot3DPrivate *d;
};

//
// dtkVisualizationPlot3D.h ends here
