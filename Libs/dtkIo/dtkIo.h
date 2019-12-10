// Version: $Id: 447bbe07fa943e3970bc4828b2b96190f7b17811 $
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

#include "dtkIoExport.h"

#include <dtkCore>

class dtkIoDataModelPluginFactory;
class dtkIoDataModelPluginManager;

// ///////////////////////////////////////////////////////////////////
// dtkIo factories and managers
// ///////////////////////////////////////////////////////////////////

namespace dtkIo
{
    namespace dataModel {
        DTKIO_EXPORT void                          initialize(const QString& path);
        DTKIO_EXPORT dtkIoDataModelPluginFactory&  pluginFactory(void);
        DTKIO_EXPORT dtkIoDataModelPluginManager&  pluginManager(void);
    }
}

//
// dtkIo.h ends here
