### CMakeLists.txt ---
##
## Author: Julien Wintz
## Copyright (C) 2008 - Julien Wintz, Inria.
## Created: Mon Nov 22 10:20:27 2010 (+0100)
######################################################################
##
### Commentary:
##
######################################################################
##
### Change log:
##
######################################################################

project(dtkPlot)

## #################################################################
## Dependencies
## #################################################################

if(DTK_BUILD_SUPPORT_PLOT AND NOT DTK_BUILD_SUPPORT_GUI)
  message(SEND_ERROR "You can't select \"DTK_BUILD_SUPPORT_PLOT\" because it's dependencies (DTK_BUILD_SUPPORT_GUI) were not met")
endif()

if(DTK_BUILD_SUPPORT_PLOT AND NOT DTK_BUILD_WIDGETS)
  message(SEND_ERROR "You can't select \"DTK_BUILD_SUPPORT_PLOT\" because it's dependencies (DTK_BUILD_WIDGETS) were not met")
endif()


if(QWT_FOUND)

include_directories(${QWT_INCLUDE_DIR})

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  dtkPlotCurve.h
  dtkPlotRenderer.h
  dtkPlotView.h
  dtkPlotViewPanner.h
  dtkPlotViewPicker.h
  dtkPlotViewZoomer.h
  dtkPlotViewLegend.h
  dtkPlotViewLegend_p.h
  dtkPlotViewGrid.h
  dtkPlotViewSettings.h
  dtkPlotViewToolBar.h)

set(${PROJECT_NAME}_SOURCES
  dtkPlotCurve.cpp
  dtkPlotRenderer.cpp
  dtkPlotView.cpp
  dtkPlotViewPanner.cpp
  dtkPlotViewPicker.cpp
  dtkPlotViewZoomer.cpp
  dtkPlotViewLegend.cpp
  dtkPlotViewGrid.cpp
  dtkPlotViewSettings.cpp
  dtkPlotViewToolBar.cpp)

set(${PROJECT_NAME}_SOURCES_QRC dtkPlot.qrc)

## #################################################################
## Build rules
## #################################################################

qt5_add_resources(${PROJECT_NAME}_SOURCES_QRC ${${PROJECT_NAME}_SOURCES_QRC})

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS}
  ${${PROJECT_NAME}_SOURCES_QRC})

target_link_libraries(${PROJECT_NAME}
  ${QWT_LIBRARIES}
# dtkCoreSupport
#  dtkGuiSupport
  dtkWidgets)

## ###################################################################
## Link rules
## ###################################################################

target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

## #################################################################
## Target properties
## #################################################################

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(${PROJECT_NAME} PROPERTIES VISIBILITY_INLINES_HIDDEN 1)
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_RPATH 0)
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")

## #################################################################
## Export header file
## #################################################################

generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export")

set(${PROJECT_NAME}_HEADERS
  ${${PROJECT_NAME}_HEADERS}
 "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export"
 "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")

## #################################################################
## Installation
## #################################################################

foreach(header ${${PROJECT_NAME}_HEADERS})
  string(REGEX REPLACE "(.*)\\.h\$" "\\1" h ${header})
  set(${PROJECT_NAME}_HEADERS_QTS "${${PROJECT_NAME}_HEADERS_QTS}" ${h})
endforeach(header)

install(FILES ${${PROJECT_NAME}_HEADERS} DESTINATION include/${PROJECT_NAME})
install(FILES ${${PROJECT_NAME}_HEADERS_QTS} DESTINATION include/${PROJECT_NAME})

#install(TARGETS ${PROJECT_NAME}
#    DESTINATION ${CMAKE_INSTALL_LIBDIR}
#      COMPONENT plotsupport
#         EXPORT dtkDepends)

endif(QWT_FOUND)
