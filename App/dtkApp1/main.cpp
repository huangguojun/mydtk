#include <QtCore>
#include <QtGui>
#include <dtkVisualization>
#include <dtkWidgets>
#include <vtkDICOMImageReader.h>
#include <vtkImageViewer2.h>

int main(int argc, char **argv)
{
    dtkApplication *application = dtkApplication::create(argc, argv);

    vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();

    // reader->SetFileName(filename.toStdString().data());
    vtkSmartPointer<vtkImageViewer2> pImageViewer = vtkSmartPointer<vtkImageViewer2>::New();
    pImageViewer->SetInputConnection(reader->GetOutputPort());

    dtkWidgetsWidget *view = new dtkVisualizationView2D;
    view->show();

    return application->exec();
}
