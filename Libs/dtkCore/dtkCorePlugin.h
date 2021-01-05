// Version: $Id: 55f45cec4b638d82ea724eb1920f21fa52ece930 $
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

#include <QtCore/QObject>
#include <QtCore/QVariant>

#include <dtkCoreExport.h>

#include "dtkCoreLayerManager.h"
#include "dtkCorePluginBase.h"
#include "dtkCorePluginFactory.h"
#include "dtkCorePluginManager.h"

// ///////////////////////////////////////////////////////////////////
// DTK_DECLARE_OBJECT
// ///////////////////////////////////////////////////////////////////

#define DTK_DECLARE_OBJECT(type) Q_DECLARE_METATYPE(type)

// ///////////////////////////////////////////////////////////////////
// DTK_DECLARE_PLUGIN
// ///////////////////////////////////////////////////////////////////

#define DTK_DECLARE_PLUGIN_INTERFACE_NAME_STRINGIFIED(type) #type

#define DTK_DECLARE_PLUGIN_INTERFACE_NAME(type) fr.inria.type

#define DTK_DECLARE_PLUGIN_INTERFACE(type)                                                         \
    DTK_DECLARE_PLUGIN_INTERFACE_NAME_STRINGIFIED(DTK_DECLARE_PLUGIN_INTERFACE_NAME(type))

#define DTK_DECLARE_PLUGIN(type, Export)                                                           \
    class Export type##Plugin : public dtkCorePluginBase                                           \
    {                                                                                              \
    };                                                                                             \
    Q_DECLARE_METATYPE(type##Plugin *);                                                            \
    Q_DECLARE_INTERFACE(type##Plugin, DTK_DECLARE_PLUGIN_INTERFACE(type));

// ///////////////////////////////////////////////////////////////////
// DTK_DECLARE_PLUGIN_FACTORY
// ///////////////////////////////////////////////////////////////////

#define DTK_DECLARE_PLUGIN_FACTORY(type, Export)                                                   \
    class Export type##PluginFactory : public dtkCorePluginFactory<type>                           \
    {                                                                                              \
    };                                                                                             \
                                                                                                   \
    template<typename T>                                                                           \
    type *type##Creator(void)                                                                      \
    {                                                                                              \
        return new T;                                                                              \
    };

// ///////////////////////////////////////////////////////////////////
// DTK_DECLARE_PLUGIN_MANAGER
// ///////////////////////////////////////////////////////////////////

#define DTK_DECLARE_PLUGIN_MANAGER(type, Export)                                                   \
    class Export type##PluginManager : public dtkCorePluginManager<type##Plugin>                   \
    {                                                                                              \
    };

// ///////////////////////////////////////////////////////////////////
// DTK_DEFINE_PLUGIN
// ///////////////////////////////////////////////////////////////////

#define DTK_DEFINE_PLUGIN(type) Q_PLUGIN_METADATA(IID DTK_DECLARE_PLUGIN_INTERFACE(type))

// /////////////////////////////////////////////////////////////////
// DTK_DECLARE_CONCEPT
// /////////////////////////////////////////////////////////////////

#define DTK_DECLARE_CONCEPT(type, Export, Namespace)                                               \
    namespace Namespace {                                                                          \
    Export type##PluginFactory &pluginFactory();                                                   \
    Export type##PluginManager &pluginManager();                                                   \
    }

// /////////////////////////////////////////////////////////////////
// DTK_DEFINE_CONCEPT
// /////////////////////////////////////////////////////////////////

#define DTK_DEFINE_CONCEPT(type, Namespace, LayerName)                                             \
    namespace Namespace {                                                                          \
    namespace _private {                                                                           \
    type##PluginFactory factory;                                                                   \
    type##PluginManager manager;                                                                   \
    }                                                                                              \
                                                                                                   \
    type##PluginFactory &pluginFactory() { return _private::factory; }                             \
                                                                                                   \
    type##PluginManager &pluginManager() { return _private::manager; }                             \
                                                                                                   \
    namespace _private {                                                                           \
    dtkCorePluginManagerRecorder mrecorder(&LayerName::manager(), &pluginManager(), #type);        \
    dtkCorePluginFactoryRecorder frecorder(&LayerName::manager(), &pluginFactory(), #type);        \
    }                                                                                              \
    }

// /////////////////////////////////////////////////////////////////
// DTK_DEFINE_LAYER_MANAGER
// /////////////////////////////////////////////////////////////////

#define DTK_DEFINE_LAYER_MANAGER                                                                   \
                                                                                                   \
    dtkCoreLayerManager &manager(void)                                                             \
    {                                                                                              \
        static dtkCoreLayerManager *layer_manager = nullptr;                                       \
        if (!layer_manager) {                                                                      \
            layer_manager = new dtkCoreLayerManager;                                               \
        }                                                                                          \
        return *layer_manager;                                                                     \
    }

//
// dtkCorePlugin.h ends here
