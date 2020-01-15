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

project(dtkVisualization VERSION ${dtkVisualization_VERSION})

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  dtkVisualization
  dtkVisualization.h
  dtkVisualizationCanvas
  dtkVisualizationCanvas.h
#  dtkVisualizationDecorator
#  dtkVisualizationDecorator.h
#  dtkVisualizationDecoratorAxes
#  dtkVisualizationDecoratorAxes.h
#  dtkVisualizationDecoratorClutEditor
#  dtkVisualizationDecoratorClutEditor.h
#  dtkVisualizationDecoratorClutEditorBase
# dtkVisualizationDecoratorClutEditorBase.h
#  dtkVisualizationDecoratorClutEditorSimple
#  dtkVisualizationDecoratorClutEditorSimple.h
#  dtkVisualizationDecoratorCollection
#  dtkVisualizationDecoratorCollection.h
#  dtkVisualizationDecoratorDelaunay2D
#  dtkVisualizationDecoratorDelaunay2D.h
#  dtkVisualizationDecoratorInfo
#  dtkVisualizationDecoratorInfo.h
#  dtkVisualizationDecoratorPoints
#  dtkVisualizationDecoratorPoints.h
#  dtkVisualizationDecoratorScalarColorMap
# dtkVisualizationDecoratorScalarColorMap.h
#  dtkVisualizationDecoratorScalarGlyphs
#  dtkVisualizationDecoratorScalarGlyphs.h
#  dtkVisualizationDecoratorScalarIsolines
#  dtkVisualizationDecoratorScalarIsolines.h
#  dtkVisualizationDecoratorSlices
#  dtkVisualizationDecoratorSlices.h
#  dtkVisualizationDecoratorSnapshot
#  dtkVisualizationDecoratorSnapshot.h
#  dtkVisualizationDecoratorVectorStreamlines
#  dtkVisualizationDecoratorVectorStreamlines.h
#  dtkVisualizationDecoratorVolume
#  dtkVisualizationDecoratorVolume.h
#  dtkVisualizationDecoratorWithClut
#  dtkVisualizationDecoratorWithClut.h
#  dtkVisualizationMetaType
# dtkVisualizationMetaType.h
#  dtkVisualizationMetaType.tpp
   dtkVisualizationPlot2D
   dtkVisualizationPlot2D.h
   dtkVisualizationPlot3D
   dtkVisualizationPlot3D.h
   dtkVisualizationTypeTraits
   dtkVisualizationTypeTraits.h
   dtkVisualizationVTKReader
   dtkVisualizationVTKReader.h
   dtkVisualizationViewHybrid
   dtkVisualizationViewHybrid.h
   dtkVisualizationViewOverlay
   dtkVisualizationViewOverlay.h
  dtkVisualizationView2D
  dtkVisualizationView2D.h
  dtkVisualizationView3D
  dtkVisualizationView3D.h
  dtkVisualizationViewVideo
  dtkVisualizationViewVideo.h
  dtkVisualizationViewVideoGL
  dtkVisualizationViewVideoGL.h
  dtkVisualizationViewVideoPlayer
  dtkVisualizationViewVideoPlayer.h
  dtkVisualizationViewVideoRaster
  dtkVisualizationViewVideoRaster.h
  )

set(${PROJECT_NAME}_SOURCES
  dtkVisualization.cpp
  dtkVisualizationCanvas.cpp
#  dtkVisualizationDecorator.cpp
#  dtkVisualizationDecoratorAxes.cpp
#  dtkVisualizationDecoratorClutEditor.cpp
#  dtkVisualizationDecoratorClutEditorSimple.cpp
   dtkVisualizationDecoratorCollection.cpp
#  dtkVisualizationDecoratorDelaunay2D.cpp
#   dtkVisualizationDecoratorInfo.cpp
#  dtkVisualizationDecoratorPoints.cpp
#  dtkVisualizationDecoratorScalarColorMap.cpp
#  dtkVisualizationDecoratorScalarGlyphs.cpp
#  dtkVisualizationDecoratorScalarIsolines.cpp
#  dtkVisualizationDecoratorSlices.cpp
#  dtkVisualizationDecoratorSnapshot.cpp
#  dtkVisualizationDecoratorVectorStreamlines.cpp
# dtkVisualizationDecoratorVolume.cpp
#  dtkVisualizationDecoratorWithClut.cpp
  dtkVisualizationMetaType.cpp
  dtkVisualizationPlot2D.cpp
  dtkVisualizationPlot3D.cpp
  dtkVisualizationVTKReader.cpp
  dtkVisualizationViewHybrid.cpp
  dtkVisualizationViewOverlay.cpp
  dtkVisualizationView2D.cpp
  dtkVisualizationView3D.cpp
  dtkVisualizationViewVideo.cpp
  dtkVisualizationViewVideoGL.cpp
  dtkVisualizationViewVideoPlayer.cpp
  dtkVisualizationViewVideoRaster.cpp
  )

set(${PROJECT_NAME}_SOURCES_QRC dtkVisualization.qrc)

set_property(SOURCE qrc_dtkVisualization.cpp PROPERTY SKIP_AUTOMOC ON)

## ###################################################################
## Config file
## ###################################################################

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.h.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.h")

set(${PROJECT_NAME}_HEADERS
  ${${PROJECT_NAME}_HEADERS}
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.h")

set(layer_BINARY_DIR "${PROJECT_BINARY_DIR}/..")

## #################################################################
## Build rules
## #################################################################

qt5_add_resources(${PROJECT_NAME}_SOURCES_RCC ${${PROJECT_NAME}_SOURCES_QRC})

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES_RCC}
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS})

#target_link_libraries(${PROJECT_NAME} PRIVATE QtMultimedia_Private)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Multimedia)
#target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::MultimediaWidgets)

target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Gui)

target_link_libraries(${PROJECT_NAME} PRIVATE dtkThemes)
#target_link_libraries(${PROJECT_NAME} PRIVATE dtkVisualizationWidgets)

target_link_libraries(${PROJECT_NAME} PUBLIC dtkCore)
target_link_libraries(${PROJECT_NAME} PUBLIC dtkFonts)
target_link_libraries(${PROJECT_NAME} PUBLIC dtkWidgets)

target_link_libraries(${PROJECT_NAME} PUBLIC ${VTK_LIBRARIES})

if(WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
endif(WIN32)

if(APPLE)
  target_link_libraries(${PROJECT_NAME} PRIVATE "-framework OpenGL")
endif(APPLE)

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${layer_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
  $<BUILD_INTERFACE:${layer_BINARY_DIR}>
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

#install(TARGETS ${PROJECT_NAME} EXPORT layer-targets
#  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

######################################################################
### CMakeLists.txt ends here
