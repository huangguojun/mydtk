// Version: $Id: 8ac2897611cf02f8f3db68d913ff85597ad573f0 $
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

#include <QtCore>

// /////////////////////////////////////////////////////////////////
// dtkCorePluginManagerBase
// /////////////////////////////////////////////////////////////////

class DTKCORE_EXPORT dtkCorePluginManagerBase : public QObject
{
public:
    virtual ~dtkCorePluginManagerBase(void) {}

 public:
    virtual void   initialize(const QString& path) = 0;
    virtual void uninitialize(void) = 0;

    virtual void setVerboseLoading(bool value) = 0;
    virtual bool verboseLoading(void) const = 0;

    virtual void setAutoLoading(bool value) = 0;
    virtual bool autoLoading(void) const = 0;

    virtual void   load(const QString& path) = 0;
    virtual void unload(const QString& path) = 0;

    virtual void loadFromName(const QString& plugin_name) = 0;
};

// /////////////////////////////////////////////////////////////////
// Forward declaration of dtkCorePluginManagerPrivate
// /////////////////////////////////////////////////////////////////

template <typename T> class dtkCorePluginManagerPrivate;

// ///////////////////////////////////////////////////////////////////
// dtkCorePluginManager
// ///////////////////////////////////////////////////////////////////

template <typename T> class dtkCorePluginManager : public dtkCorePluginManagerBase
{
public:
     dtkCorePluginManager(void);
    ~dtkCorePluginManager(void);

#pragma mark -
#pragma mark Manager Management

    void   initialize(const QString& path);
    void uninitialize(void);

public:
    void setVerboseLoading(bool value);
    bool verboseLoading(void) const;
    void setAutoLoading(bool value);
    bool autoLoading(void) const;
    void loadFromName(const QString& plugin_name);
    void setLayerVersion(const QString& layer_version);

#pragma mark -
#pragma mark Plugin Management

    void   load(const QString& path);
    void unload(const QString& path);

#pragma mark -
#pragma mark Plugin Queries

    QStringList plugins(void) const ;
    QStringList availablePlugins(void) const;

protected:
    dtkCorePluginManagerPrivate<T> *d;
};

#include "dtkCorePluginManager.tpp"

//
// dtkCorePluginManager.h ends here
