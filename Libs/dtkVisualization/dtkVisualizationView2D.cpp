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

#include "dtkVisualizationView2D.h"

#include <QtCore>

#include <vtkCamera.h>
#include <vtkRenderer.h>

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationView2D
// ///////////////////////////////////////////////////////////////////

void dtkVisualizationView2D::switchCameraView(void)
{
#pragma message ""
    qDebug() << Q_FUNC_INFO << "to be implemented as an iteration over 3D/2D+X/2D-X/2D+Y/2D-Y/2D+Z/@D-Z";
}

void dtkVisualizationView2D::setCameraViewNegativeX(void)
{
    double bounds[6]; this->renderer()->ComputeVisiblePropBounds(bounds);

    double side = std::abs(bounds[1]-bounds[0]);

    double center[3];
    center[0] = (bounds[1]+bounds[0])/2;
    center[1] = (bounds[3]+bounds[2])/2;
    center[2] = (bounds[5]+bounds[4])/2;

    vtkCamera *camera = this->renderer()->GetActiveCamera();
    camera->SetPosition(center[0]+side, center[1], center[2]);
    camera->SetFocalPoint(center[0], center[1], center[2]);
    camera->SetViewUp(0, 1, 0);

    this->renderer()->ResetCamera();

    this->update();
}

void dtkVisualizationView2D::setCameraViewPositiveX(void)
{
    double bounds[6]; this->renderer()->ComputeVisiblePropBounds(bounds);

    double side = std::abs(bounds[1]-bounds[0]);

    double center[3];
    center[0] = (bounds[1]+bounds[0])/2;
    center[1] = (bounds[3]+bounds[2])/2;
    center[2] = (bounds[5]+bounds[4])/2;

    vtkCamera *camera =this->renderer()->GetActiveCamera();
    camera->SetPosition(center[0]-side, center[1], center[2]);
    camera->SetFocalPoint(center[0], center[1], center[2]);
    camera->SetViewUp(0, 1, 0);

    this->renderer()->ResetCamera();

    this->update();
}

void dtkVisualizationView2D::setCameraViewNegativeY(void)
{
    double bounds[6];this->renderer()->ComputeVisiblePropBounds(bounds);

    double side = std::abs(bounds[3]-bounds[2]);

    double center[3];
    center[0] = (bounds[1]+bounds[0])/2;
    center[1] = (bounds[3]+bounds[2])/2;
    center[2] = (bounds[5]+bounds[4])/2;

    vtkCamera *camera =this->renderer()->GetActiveCamera();
    camera->SetPosition(center[0], center[1]+side, center[2]);
    camera->SetFocalPoint(center[0], center[1], center[2]);
    camera->SetViewUp(0, 0, 1);

    this->renderer()->ResetCamera();

    this->update();
}

void dtkVisualizationView2D::setCameraViewPositiveY(void)
{
    double bounds[6];this->renderer()->ComputeVisiblePropBounds(bounds);

    double side = std::abs(bounds[3]-bounds[2]);

    double center[3];
    center[0] = (bounds[1]+bounds[0])/2;
    center[1] = (bounds[3]+bounds[2])/2;
    center[2] = (bounds[5]+bounds[4])/2;

    vtkCamera *camera =this->renderer()->GetActiveCamera();
    camera->SetPosition(center[0], center[1]-side, center[2]);
    camera->SetFocalPoint(center[0], center[1], center[2]);
    camera->SetViewUp(0, 0, 1);

    this->renderer()->ResetCamera();

    this->update();
}

// ///////////////////////////////////////////////////////////////////

//
// dtkVisualizationView2D.cpp ends here
