// Version: $Id: 2f06851c5416d366049116fb08072a0cd021f0fa $
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

class dtkCoreLayerManager;
class dtkCorePluginFactoryBase;

// /////////////////////////////////////////////////////////////////
// dtkCorePluginFactoryRecorder interface
// /////////////////////////////////////////////////////////////////

class DTKCORE_EXPORT dtkCorePluginFactoryRecorder
{
public:
    explicit dtkCorePluginFactoryRecorder(dtkCoreLayerManager *layer_manager, dtkCorePluginFactoryBase *plugin_factory, const QString& plugin_name);
    ~dtkCorePluginFactoryRecorder(void) = default;
};

//
// dtkCorePluginFactoryRecorder.h ends here
