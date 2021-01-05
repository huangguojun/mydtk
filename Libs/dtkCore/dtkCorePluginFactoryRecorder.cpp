// Version: $Id: dd3649f00f87169e394aad4538bc5b3e1eab63bc $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkCorePluginFactoryRecorder.h"

#include "dtkCoreLayerManager.h"

// /////////////////////////////////////////////////////////////////
// dtkCorePluginFactoryRecorder implementation
// /////////////////////////////////////////////////////////////////

dtkCorePluginFactoryRecorder::dtkCorePluginFactoryRecorder(dtkCoreLayerManager *layer_manager,
                                                           dtkCorePluginFactoryBase *plugin_factory,
                                                           const QString &plugin_name)
{
    layer_manager->record(plugin_name, plugin_factory);
}

//
// dtkCorePluginFactoryRecorder.cpp ends here
