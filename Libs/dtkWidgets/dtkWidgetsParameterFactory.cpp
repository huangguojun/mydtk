// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkWidgetsParameterFactory.h"

#include "dtkWidgets.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

dtkWidgetsParameter *dtkWidgetsParameterFactory::create(const QString& key)
{
    dtkWidgetsParameter *widget = dtkCorePluginFactory<dtkWidgetsParameter>::create(key);
    if(!widget) {
        qWarning() << Q_FUNC_INFO << "The widget with key :" << key << "could not be instanciated by the factory.";
        return nullptr;
    }
    return widget;
}

dtkWidgetsParameter *dtkWidgetsParameterFactory::create(dtkCoreParameter *parameter, const QString& key)
{
    if (!parameter) {
        qWarning() << Q_FUNC_INFO << "The input parameter is null. Nothing is done. Return nullptr.";
        return nullptr;
    }

    dtkWidgetsParameter *widget = dtkCorePluginFactory<dtkWidgetsParameter>::create(key);
    if(!widget) {
        qWarning() << Q_FUNC_INFO << "The widget with key :" << key << "could not be instanciated by the factory.";
        return nullptr;
    }
    if(!widget->connect(parameter)) {
        qWarning() << Q_FUNC_INFO << "The parameter could not be connected to the widget.";
    }
    return widget;
}

// ///////////////////////////////////////////////////////////////////
// Register to dtkWidgets layer implementation
// ///////////////////////////////////////////////////////////////////

namespace dtk
{
    namespace widgets
    {
        namespace parameters
        {
            namespace _private
            {
                dtkWidgetsParameterFactory       factory;
                dtkWidgetsParameterPluginManager manager;
            }

            dtkWidgetsParameterFactory& pluginFactory(void)
            {
                return _private::factory;
            }

            dtkWidgetsParameterPluginManager& pluginManager(void)
            {
                return _private::manager;
            }

            namespace _private
            {
                //dtkCorePluginManagerRecorder mrecorder(&dtk::widgets::manager(), &pluginManager(), "dtkWidgetsParameter");
                //dtkCorePluginFactoryRecorder frecorder(&dtk::widgets::manager(), &pluginFactory(), "dtkWidgetsParameter");
            }
        }
    }
}

//
// dtkWidgetsParameterFactory.cpp ends here
