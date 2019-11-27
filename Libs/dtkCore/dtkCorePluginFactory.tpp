// Version: $Id: bc11fe22842a85a61e71a3acedba8f70e6f5f49e $
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

#include <dtkLog>
#include <dtkMeta>

#include "dtkCorePluginFactory_p.h"
#include "dtkCorePluginBase.h"

// ///////////////////////////////////////////////////////////////////
// dtkCorePluginFactory implementation
// ///////////////////////////////////////////////////////////////////

template <typename T> inline dtkCorePluginFactory<T>::dtkCorePluginFactory(void) : m_d(new dtkCorePluginFactoryPrivate)
{

}

template <typename T> inline dtkCorePluginFactory<T>::~dtkCorePluginFactory(void)
{
    delete m_d;
}

// /////////////////////////////////////////////////////////////////
// Type creator registration
// /////////////////////////////////////////////////////////////////

template <typename T> inline void dtkCorePluginFactory<T>::record(const QString& key, creator func)
{
    if (this->creators.contains(key)) {
        dtkTrace() << Q_FUNC_INFO << "Factory already contains key" << key << ". Nothing is done";
        return;
    }

    this->creators.insert(key, func);
}

template <typename T> inline void dtkCorePluginFactory<T>::clear(void)
{
    this->creators.clear();
    for (QString key: this->creators_plugins.keys()) {
        delete this->creators_plugins[key];
    }
    this->creators_plugins.clear();
}

template <typename T> inline void dtkCorePluginFactory<T>::recordPlugin(const QString& key, dtkCorePluginBase *plugin, bool force)
{
    if (this->creators_plugins.contains(key)) {
        if (!force) {
            qDebug() << Q_FUNC_INFO << "Factory already contains key" << key << ". Nothing is done";
            return;
        } else {
            qDebug() << Q_FUNC_INFO << "Factory already contains key" << key << ". replace old plugin";
            delete this->creators_plugins[key];
        }
    }

    this->creators_plugins.insert(key, plugin);
}

template <typename T> inline void dtkCorePluginFactory<T>::record(const QString& key, widget_creator func)
{
    if (this->widget_creators.contains(key)) {
        dtkTrace() << Q_FUNC_INFO << "Factory already contains key" << key << ". Nothing is done";
        return;
    }

    this->widget_creators.insert(key, func);
}

// /////////////////////////////////////////////////////////////////
// Type creator invokation
// /////////////////////////////////////////////////////////////////

template <typename T> inline T *dtkCorePluginFactory<T>::create(const QString& key) const
{
    T *obj = nullptr;
    if (this->creators.contains(key)) {
        obj = this->creators.value(key)();

    } else if (this->creators_plugins.contains(key)) {
        void *o = this->creators_plugins.value(key)->create();
        if (!o) {
            dtkWarn() << Q_FUNC_INFO << "Nullptr is returned by plugin creator.";
            return nullptr;
        }
        obj = static_cast<T*>(o);
    }

    if (obj) {
        m_d->touch(dtkMetaType::variantFromValue(obj));
    }

    return obj;
}

// ///////////////////////////////////////////////////////////////////
// Widget creation
// ///////////////////////////////////////////////////////////////////

template <typename T> inline QWidget *dtkCorePluginFactory<T>::createWidget(const QString& key) const
{
    if (this->widget_creators.contains(key)) {
        return this->widget_creators[key]();
    }
    return nullptr;
}

// /////////////////////////////////////////////////////////////////
// Type creator inspection
// /////////////////////////////////////////////////////////////////

template <typename T> inline QStringList dtkCorePluginFactory<T>::keys(void) const
{
    QStringList lkeys = this->creators.keys() ;
    lkeys <<  this->creators_plugins.keys();
    return  lkeys;
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

template <typename T> inline void dtkCorePluginFactory<T>::connect(dtkObjectManager *manager) const
{
    m_d->connect(manager);
}

template <typename T> inline void dtkCorePluginFactory<T>::disconnect(dtkObjectManager *manager) const
{
    m_d->disconnect(manager);
}

//
// dtkCorePluginFactory_t.h ends here
