// Version: $Id: 47b36cd334c5809fdb25a9dfc2dd65d9184474a3 $
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

#include <dtkCoreExport.h>

class QString;

class dtkCorePluginManagerBase;
class dtkCoreLayerManager;

// /////////////////////////////////////////////////////////////////
// dtkCorePluginManagerRecorder interface
// /////////////////////////////////////////////////////////////////

class DTKCORE_EXPORT dtkCorePluginManagerRecorder
{
public:
    explicit dtkCorePluginManagerRecorder(dtkCoreLayerManager *layer_manager,
                                          dtkCorePluginManagerBase *plugin_manager,
                                          const QString &plugin_manager_name);
    ~dtkCorePluginManagerRecorder(void) = default;
};

//
// dtkCorePluginManagerRecorder.h ends here
