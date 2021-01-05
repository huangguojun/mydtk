// Version: $Id: d3e9ca050eff2ca5de3bd3577c1adfd57e7e021d $
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

#include <dtkMathExport.h>

#include <QtCore>

class dtkMatrixPluginFactory;
class dtkMatrixPluginManager;

namespace dtkMath {
namespace matrix {
DTKMATH_EXPORT dtkMatrixPluginFactory &pluginFactory(void);
DTKMATH_EXPORT dtkMatrixPluginManager &pluginManager(void);
} // namespace matrix
} // namespace dtkMath

//
// dtkMath.h ends here
