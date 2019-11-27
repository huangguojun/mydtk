// Version: $Id: 32ce143b1504cfd8caa289887e1a25bbf10d699d $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkCorePluginManagerRecorder.h"

#include "dtkCoreLayerManager.h"

// /////////////////////////////////////////////////////////////////
// dtkCorePluginManagerRecorder implementation
// /////////////////////////////////////////////////////////////////

dtkCorePluginManagerRecorder::dtkCorePluginManagerRecorder(dtkCoreLayerManager *layer_manager, dtkCorePluginManagerBase *plugin_manager, const QString& plugin_manager_name)
{
    layer_manager->record(plugin_manager_name, plugin_manager);
}

//
// dtkCorePluginManagerRecorder.cpp ends here
