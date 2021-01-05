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

#pragma once

#include <dtkWidgetsExport>

#include "dtkWidgetsParameter.h"

#include <dtkCore>

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterPlugin declaration
// ///////////////////////////////////////////////////////////////////

class DTKWIDGETS_EXPORT dtkWidgetsParameterPlugin : public dtkCorePluginBase
{
};

Q_DECLARE_METATYPE(dtkWidgetsParameterPlugin *);
Q_DECLARE_INTERFACE(dtkWidgetsParameterPlugin,
                    DTK_DECLARE_PLUGIN_INTERFACE(dtkWidgetsParameterPlugin));

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterPluginManager declaration
// ///////////////////////////////////////////////////////////////////

class DTKWIDGETS_EXPORT dtkWidgetsParameterPluginManager
    : public dtkCorePluginManager<dtkWidgetsParameterPlugin>
{
};

// ///////////////////////////////////////////////////////////////////
// dtkWidgetsParameterFactory declaration
// ///////////////////////////////////////////////////////////////////

class DTKWIDGETS_EXPORT dtkWidgetsParameterFactory
    : public dtkCorePluginFactory<dtkWidgetsParameter>
{
public:
    dtkWidgetsParameter *create(const QString &key);
    dtkWidgetsParameter *create(dtkCoreParameter *parameter, const QString &key);
};

// /////////////////////////////////////////////////////////////////
// Register to dtkWidgets layer declaration
// /////////////////////////////////////////////////////////////////

namespace dtk {
namespace widgets {
namespace parameters {
DTKWIDGETS_EXPORT dtkWidgetsParameterFactory &pluginFactory(void);
DTKWIDGETS_EXPORT dtkWidgetsParameterPluginManager &pluginManager(void);
} // namespace parameters
} // namespace widgets
} // namespace dtk

//
// dtkWidgetsParameterFactory.h ends here
