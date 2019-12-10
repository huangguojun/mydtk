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

#include "dtkVisualizationVTKReader.h"

#include <QtCore>

#include <vtkDataSet.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkRectilinearGrid.h>
#include <vtkStructuredGrid.h>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLImageDataReader.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLRectilinearGridReader.h>
#include <vtkXMLStructuredGridReader.h>
#include <vtkXMLUnstructuredGridReader.h>

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationVTKReader implementation
// ///////////////////////////////////////////////////////////////////

vtkDataSet *dtkVisualizationVTKReader(const QString& path)
{
    if (path.endsWith(".vti"))
        return dtkVisualizationVTKReaderVTI(path);

    if (path.endsWith(".vtp"))
        return dtkVisualizationVTKReaderVTP(path);

    if (path.endsWith(".vtr"))
        return dtkVisualizationVTKReaderVTR(path);

    if (path.endsWith(".vts"))
        return dtkVisualizationVTKReaderVTS(path);

    if (path.endsWith(".vtu"))
        return dtkVisualizationVTKReaderVTU(path);

    qWarning() << "file" << path << "is not handled by vtk reader";
    return nullptr;
}

vtkDataSet *dtkVisualizationVTKReaderVTI(const QString& path)
{
    vtkDataSet *res = vtkImageData::New();
    vtkSmartPointer<vtkXMLImageDataReader> reader = vtkSmartPointer<vtkXMLImageDataReader>::New();
    reader->SetFileName(qPrintable(path));
    reader->Update();
    res->DeepCopy(reader->GetOutput());
    return res;
}

vtkDataSet *dtkVisualizationVTKReaderVTP(const QString& path)
{
    vtkDataSet *res = vtkPolyData::New();
    vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
    reader->SetFileName(qPrintable(path));
    reader->Update();
    res->DeepCopy(reader->GetOutput());
    return res;
}

vtkDataSet *dtkVisualizationVTKReaderVTR(const QString& path)
{
    vtkDataSet *res = vtkRectilinearGrid::New();
    vtkSmartPointer<vtkXMLRectilinearGridReader> reader = vtkSmartPointer<vtkXMLRectilinearGridReader>::New();
    reader->SetFileName(qPrintable(path));
    reader->Update();
    res->DeepCopy(reader->GetOutput());
    return res;
}

vtkDataSet *dtkVisualizationVTKReaderVTS(const QString& path)
{
    vtkDataSet *res = vtkStructuredGrid::New();
    vtkSmartPointer<vtkXMLStructuredGridReader> reader = vtkSmartPointer<vtkXMLStructuredGridReader>::New();
    reader->SetFileName(qPrintable(path));
    reader->Update();
    res->DeepCopy(reader->GetOutput());
    return res;
}

vtkDataSet *dtkVisualizationVTKReaderVTU(const QString& path)
{
    vtkDataSet *res = vtkUnstructuredGrid::New();
    vtkSmartPointer<vtkXMLUnstructuredGridReader> reader = vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();
    reader->SetFileName(qPrintable(path));
    reader->Update();
    res->DeepCopy(reader->GetOutput());
    return res;
}

//
// dtkVisualizationView3D.cpp ends here
