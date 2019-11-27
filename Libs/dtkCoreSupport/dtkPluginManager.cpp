/* dtkPluginManager.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Tue Aug  4 12:20:59 2009 (+0200)
 * Version: $Id: b3bc20cb17f746daf0068ab69d9642c37f8eb20a $
 * Last-Updated: mer. avril  2 08:45:01 2014 (+0200)
 *           By: Nicolas Niclausse
 *     Update #: 321
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkPlugin.h"
#include "dtkPluginManager.h"

#include <QtWidgets>

#include <dtkLog>

// /////////////////////////////////////////////////////////////////
// Helper functions
// /////////////////////////////////////////////////////////////////

QStringList dtkPluginManagerPathSplitter(QString path)
{
    QString paths = path;

#ifdef Q_OS_WIN
    QStringList pathList;
    QRegExp pathFilterRx("(([a-zA-Z]:|)[^:]+)");

    int pos = 0;

    while ((pos = pathFilterRx.indexIn(paths, pos)) != -1) {

        QString pathItem = pathFilterRx.cap(1);
        pathItem.replace( "\\" , "/" );

        if (!pathItem.isEmpty())
            pathList << pathItem;

        pos += pathFilterRx.matchedLength();
    }

#else
    QStringList pathList = paths.split(":", QString::SkipEmptyParts);
#endif

    return pathList;
}

// /////////////////////////////////////////////////////////////////
// dtkPluginManagerPrivate
// /////////////////////////////////////////////////////////////////

class dtkPluginManagerPrivate
{
public:
    bool check(const QString& path);

public:
    QHash<QString, QVariant> names;
    QHash<QString, QVariant> versions;
    QHash<QString, QVariantList> dependencies;

public:
    QString path;

    QHash<QString, QPluginLoader *> loaders;

    bool verboseLoading;
    int argc;
    char *argv;
};

#include "dtkAbstractData.h"

// #include <dtkMath/dtkVector.h>
// #include <dtkMath/dtkVector3D.h>
// #include <dtkMath/dtkQuaternion.h>

dtkPluginManager *dtkPluginManager::instance(void)
{
    if (!s_instance) {
        s_instance = new dtkPluginManager;

        qRegisterMetaType<dtkAbstractObject>("dtkAbstractObject");
        qRegisterMetaType<dtkAbstractObject *>("dtkAbstractObject*");
        qRegisterMetaType<dtkAbstractData>("dtkAbstractData");
        qRegisterMetaType<dtkAbstractData *>("dtkAbstractData*");
        // qRegisterMetaType<dtkVectorReal>("dtkVectorReal");
        // qRegisterMetaType<dtkVectorReal*>("dtkVectorReal*");
        // qRegisterMetaType<dtkVector3DReal>("dtkVector3DReal");
        // qRegisterMetaType<dtkVector3DReal*>("dtkVector3DReal*");
        // qRegisterMetaType<dtkQuaternionReal>("dtkQuaternionReal");
        // qRegisterMetaType<dtkQuaternionReal*>("dtkQuaternionReal*");
    }

    return s_instance;
}

bool dtkPluginManagerPrivate::check(const QString& path)
{
    bool status = true;

    foreach (QVariant item, this->dependencies.value(path)) {

        QVariantMap mitem = item.toMap();
        QVariant na_mitem = mitem.value("name");
        QVariant ve_mitem = mitem.value("version");
        QString key = this->names.key(na_mitem);

        if (!this->names.values().contains(na_mitem)) {
            dtkWarn() << "  Missing dependency:" << na_mitem.toString() << "for plugin" << path;
            status = false;
            continue;
        }

        if (this->versions.value(key) != ve_mitem) {
            dtkWarn() << "    Version mismatch:" << na_mitem.toString() << "version" << this->versions.value(this->names.key(na_mitem)).toString() << "but" << ve_mitem.toString() << "required for plugin" << path;
            status = false;
            continue;
        }

        if (!check(key)) {
            dtkWarn() << "Corrupted dependency:" << na_mitem.toString() << "for plugin" << path;
            status = false;
            continue;
        }
    }

    return status;
}

// /////////////////////////////////////////////////////////////////
// dtkPluginManager
// /////////////////////////////////////////////////////////////////

void dtkPluginManager::initializeApplication(void)
{
    d->argc = 1;
    d->argv = new char[13];
    d->argv = (char *)"dtk-embedded";

    (void) new QApplication(d->argc, &(d->argv));
}

void dtkPluginManager::initialize(void)
{
    if (d->path.isNull())
        this->readSettings();

    QStringList pathList = dtkPluginManagerPathSplitter(d->path);

    const QString appDir = qApp->applicationDirPath();

    foreach (QString path, pathList) {

        QDir dir(appDir);

        if (dir.cd(path)) {
            dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

            foreach (QFileInfo entry, dir.entryInfoList())
                scan(entry.absoluteFilePath());

            foreach (QFileInfo entry, dir.entryInfoList())
                loadPlugin(entry.absoluteFilePath());
        } else {
            dtkWarn() << "Failed to load plugins from path " << path << ". Could not cd to directory.";
        }
    }
}

void dtkPluginManager::uninitialize(void)
{
    this->writeSettings();

    foreach (QString path, d->loaders.keys())
        unloadPlugin(path);
}

void dtkPluginManager::uninitializeApplication(void)
{
    delete qApp;
}


void dtkPluginManager::scan(const QString& path)
{
    if (!QLibrary::isLibrary(path))
        return;

    QPluginLoader *loader = new QPluginLoader(path);

    d->names.insert(path, loader->metaData().value("MetaData").toObject().value("name").toVariant());
    d->versions.insert(path, loader->metaData().value("MetaData").toObject().value("version").toVariant());
    d->dependencies.insert(path, loader->metaData().value("MetaData").toObject().value("dependencies").toArray().toVariantList());

    delete loader;
}

//! Load a specific plugin designated by its name.
/*! The path is retrieved through the plugin manager settings.
 *
 * \param name The name of the plugin, without platform specific prefix (.e.g lib) and suffix (e.g. .so or .dylib or .dll)
 */

void dtkPluginManager::load(const QString& name)
{
    if (d->path.isNull())
        this->readSettings();

    QStringList pathList = dtkPluginManagerPathSplitter(d->path);

    const QString appDir = qApp->applicationDirPath();

    foreach (QString path, pathList) {

        QDir dir(appDir);

        if (dir.cd(path)) {
            dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

            foreach (QFileInfo entry, dir.entryInfoList())
                if (entry.fileName().contains(name))
                    loadPlugin(entry.absoluteFilePath());
        } else {
            dtkWarn() << "Failed to load plugins from path " << path << ". Could not cd to directory.";
        }
    }
}

//! Unload a specific plugin designated by its name.
/*! The path is retrieved through the plugin manager settings.
 *
 * \param name The name of the plugin, without platform specific prefix (.e.g lib) and suffix (e.g. .so or .dylib or .dll)
 */

void dtkPluginManager::unload(const QString& name)
{
    if (d->path.isNull())
        this->readSettings();

    QStringList pathList = dtkPluginManagerPathSplitter(d->path);

    const QString appDir = qApp->applicationDirPath();

    foreach (QString path, pathList) {

        QDir dir(appDir);

        if (dir.cd(path)) {
            dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

            foreach (QFileInfo entry, dir.entryInfoList())
                if (entry.fileName().contains(name))
                    if (this->plugin(name))
                        this->unloadPlugin(entry.absoluteFilePath());
        } else {
            dtkWarn() << "Failed to load plugins from path " << path << ". Could not cd to directory.";
        }
    }
}

void dtkPluginManager::readSettings(void)
{
    QSettings settings("inria", "dtk");
    QString defaultPath;
    QDir plugins_dir;
#ifdef Q_OS_MAC
    plugins_dir = qApp->applicationDirPath() + "/../PlugIns";
#else
    plugins_dir = qApp->applicationDirPath() + "/../plugins";
#endif

    defaultPath = plugins_dir.absolutePath();
    settings.beginGroup("plugins");

    if (!settings.contains("path")) {
        dtkDebug() << "Filling in empty path in settings with default path:" << defaultPath;
        settings.setValue("path", defaultPath);
    }

    d->path = settings.value("path", defaultPath).toString();

    settings.endGroup();

    if (d->path.isEmpty()) {
        dtkWarn() << "Your dtk config does not seem to be set correctly.";
        dtkWarn() << "Please set plugins.path.";
    }
}

void dtkPluginManager::writeSettings(void)
{
    // QSettings settings("inria", "dtk");
    // settings.beginGroup("plugins");
    // settings.setValue("path", d->path);
    // settings.endGroup();
}

void dtkPluginManager::printPlugins(void)
{
    foreach (QString path, d->loaders.keys())
        qDebug() << path;
}

void dtkPluginManager::setVerboseLoading(bool value)
{
    d->verboseLoading = true;
}

bool dtkPluginManager::verboseLoading(void) const
{
    return d->verboseLoading;
}

dtkPlugin *dtkPluginManager::plugin(const QString& name)
{
    foreach (QPluginLoader *loader, d->loaders) {
        dtkPlugin *plugin = qobject_cast<dtkPlugin *>(loader->instance());

        if (plugin->name() == name)
            return plugin;
    }

    return NULL;
}

QList<dtkPlugin *> dtkPluginManager::plugins(void)
{
    QList<dtkPlugin *> list;

    foreach (QPluginLoader *loader, d->loaders)
        list << qobject_cast<dtkPlugin *>(loader->instance());

    return list;
}

void dtkPluginManager::setPath(const QString& path)
{
    d->path = path;
}

QString dtkPluginManager::path(void) const
{
    return d->path;
}

dtkPluginManager::dtkPluginManager(void) : d(new dtkPluginManagerPrivate)
{
    d->verboseLoading = false;
    d->argv = NULL;
}

dtkPluginManager::~dtkPluginManager(void)
{
    if (d->argv) {
        delete d->argv;
    }

    delete d;

    d = NULL;
}

/*!
    \brief      Loads the plugin from the given filename.
                Derived classes may override to prevent certain plugins being loaded,
                or provide additional functionality. In most cases they should still
                call the base implementation (this).
    \param      path : Path to plugin file to be loaded.
*/

void dtkPluginManager::loadPlugin(const QString& path)
{
    if (!d->check(path)) {
        QString error = "check failure for plugin file " + path;

        if (d->verboseLoading) {
            dtkWarn() << error;
        }

        return;
    }

    QPluginLoader *loader = new QPluginLoader(path);

    loader->setLoadHints (QLibrary::ExportExternalSymbolsHint);

    if (!loader->load()) {
        QString error = "Unable to load ";
        error += path;
        error += " - ";
        error += loader->errorString();

        if (d->verboseLoading) {
            dtkWarn() << error;
        }

        emit loadError(error);
        delete loader;
        return;
    }

    dtkPlugin *plugin = qobject_cast<dtkPlugin *>(loader->instance());

    if (!plugin) {
        QString error = "Unable to retrieve ";
        error += path;

        if (d->verboseLoading) {
            dtkWarn() << error;
        }

        emit loadError(error);
        return;
    }

    if (!plugin->initialize()) {
        QString error = "Unable to initialize ";
        error += plugin->name();
        error += " plugin";

        if (d->verboseLoading) {
            dtkWarn() << error;
        }

        emit loadError(error);
        return;
    }

    d->loaders.insert(path, loader);

    if (d->verboseLoading) {
        dtkTrace() << "Loaded plugin " << plugin->name() << " from " << path;
    }

    emit loaded(plugin->name());
}

//! Unloads the plugin previously loaded from the given filename.
/*! Derived classes may override to prevent certain plugins being
 *  unloaded, or provide additional functionality. In most
 *  cases they should still call the base implementation
 *  (this).
 *
 * \param path Path to plugin file to be unloaded.
 */
void dtkPluginManager::unloadPlugin(const QString& path)
{
    dtkPlugin *plugin = qobject_cast<dtkPlugin *>(d->loaders.value(path)->instance());

    if (!plugin) {
        if (d->verboseLoading) {
            dtkDebug() << "dtkPluginManager - Unable to retrieve " << plugin->name() << " plugin";
        }

        return;
    }

    if (!plugin->uninitialize()) {
        if (d->verboseLoading) {
            dtkTrace() << "Unable to uninitialize " << plugin->name() << " plugin";
        }

        return;
    }

    QPluginLoader *loader = d->loaders.value(path);

    if (!loader->unload()) {
        if (d->verboseLoading) {
            dtkDebug() << "dtkPluginManager - Unable to unload plugin: " << loader->errorString();
        }

        return;
    }

    delete loader;

    d->loaders.remove(path);

    // emit unloaded(plugin->name());
}

dtkPluginManager *dtkPluginManager::s_instance = NULL;
