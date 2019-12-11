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

project(dtkVisualizationWidgets VERSION ${dtkVisualization_VERSION})

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  dtkVisualizationWidgets
  dtkVisualizationWidgetsActorList
  dtkVisualizationWidgetsActorList.h
  dtkVisualizationWidgetsClutEditor
  dtkVisualizationWidgetsClutEditor.h
  dtkVisualizationWidgetsColorMapEditor
  dtkVisualizationWidgetsColorMapEditor.h
  dtkVisualizationWidgetsFieldScalar
  dtkVisualizationWidgetsFieldScalar.h
  dtkVisualizationWidgetsSliceControls
  dtkVisualizationWidgetsSliceControls.h
  dtkVisualizationWidgetsVideoControls
  dtkVisualizationWidgetsVideoControls.h
  )

set(${PROJECT_NAME}_SOURCES
  dtkVisualizationWidgetsActorList.cpp
  dtkVisualizationWidgetsClutEditor.cpp
  dtkVisualizationWidgetsColorMapEditor.cpp
  dtkVisualizationWidgetsFieldScalar.cpp
  dtkVisualizationWidgetsSliceControls.cpp
  dtkVisualizationWidgetsVideoControls.cpp
)

set(${PROJECT_NAME}_SOURCES_QRC
  dtkVisualizationWidgets.qrc)

## #################################################################
## Build rules
## #################################################################

qt5_add_resources(${PROJECT_NAME}_SOURCES_RCC ${${PROJECT_NAME}_SOURCES_QRC})

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_SOURCES_RCC}
  ${${PROJECT_NAME}_HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Gui)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Xml)

target_link_libraries(${PROJECT_NAME} PRIVATE dtkFonts)

target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Widgets)

target_link_libraries(${PROJECT_NAME} PRIVATE ${VTK_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${layer_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
  $<INSTALL_INTERFACE:include>)

## #################################################################
## Target properties
## #################################################################

set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_RPATH 0)
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION   ${${PROJECT_NAME}_VERSION}
                                                 SOVERSION ${${PROJECT_NAME}_VERSION_MAJOR})

## #################################################################
## Export header file
## #################################################################

generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export")
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")

set(${PROJECT_NAME}_HEADERS
  ${${PROJECT_NAME}_HEADERS}
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export"
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")

## ###################################################################
## Install rules - files
## ###################################################################

install(FILES ${${PROJECT_NAME}_HEADERS}
  DESTINATION include/${PROJECT_NAME})

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME} EXPORT layer-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

######################################################################
### CMakeLists.txt ends here
