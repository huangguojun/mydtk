## Version: $Id$
##
######################################################################
##
### Commentary:
##
######################################################################
##
### Change Log:
##
######################################################################
##
### Code:

set(DTK_BUILD_32 0)
set(DTK_BUILD_64 1)

## ###################################################################
## Defines:
## - dtk_INCLUDE_DIRS
## ###################################################################

set(dtk_INCLUDE_DIR "/home/hgj/Project/dtk_pro/dtk")

set(dtk_INCLUDE_DIRS
  "/home/hgj/Project/dtk_pro/dtk"
  "/home/hgj/Project/dtk_pro/dtk/include"
  "/home/hgj/Project/dtk_pro/dtk/src"
  "/home/hgj/Project/dtk_pro/dtk/src/dtk"
  "/home/hgj/Project/dtk_pro/dtk/src/dtkLog"
  "/home/hgj/Project/dtk_pro/dtk/src/dtkCore"
  "/home/hgj/Project/dtk_pro/dtk/src/dtkMath"
  "/home/hgj/Project/dtk_pro/dtk/src/dtkMeta"
  "/home/hgj/Project/dtk_pro/dtk/src/dtkTest"
  "/home/hgj/Project/dtk_pro/dtk/build")

include("/home/hgj/Project/dtk_pro/dtk/build/dtkDepends.cmake")
include("/home/hgj/Project/dtk_pro/dtk/cmake/dtkWrap.cmake")

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH} "/home/hgj/Project/dtk_pro/dtk/cmake/")

set_directory_properties(PROPERTIES TEST_INCLUDE_FILE "/home/hgj/Project/dtk_pro/dtk/build/dtkSetTestEnvironment.cmake")

set(dtk_MODULE_PATH "/home/hgj/Project/dtk_pro/dtk/build")
set(dtk_INSTALL_DOCS "/home/hgj/Project/dtk_pro/dtk/build/html")
set(dtk_CONFIG_DOCS  "/home/hgj/Project/dtk_pro/dtk/build/doc/cfg")

set(dtk_WRAP_PREFIX "/home/hgj/Project/dtk_pro/dtk/wrp")

## ###################################################################
## Options
## ###################################################################

set(DTK_BUILD_DISTRIBUTED ON)
set(DTK_BUILD_COMPOSER ON)
set(DTK_BUILD_SCRIPT ON)
set(DTK_BUILD_WRAPPERS OFF)
set(DTK_BUILD_WIDGETS ON)
set(DTK_BUILD_SUPPORT_CORE OFF)
set(DTK_BUILD_SUPPORT_CONTAINER OFF)
set(DTK_BUILD_SUPPORT_COMPOSER OFF)
set(DTK_BUILD_SUPPORT_DISTRIBUTED OFF)
set(DTK_BUILD_SUPPORT_GUI OFF)
set(DTK_BUILD_SUPPORT_MATH OFF)
set(DTK_BUILD_SUPPORT_PLOT OFF)
set(DTK_BUILD_SUPPORT_VR OFF)
set(DTK_ENABLE_COVERAGE OFF)
set(DTK_COVERAGE_USE_COBERTURA ON)
set(DTK_ENABLE_MEMCHECK OFF)

set(DTK_HAVE_MPI NO)
set(DTK_HAVE_VRPN NO)
set(DTK_HAVE_NITE )
set(DTK_HAVE_ZLIB YES)

if(DTK_BUILD_DISTRIBUTED)
  set(dtk_INCLUDE_DIRS ${dtk_INCLUDE_DIRS} "/home/hgj/Project/dtk_pro/dtk/src/dtkDistributed")
  set(dtk_INCLUDE_DIRS ${dtk_INCLUDE_DIRS} "/home/hgj/Project/dtk_pro/dtk/src/dtkDistributedTest")
endif(DTK_BUILD_DISTRIBUTED)

if(DTK_BUILD_COMPOSER)
  set(dtk_INCLUDE_DIRS ${dtk_INCLUDE_DIRS} "/home/hgj/Project/dtk_pro/dtk/src/dtkComposer")
endif(DTK_BUILD_COMPOSER)

if(DTK_BUILD_SCRIPT)
  set(dtk_INCLUDE_DIRS ${dtk_INCLUDE_DIRS} "/home/hgj/Project/dtk_pro/dtk/src/dtkScript")
endif(DTK_BUILD_SCRIPT)

if(DTK_BUILD_WRAPPERS)
  set(SIP_FOUND )
  set(SWIG_FOUND )
  set(DTK_PYTHON_WRAPPER )
  set(DTK_PYTHON_INSTALL_PATH )
  set(DTK_WRAPPING_PYTHON )
  set(DTK_WRAPPING_CSHARP )
  set(DTK_WRAPPING_TCL )
endif(DTK_BUILD_WRAPPERS)

if(DTK_BUILD_WIDGETS)
  set(dtk_INCLUDE_DIRS ${dtk_INCLUDE_DIRS} "/home/hgj/Project/dtk_pro/dtk/src/dtkWidgets")
endif(DTK_BUILD_WIDGETS)

if(DTK_BUILD_SUPPORT_CORE)
  set(dtk_INCLUDE_DIRS ${dtk_INCLUDE_DIRS} "/home/hgj/Project/dtk_pro/dtk/include/dtkCoreSupport")
endif(DTK_BUILD_SUPPORT_CORE)

if(DTK_BUILD_SUPPORT_CONTAINER)
  set(dtk_INCLUDE_DIRS ${dtk_INCLUDE_DIRS} "/home/hgj/Project/dtk_pro/dtk/include/dtkContainerSupport")
endif(DTK_BUILD_SUPPORT_CONTAINER)

if(DTK_BUILD_SUPPORT_MATH)
  set(dtk_INCLUDE_DIRS ${dtk_INCLUDE_DIRS} "/home/hgj/Project/dtk_pro/dtk/include/dtkMathSupport")
endif(DTK_BUILD_SUPPORT_MATH)

if(DTK_BUILD_SUPPORT_GUI)
  set(dtk_INCLUDE_DIRS ${dtk_INCLUDE_DIRS} "/home/hgj/Project/dtk_pro/dtk/include/dtkGuiSupport")
endif(DTK_BUILD_SUPPORT_GUI)

## ###################################################################
## Helper function
## ###################################################################

function(PREPEND var prefix)
   set(listVar "")
   foreach(f ${ARGN})
      list(APPEND listVar "${prefix}/${f}")
   endforeach(f)
   set(${var} "${listVar}" PARENT_SCOPE)
endfunction(PREPEND)

######################################################################
### dtkConfig.cmake.in ends here
