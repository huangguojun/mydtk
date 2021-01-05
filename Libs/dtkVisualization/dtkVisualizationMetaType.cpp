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

#include "dtkVisualizationMetaType.h"

#include <QtCore>

namespace dtk {
namespace visualization {

void registerVTKToMetaType(void)
{
    qRegisterMetaType<vtkDataSet *>();
    qRegisterMetaType<vtkImageData *>();
    qRegisterMetaType<vtkPoints *>();
    qRegisterMetaType<vtkPointSet *>();
    qRegisterMetaType<vtkPolyData *>();
    qRegisterMetaType<vtkRectilinearGrid *>();
    qRegisterMetaType<vtkStructuredGrid *>();
    qRegisterMetaType<vtkUnstructuredGrid *>();

    QMetaType::registerConverter<vtkImageData *, vtkDataSet *>();
    QMetaType::registerConverter<vtkPointSet *, vtkDataSet *>();
    QMetaType::registerConverter<vtkRectilinearGrid *, vtkDataSet *>();
    QMetaType::registerConverter<vtkPolyData *, vtkDataSet *>();
    QMetaType::registerConverter<vtkStructuredGrid *, vtkDataSet *>();
    QMetaType::registerConverter<vtkUnstructuredGrid *, vtkDataSet *>();

    QMetaType::registerConverter<vtkPolyData *, vtkPointSet *>();
    QMetaType::registerConverter<vtkStructuredGrid *, vtkPointSet *>();
    QMetaType::registerConverter<vtkUnstructuredGrid *, vtkPointSet *>();
}
} // namespace visualization
}; // namespace dtk

//
// dtkVisualizationMetaType.cpp ends here
