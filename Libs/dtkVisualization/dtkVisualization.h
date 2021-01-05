#pragma once

#include <dtkVisualizationExport>

class QString;
// class dtkCoreLayerManager;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

namespace dtk {
namespace visualization {

//   DTKVISUALIZATION_EXPORT dtkCoreLayerManager& manager(void);

// DTKVISUALIZATION_EXPORT void activateObjectManager(void);

DTKVISUALIZATION_EXPORT void initialize(void);
// DTKVISUALIZATION_EXPORT void initialize(const QString& path);
// DTKVISUALIZATION_EXPORT void setVerboseLoading(bool b);
// DTKVISUALIZATION_EXPORT void uninitialize(void);

} // namespace visualization
}; // namespace dtk

//
// dtkVisualization.h ends here
