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

class QString;
class vtkDataSet;

// ///////////////////////////////////////////////////////////////////
// dtkVisualizationVTKReader
// ///////////////////////////////////////////////////////////////////

DTKVISUALIZATION_EXPORT vtkDataSet *dtkVisualizationVTKReader(const QString& path);
DTKVISUALIZATION_EXPORT vtkDataSet *dtkVisualizationVTKReaderVTI(const QString& path);
DTKVISUALIZATION_EXPORT vtkDataSet *dtkVisualizationVTKReaderVTP(const QString& path);
DTKVISUALIZATION_EXPORT vtkDataSet *dtkVisualizationVTKReaderVTR(const QString& path);
DTKVISUALIZATION_EXPORT vtkDataSet *dtkVisualizationVTKReaderVTS(const QString& path);
DTKVISUALIZATION_EXPORT vtkDataSet *dtkVisualizationVTKReaderVTU(const QString& path);

//
// dtkVisualizationVTKReader.h ends here
