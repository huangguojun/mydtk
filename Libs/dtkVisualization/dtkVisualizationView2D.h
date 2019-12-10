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

#include "dtkVisualizationCanvas.h"

class DTKVISUALIZATION_EXPORT dtkVisualizationView2D : public dtkVisualizationCanvas
{
    Q_OBJECT

public:
    using dtkVisualizationCanvas::dtkVisualizationCanvas;

public:
    ~dtkVisualizationView2D(void) = default;

public slots:
    void switchCameraView(void);
    void setCameraViewNegativeX(void);
    void setCameraViewPositiveX(void);
    void setCameraViewNegativeY(void);
    void setCameraViewPositiveY(void);
};

//
// dtkVisualizationView2D.h ends here
