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

#include "dtkVisualizationView3D.h"

#include <vtkCamera.h>
#include <vtkRenderer.h>

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationView3D
// ///////////////////////////////////////////////////////////////////

void dtkVisualizationView3D::setCameraViewNegativeZ(void)
{
    double bounds[6]; this->renderer()->ComputeVisiblePropBounds(bounds);

    double side = std::abs(bounds[5]-bounds[4]);

    double center[3];
    center[0] = (bounds[1]+bounds[0])/2;
    center[1] = (bounds[3]+bounds[2])/2;
    center[2] = (bounds[5]+bounds[4])/2;

    vtkCamera *camera = this->renderer()->GetActiveCamera();
    camera->SetPosition(center[0], center[1], center[2]+side);
    camera->SetFocalPoint(center[0], center[1], center[2]);
    camera->SetViewUp(0, 1, 0);

    this->renderer()->ResetCamera();

    this->update();
}

void dtkVisualizationView3D::setCameraViewPositiveZ(void)
{
    double bounds[6]; this->renderer()->ComputeVisiblePropBounds(bounds);

    double side = std::abs(bounds[5]-bounds[4]);

    double center[3];
    center[0] = (bounds[1]+bounds[0])/2;
    center[1] = (bounds[3]+bounds[2])/2;
    center[2] = (bounds[5]+bounds[4])/2;

    vtkCamera *camera = this->renderer()->GetActiveCamera();
    camera->SetPosition(center[0], center[1], center[2]-side);
    camera->SetFocalPoint(center[0], center[1], center[2]);
    camera->SetViewUp(0, 1, 0);

    this->renderer()->ResetCamera();

    this->update();
}

//
// dtkVisualizationView3D.cpp ends here
