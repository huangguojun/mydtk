// Version: $Id: 06ac1f98c1b5e5e72a762ee6ceca53f15d638952 $
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

#include "dtkCorePluginManagerHandler.h"
#include "dtkCore.h"

#include <dtkLog>

// ///////////////////////////////////////////////////////////////////
// dtkCorePluginManagerPrivate
// ///////////////////////////////////////////////////////////////////

template <typename T> class dtkCorePluginManagerPrivate
{
public:
    bool check(const QString& path, bool checkConceptLayer = true);
    bool checkConcept(const QString& path);

public:
    bool verboseLoading;
    bool autoLoading;
    QString layerVersion = QString();

public:
    QHash<QString, QPluginLoader *> loaders;
};

// ///////////////////////////////////////////////////////////////////
// dtkCorePluginManagerPrivate
// ///////////////////////////////////////////////////////////////////

template <typename T> inline bool dtkCorePluginManagerPrivate<T>::checkConcept(const QString& path)
{
    QString conceptName = QMetaType::typeName(qMetaTypeId<T *>());
    conceptName.remove("Plugin*");

    QString pluginConcept = dtkCorePluginManagerHandler::instance()->concept(path);

    if (conceptName != pluginConcept) {
        if (this->verboseLoading) {
            dtkDebug() << "skip plugin: not an implementation of the current concept" << conceptName << ", this plugin is for " << pluginConcept;
        }

        return false;
    }

    return true;
}

template <typename T> inline bool dtkCorePluginManagerPrivate<T>::check(const QString& path, bool checkConceptLayer)
{
    bool status = true;

    if (checkConceptLayer && !this->checkConcept(path)) {
        return false;
    }


    if (checkConceptLayer && !layerVersion.isNull() && !checkVersion(layerVersion , dtkCorePluginManagerHandler::instance()->pluginsLayerVersion(path))) {
        dtkWarn() << "    Version mismatch: layer version " << layerVersion
                  << " plugin compiled for layer version" << dtkCorePluginManagerHandler::instance()->pluginsLayerVersion(path)
                  << " for plugin " << path;
        return false;
    }

    foreach (QVariant item, dtkCorePluginManagerHandler::instance()->dependencies(path)) {

        QVariantMap mitem = item.toMap();
        QString na_mitem = mitem.value("name").toString();
        QString ve_mitem = mitem.value("version").toString();
        QString key = dtkCorePluginManagerHandler::instance()->pluginPath(na_mitem);

        if (!dtkCorePluginManagerHandler::instance()->hasName(na_mitem)) {
            dtkWarn() << "  Missing dependency:" << na_mitem << "for plugin" << path;
            status = false;

            continue;
        }

        if (!checkVersion(dtkCorePluginManagerHandler::instance()->version(key), ve_mitem)) {
            dtkWarn() << "    Version mismatch:" << na_mitem << "version" << dtkCorePluginManagerHandler::instance()->version(key) << "but" << ve_mitem << "required for plugin" << path;

            status = false;

            continue;
        }

        if (!check(key, false)) {
            dtkWarn() << "Corrupted dependency:" << na_mitem << "for plugin" << path;

            status = false;

            continue;
        }
    }

    return status;
}

// ///////////////////////////////////////////////////////////////////
// dtkCorePluginManager
// ///////////////////////////////////////////////////////////////////

template <typename T> inline dtkCorePluginManager<T>::dtkCorePluginManager(void) : d(new dtkCorePluginManagerPrivate<T>)
{
    d->verboseLoading = false;
    d->autoLoading = true;
}

template <typename T> inline dtkCorePluginManager<T>::~dtkCorePluginManager(void)
{
    delete d;

    d = NULL;
}

// /////////////////////////////////////////////////////////////////
// Logging
// /////////////////////////////////////////////////////////////////

template <typename T> inline void dtkCorePluginManager<T>::setVerboseLoading(bool value)
{
    d->verboseLoading = value;
}

template <typename T> inline bool dtkCorePluginManager<T>::verboseLoading(void) const
{
    return d->verboseLoading;
}

// /////////////////////////////////////////////////////////////////
// Auto loading
// /////////////////////////////////////////////////////////////////

template <typename T> inline void dtkCorePluginManager<T>::setAutoLoading(bool value)
{
    d->autoLoading = value;
}

template <typename T> inline bool dtkCorePluginManager<T>::autoLoading(void) const
{
    return d->autoLoading;
}

template <typename T> inline void dtkCorePluginManager<T>::loadFromName(const QString& plugin_name)
{
    QString full_name = plugin_name % "Plugin";

    auto i = dtkCorePluginManagerHandler::instance()->namesBegin();
    auto e = dtkCorePluginManagerHandler::instance()->namesEnd();

    while (i != e) {
        if (QString::compare(i.value(), full_name) == 0) {
            this->load(i.key());
            return;
        }

        ++i;
    }

    dtkWarn() << Q_FUNC_INFO << plugin_name << " not found ";
    dtkWarn() << Q_FUNC_INFO << "keys" << dtkCorePluginManagerHandler::instance()->pluginPaths() << dtkCorePluginManagerHandler::instance()->names();
}

// /////////////////////////////////////////////////////////////////
// set Layer Version
// /////////////////////////////////////////////////////////////////

template <typename T> inline void dtkCorePluginManager<T>::setLayerVersion(const QString& layer_version)
{
    d->layerVersion = layer_version;
}

// /////////////////////////////////////////////////////////////////
// Manager Management
// /////////////////////////////////////////////////////////////////

template <typename T> void dtkCorePluginManager<T>::initialize(const QString& path)
{

    foreach (QString path2, path.split(":", QString::SkipEmptyParts)) {

        QDir dir(path2);

        if (d->verboseLoading) {
            dtkTrace() << "scanning directory for plugins:" << path2;
        }

        foreach (QFileInfo info, dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot))
            dtkCorePluginManagerHandler::instance()->scan(info.absoluteFilePath(), d->verboseLoading);

        if (d->autoLoading) {
            foreach (QFileInfo info, dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot)) {
                if (d->checkConcept(info.absoluteFilePath())) {
                    if (d->verboseLoading) {
                        dtkInfo() << "load" << info.absoluteFilePath();
                    }
                    this->load(info.absoluteFilePath());
                }
            }
        } else {
            dtkInfo() << "auto loading disabled";
        }
    }
}

template <typename T> inline void dtkCorePluginManager<T>::uninitialize(void)
{
    foreach (const QString& path, d->loaders.keys())
        this->unload(path);
}

// /////////////////////////////////////////////////////////////////
// Plugin Management
// /////////////////////////////////////////////////////////////////

template <typename T> inline void dtkCorePluginManager<T>::load(const QString& path)
{
    if (!QLibrary::isLibrary(path)) {
        QString error = "Unable to load non library file " + path;

        if (d->verboseLoading) {
            dtkWarn() << error;
        }

        return;
    }

    if (!d->check(path)) {
        QString error = "check failure for plugin file " + path;

        if (d->verboseLoading) {
            dtkWarn() << error;
        }

        return;
    }

    QPluginLoader *loader = new QPluginLoader(path); // should not set this as parent to avoid bad deallocation

    if (!loader) {
        QString error = "Empty loader for file " + path;

        if (d->verboseLoading) {
            dtkWarn() << error;
        }

        return;
    }

    loader->setLoadHints(QLibrary::ExportExternalSymbolsHint);

    if (d->verboseLoading) {
        dtkTrace() << "Loading plugin from " << path;
    }

    if (!loader->load()) {
        QString error = "Unable to load ";
        error += path;
        error += " - ";
        error += loader->errorString();
        dtkWarn() << error;
        delete loader;
        return;
    }

    T *plugin = qobject_cast<T *>(loader->instance());

    if (!plugin) {
        QString error = "Unable to retrieve ";
        error += path;
        dtkWarn() << error;

        loader->unload();
        delete loader;
        return;
    }

    plugin->initialize();

    d->loaders.insert(path, loader);

    if (d->verboseLoading) {
        QString name =  loader->metaData().value("MetaData").toObject().value("name").toString();
        dtkTrace() << "Loaded plugin " <<  name  << " from " << path;
    }
}

template <typename T> inline void dtkCorePluginManager<T>::unload(const QString& path)
{
    QPluginLoader *loader = d->loaders.value(path);

    T *plugin = qobject_cast<T *>(loader->instance());

    if (plugin)
        plugin->uninitialize();

    if (loader->unload()) {
        d->loaders.remove(path);
        delete loader;
    } else {
        dtkWarn() << Q_FUNC_INFO << loader->errorString();
    }
}

// /////////////////////////////////////////////////////////////////
// Plugin Queries
// /////////////////////////////////////////////////////////////////

template <typename T> inline QStringList dtkCorePluginManager<T>::plugins(void) const
{
    return d->loaders.keys();
}

template <typename T> inline QStringList dtkCorePluginManager<T>::availablePlugins(void) const
{
    return dtkCorePluginManagerHandler::instance()->names();
}

//
// dtkCorePluginManager_t.h ends here
