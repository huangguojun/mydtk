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

#include <dtkWidgetsExport>

class dtkCoreLayerManager;
class QString;

// ///////////////////////////////////////////////////////////////////
// Layer methods declarations
// ///////////////////////////////////////////////////////////////////

namespace dtk {

namespace widgets {

DTKWIDGETS_EXPORT dtkCoreLayerManager &manager(void);

DTKWIDGETS_EXPORT void initialize(void);
DTKWIDGETS_EXPORT void initialize(const QString &path);
DTKWIDGETS_EXPORT void setVerboseLoading(bool b);
DTKWIDGETS_EXPORT void setAutoLoading(bool auto_loading);
DTKWIDGETS_EXPORT void uninitialize(void);
} // namespace widgets
} // namespace dtk

//
// dtkWidgets.h ends here
