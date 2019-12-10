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

#include "dtkVisualizationDecoratorSnapshot.h"
#include "dtkVisualizationCanvas.h"
#include "dtkVisualizationDecoratorClutEditor.h"
#include "dtkVisualizationMetaType.h"
#include "dtkVisualizationView2D.h"

#include <dtkLog>

#include <QtGui>
#include <QtWidgets>

#include <vtkPNGWriter.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkWindowToImageFilter.h>

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorSnapshotPrivate declaration
// ///////////////////////////////////////////////////////////////////

class dtkVisualizationDecoratorSnapshotPrivate
{
public:
    vtkSmartPointer<vtkWindowToImageFilter> window_to_image_filter;
    vtkSmartPointer<vtkPNGWriter>           writer;

public:
    dtkVisualizationView2D *view = nullptr;
    QList<QWidget *> inspectors;
    QPushButton *push_button = nullptr;
};

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationDecoratorSnapshot implementation
// ///////////////////////////////////////////////////////////////////

dtkVisualizationDecoratorSnapshot::dtkVisualizationDecoratorSnapshot(void): d(new dtkVisualizationDecoratorSnapshotPrivate())
{
    this->setObjectName("Snapshot");

    d->window_to_image_filter = vtkSmartPointer<vtkWindowToImageFilter>::New();
    // d->window_to_image_filter->SetMagnification(3); //set the resolution of the output image (3 times the current resolution of vtk render window)
    d->window_to_image_filter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
    d->window_to_image_filter->ReadFrontBufferOff(); // read from the back buffer

    d->writer = vtkSmartPointer<vtkPNGWriter>::New();

    // //////////
    // // Inspectors creation

    d->push_button = new QPushButton("Snap");

    // //////////
    // // Inspectors connections

    connect(d->push_button, &QPushButton::clicked, [=] () {

            if (!this->canvas()) return;

            QString path = QFileDialog::getSaveFileName(nullptr, tr("Save snapshot"), ".", tr("Images (*.png)"));

            d->window_to_image_filter->SetInput(this->canvas()->interactor()->GetRenderWindow());
            d->window_to_image_filter->Update();

            vtkSmartPointer<vtkPNGWriter>::New();
            d->writer->SetFileName(path.toStdString().c_str());
            d->writer->SetInputConnection(d->window_to_image_filter->GetOutputPort());
            d->writer->Write();

        });

    d->inspectors << d->push_button;
}

dtkVisualizationDecoratorSnapshot::~dtkVisualizationDecoratorSnapshot(void)
{
    delete d;
    d = nullptr;
}

void dtkVisualizationDecoratorSnapshot::touch(void)
{
}

bool dtkVisualizationDecoratorSnapshot::isDecorating(void)
{
    return true;
}

void dtkVisualizationDecoratorSnapshot::setData(const QVariant& data)
{

}

void dtkVisualizationDecoratorSnapshot::setCanvas(dtkVisualizationCanvas *canvas)
{
    dtkVisualizationView2D *view = dynamic_cast<dtkVisualizationView2D *>(canvas);
    if (!view) {
        qWarning() << Q_FUNC_INFO << "View 2D or view 3D expected as canvas. Canvas is reset to nullptr.";
        return;
    }

    d->view = view;
}

void dtkVisualizationDecoratorSnapshot::unsetCanvas(void)
{
    d->view = nullptr;
}

QVariant dtkVisualizationDecoratorSnapshot::data(void) const
{
    return QVariant();
}

void dtkVisualizationDecoratorSnapshot::setVisibility(bool)
{
}

dtkVisualizationCanvas *dtkVisualizationDecoratorSnapshot::canvas(void) const
{
    return d->view;
}

QList<QWidget *> dtkVisualizationDecoratorSnapshot::inspectors(void) const
{
    return d->inspectors;
}
//
// dtkVisualizationDecoratorSnapshot.cpp ends here
