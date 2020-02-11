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

project(dtkMath)

## #################################################################
## Input
## #################################################################

set(${PROJECT_NAME}_HEADERS
  dtkGraph
  dtkGraph.h
  dtkGraphEdge
  dtkGraphEdge.h
  dtkGraphTemplate
  dtkGraphTemplate.h
  dtkGraphTemplate.tpp
  dtkGraphTemplateEdge
  dtkGraphTemplateEdge.h
  dtkGraphTemplateEdge.tpp
  dtkMath
  dtkMath.h
  dtkMathArray.h
  dtkMathArray.tpp
  dtkMatrix
  dtkMatrix.h
  dtkMatrix_p.h
  dtkMatrix.tpp)

set(${PROJECT_NAME}_SOURCES
  dtkGraph.cpp
  dtkGraphEdge.cpp
  dtkMath.cpp
  dtkMathArray.cpp
  dtkMatrix.cpp)

## #################################################################
## Build rules
## #################################################################

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(${PROJECT_NAME} PROPERTIES VISIBILITY_INLINES_HIDDEN 1)
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_RPATH 0)
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION   ${dtk_VERSION}
                                                 SOVERSION ${dtk_VERSION_MAJOR})

## ###################################################################
## LINK rules
## ###################################################################

target_link_libraries(${PROJECT_NAME} Qt5::Core)

target_link_libraries(${PROJECT_NAME} dtkCore)
target_link_libraries(${PROJECT_NAME} dtkLog)

## #################################################################
## Export header file
## #################################################################

generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export")

set(${PROJECT_NAME}_HEADERS
  ${${PROJECT_NAME}_HEADERS}
 "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export"
 "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")

## ###################################################################
## Install rules - files
## ###################################################################

install(FILES ${${PROJECT_NAME}_HEADERS}
  DESTINATION include/${PROJECT_NAME}
    COMPONENT math)

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT math
        EXPORT dtkDepends)

######################################################################
### CMakeLists.txt ends here
