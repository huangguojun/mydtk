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

project(dtkScript)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  dtkScript
  dtkScriptInterpreter.h)

set(${PROJECT_NAME}_SOURCES
  dtkScriptInterpreter.cpp)

if(DTK_BUILD_WRAPPERS AND DTK_HAVE_PYTHON)

set(${PROJECT_NAME}_HEADERS ${${PROJECT_NAME}_HEADERS}
  dtkScriptInterpreterPython.h)

set(${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_SOURCES}
  dtkScriptInterpreterPython.cpp)

endif(DTK_BUILD_WRAPPERS AND DTK_HAVE_PYTHON)

if(DTK_BUILD_WRAPPERS AND DTK_HAVE_TCL)

set(${PROJECT_NAME}_HEADERS ${${PROJECT_NAME}_HEADERS}
  dtkScriptInterpreterTcl.h)

set(${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_SOURCES}
  dtkScriptInterpreterTcl.cpp)

endif(DTK_BUILD_WRAPPERS AND DTK_HAVE_TCL)

## #################################################################
## Build rules
## #################################################################

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS})

target_link_libraries(${PROJECT_NAME} Qt5::Core)

target_link_libraries(${PROJECT_NAME}
  dtkCore
  dtkLog)

if(DTK_BUILD_WRAPPERS AND DTK_HAVE_TCL)

target_link_libraries(${PROJECT_NAME} ${TCL_LIBRARY})

endif(DTK_BUILD_WRAPPERS AND DTK_HAVE_TCL)

if(DTK_BUILD_WRAPPERS AND DTK_HAVE_PYTHON)

target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES})

endif(DTK_BUILD_WRAPPERS AND DTK_HAVE_PYTHON)

## #################################################################
## Target properties
## #################################################################

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(${PROJECT_NAME} PROPERTIES VISIBILITY_INLINES_HIDDEN 1)
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_RPATH 0)
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION   ${dtk_VERSION}
                                                 SOVERSION ${dtk_VERSION_MAJOR})

## #################################################################
## Export header file
## #################################################################

#generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")
#generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export")

set(${PROJECT_NAME}_HEADERS
  ${${PROJECT_NAME}_HEADERS}
 "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export"
 "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")

## #################################################################
## Installation
## #################################################################

install(FILES ${${PROJECT_NAME}_HEADERS}
  DESTINATION include/${PROJECT_NAME}
    COMPONENT script)

## ###################################################################
## Install rules - targets
## ###################################################################

#install(TARGETS ${PROJECT_NAME}
#    DESTINATION ${CMAKE_INSTALL_LIBDIR}
#      COMPONENT script
#         EXPORT dtkDepends)

######################################################################
### CMakeLists.txt ends here
