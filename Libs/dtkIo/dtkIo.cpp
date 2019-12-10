// Version: $Id: $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkIoExport.h"

#include "dtkIo.h"
#include "dtkIoDataModel.h"

namespace dtkIo
{

    namespace dataModel {

        namespace _private {
            dtkIoDataModelPluginFactory factory;
            dtkIoDataModelPluginManager manager;
        }
        
        void initialize(const QString& path) {
            pluginManager().initialize(path);
        }

        dtkIoDataModelPluginManager& pluginManager(void) {
            return _private::manager;
        }

        dtkIoDataModelPluginFactory& pluginFactory(void) {
            return _private::factory;
        }
    }

}
//
// dtkIo.cpp ends here
