######################################################################
##
### Code:

project(dtkVisualizationViewer)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
 dtkVisualizationViewer.h
 dtkVisualizationViewerStack.h
)

set(${PROJECT_NAME}_SOURCES
  dtkVisualizationViewer.cpp
  dtkVisualizationViewerStack.cpp
  main.cpp)

set(${PROJECT_NAME}_SOURCES_QRC dtkVisualizationViewer.qrc)

set_property(SOURCE qrc_dtkVisualizationViewer.cpp PROPERTY SKIP_AUTOMOC ON)

## #################################################################
## Build rules
## #################################################################

qt5_add_resources(${PROJECT_NAME}_SOURCES_RCC ${${PROJECT_NAME}_SOURCES_QRC})

add_executable(${PROJECT_NAME} MACOSX_BUNDLE
  ${${PROJECT_NAME}_SOURCES_RCC}
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS})

target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_link_libraries(${PROJECT_NAME} Qt5::Gui)

target_link_libraries(${PROJECT_NAME} dtkCore)
#target_link_libraries(${PROJECT_NAME} dtkCoreRuntime)
target_link_libraries(${PROJECT_NAME} dtkThemes)
target_link_libraries(${PROJECT_NAME} dtkVisualization)
#target_link_libraries(${PROJECT_NAME} dtkVisualizationWidgets)
target_link_libraries(${PROJECT_NAME} dtkWidgets)

## ###################################################################
## Bundle setup
## ###################################################################

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.plist.in ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.plist)

## #################################################################
## Install rules
## #################################################################

if (UNIX AND NOT APPLE)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.desktop.in ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.desktop)

  install(PROGRAMS ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.desktop DESTINATION ~/.local/share/applications)
endif(UNIX AND NOT APPLE)

install(FILES ${${PROJECT_NAME}_MODULES} DESTINATION modules)
install(TARGETS ${PROJECT_NAME}
   BUNDLE DESTINATION bin
  RUNTIME DESTINATION bin)

######################################################################
### CMakeLists.txt ends here
