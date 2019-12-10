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

#include "dtkVisualizationView2D.h"

class DTKVISUALIZATION_EXPORT dtkVisualizationView3D : public dtkVisualizationView2D
{
    Q_OBJECT

public:
    using dtkVisualizationView2D::dtkVisualizationView2D;

public:
    ~dtkVisualizationView3D(void) = default;

public slots:
    void setCameraViewNegativeZ(void);
    void setCameraViewPositiveZ(void);
};

//
// dtkVisualizationView3D.h ends here
