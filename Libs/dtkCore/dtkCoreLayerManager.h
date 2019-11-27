// Version: $Id: 3dbda2257781c79d6314e4bdf403ef7ea182e972 $
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

#include <memory>

#include <QtCore>

class dtkCorePluginManagerBase;
class dtkCorePluginFactoryBase;
class dtkObjectManager;

// /////////////////////////////////////////////////////////////////
// dtkCoreLayerManager interface
// /////////////////////////////////////////////////////////////////

class DTKCORE_EXPORT dtkCoreLayerManager
{
public:
    using PluginManagers = QHash<QString, dtkCorePluginManagerBase *>;
    using PluginFactories = QHash<QString, dtkCorePluginFactoryBase *>;

public:
     dtkCoreLayerManager(void);
    ~dtkCoreLayerManager(void);

public:
    void record(const QString& plugin_manager_name, dtkCorePluginManagerBase *plugin_manager);
    void record(const QString& plugin_factory_name, dtkCorePluginFactoryBase *plugin_factory);

    void initialize(void);
    void initialize(const QString& path);

    void uninitialize(void);

    void setVerboseLoading(bool value);
    bool verboseLoading(void) const;

    void setAutoLoading(bool value);
    bool autoLoading(void) const;

    void setObjectManager(dtkObjectManager *manager);
    dtkObjectManager *objectManager(void) const;

    PluginManagers pluginManagers(void) const;
    PluginFactories pluginFactories(void) const;

private:
    class dtkCoreLayerManagerPrivate *d;
};

//
// dtkCoreLayerManager.h ends here
