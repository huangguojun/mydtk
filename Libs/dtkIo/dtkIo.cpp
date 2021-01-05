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

namespace dtkIo {

namespace dataModel {

namespace _private {
dtkIoDataModelPluginFactory factory;
dtkIoDataModelPluginManager manager;
} // namespace _private

void initialize(const QString &path)
{
    pluginManager().initialize(path);
}

dtkIoDataModelPluginManager &pluginManager(void)
{
    return _private::manager;
}

dtkIoDataModelPluginFactory &pluginFactory(void)
{
    return _private::factory;
}
} // namespace dataModel

} // namespace dtkIo
//
// dtkIo.cpp ends here
