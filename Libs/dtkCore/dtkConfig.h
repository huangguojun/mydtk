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

#define dtk_VERSION_MAJOR 1
#define dtk_VERSION_MINOR 6
#define dtk_VERSION_PATCH 7

#include <QString>

static const QString DTK_INSTALL_PREFIX = "/home/hgj/Project/dtk_pro/dtk/install";

/* #undef DTK_HAVE_MPI */
#define DTK_HAVE_ZLIB
/* #undef DTK_HAVE_PLOT */

#define DTK_BUILD_COMPOSER
#define DTK_BUILD_DISTRIBUTED
/* #undef DTK_BUILD_SCRIPT */
/* #undef DTK_BUILD_WRAPPERS */
#define DTK_BUILD_WIDGETS

/* #undef DTK_BUILD_SUPPORT_CORE */
/* #undef DTK_BUILD_SUPPORT_COMPOSER */
/* #undef DTK_BUILD_SUPPORT_CONTAINER */
/* #undef DTK_BUILD_SUPPORT_DISTRIBUTED */
/* #undef DTK_BUILD_SUPPORT_GUI */
/* #undef DTK_BUILD_SUPPORT_MATH */
/* #undef DTK_BUILD_SUPPORT_PLOT */
/* #undef DTK_BUILD_SUPPORT_VR */

/* #undef DTK_BUILD_32 */
#define DTK_BUILD_64

/* #undef COMPILER_SUPPORTS_CXX11 */

//
// dtkConfig.h.in ends here
