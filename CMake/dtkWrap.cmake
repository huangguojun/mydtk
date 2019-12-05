## Version: $Id: b376c88ca059aa2da2994e4d19385bee7bd71f1a $
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

if(DTK_BUILD_WRAPPERS)

## #################################################################
## Python
## #################################################################

if(DTK_WRAPPING_PYTHON)
  set(DTK_PYTHON_VERSION 2 CACHE STRING "Python version to use: 2, 2.x, 3, 3.x, or empty")

  find_package(PythonInterp ${DTK_PYTHON_VERSION})
  find_package(PythonLibs QUIET)

  if(PYTHONLIBS_FOUND)
    set(DTK_PYTHON_INSTALL_PATH "${CMAKE_INSTALL_LIBDIR}/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages" CACHE PATH "install subdirectory for python modules" )

    include_directories(${PYTHON_INCLUDE_DIRS})
    get_filename_component(PYTHON_PATH ${PYTHON_LIBRARIES} PATH)
    link_directories(${PYTHON_PATH})
    add_definitions(-DHAVE_PYTHON) # Towards deprecation
    set(DTK_HAVE_PYTHON 1)
  else(PYTHONLIBS_FOUND)
    set(DTK_HAVE_PYTHON 0)
  endif(PYTHONLIBS_FOUND)

endif(DTK_WRAPPING_PYTHON)

if(NOT DTK_PYTHON_WRAPPER)
  set(DTK_PYTHON_WRAPPER SWIG CACHE STRING "Choose the type of python wrapping")
else()
  set(DTK_PYTHON_WRAPPER ${DTK_PYTHON_WRAPPER} CACHE STRING "dtk python wrapper")
endif()

set_property(CACHE DTK_PYTHON_WRAPPER PROPERTY STRINGS "SWIG_AND_SIP" "SWIG" "SIP")

if(DTK_PYTHON_WRAPPER MATCHES "SIP")
  find_package(SIP)
endif()

if(DTK_PYTHON_WRAPPER MATCHES "SWIG")
  find_package(SWIG QUIET)
endif()

if(SIP_FOUND)
  set(SIP_PLATFORM "WS_X11")
  if(APPLE)
    set(SIP_PLATFORM "WS_MACX")
  endif()
  if(WIN32)
    set(SIP_PLATFORM "WS_WIN")
  endif()

  macro(dtk_wrap_sip project target name input includes deps)

    set(wrap_output sip${name}cmodule.cpp  ${includes})

    add_custom_command(
      OUTPUT ${wrap_output}
      COMMAND ${SIP_EXECUTABLE}
      ARGS
      "-I" "${PYQT5_MODULE_DIR}/"
      "-I" "${dtk_INCLUDE_DIR}/sip"
      "-t" "${SIP_PLATFORM}"
      "-c" "${CMAKE_CURRENT_BINARY_DIR}/"
      ${input}
      MAIN_DEPENDENCY ${deps}
      COMMENT "Wrapping ${input} to python SIP")

    set(${target} ${${target}} ${wrap_output})
  endmacro(dtk_wrap_sip)

endif(SIP_FOUND)

if(SWIG_FOUND)

  include(${SWIG_USE_FILE})

  set(CMAKE_SWIG_FLAGS "")

  macro(dtk_wrap project target name language input deps swig_includes)

    set(wrap_output ${project}_wrap_${language}.cpp)

    string(REGEX REPLACE "([^;]+)" "-I\\1" include_dirs "${swig_includes}")

    add_custom_command(
      OUTPUT ${wrap_output}
      COMMAND ${SWIG_EXECUTABLE}
      ARGS
      "-${language}"
      "-c++"
      "-module" ${name}
      ${include_dirs}
      "-outdir" ${CMAKE_CURRENT_BINARY_DIR}
      "-o" ${wrap_output}
      ${input}
      MAIN_DEPENDENCY ${deps}
      COMMENT "Wrapping ${input} to ${language}")

    set(${target} ${${target}} ${wrap_output})

  endmacro(dtk_wrap)

  if(DTK_BUILD_WRAPPERS)

    if(${PROJECT_NAME} STREQUAL "dtk")
      option(DTK_WRAPPING_PYTHON "Build python wrapper" OFF)
      option(DTK_WRAPPING_TCL    "Build TCL wrapper" OFF)
      option(DTK_WRAPPING_CSHARP "Build C# wrapper" OFF)
    endif()

    if(NOT ${PROJECT_NAME} STREQUAL "dtk")
      mark_as_advanced(DTK_WRAPPING_PYTHON)
      mark_as_advanced(DTK_WRAPPING_TCL)
      mark_as_advanced(DTK_WRAPPING_CSHARP)
    endif()

  endif()

else(SWIG_FOUND)

  if(DTK_PYTHON_WRAPPER MATCHES 'SWIG')
    message("DTK_BUILD_WRAPPERS options requires SWIG.")
  endif(DTK_PYTHON_WRAPPER MATCHES 'SWIG')

endif(SWIG_FOUND)

if(SWIG_FOUND)
  add_definitions(-DHAVE_SWIG)
endif(SWIG_FOUND)

## #################################################################
## Tcl
## #################################################################

if(DTK_WRAPPING_TCL)

find_package(TCL QUIET)

if(TCL_FOUND)
  include_directories(${TCL_INCLUDE_PATH})
endif(TCL_FOUND)

if(TCL_FOUND)
  add_definitions(-DHAVE_TCL)
  set(DTK_HAVE_TCL 1)
else(TCL_FOUND)
  set(DTK_HAVE_TCL 0)
endif(TCL_FOUND)

endif(DTK_WRAPPING_TCL)


# mark_as_advanced(SWIG_DIR)
# mark_as_advanced(SWIG_EXECUTABLE)
# mark_as_advanced(SWIG_VERSION)

# mark_as_advanced(SIP_DIR)
# mark_as_advanced(SIP_VERSION)
# mark_as_advanced(SIP_EXECUTABLE)
mark_as_advanced(_find_sip_py)

mark_as_advanced(SIP_DEFAULT_SIP_DIR)
mark_as_advanced(PYQT5_MODULE_DIR)
mark_as_advanced(Qt5DIR)
mark_as_advanced(SIP_MODULE_DIR)

endif(DTK_BUILD_WRAPPERS)

######################################################################
### dtkWrap.cmake ends here
