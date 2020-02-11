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

project(dtkGui)

if(DTK_BUILD_SUPPORT_GUI AND NOT DTK_BUILD_SUPPORT_CORE)
  message(SEND_ERROR "You can't select \"DTK_BUILD_SUPPORT_GUI\" because it's dependencies (DTK_BUILD_SUPPORT_CORE) were not met")
endif()

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  dtkAboutBox.h
  dtkAboutPlugin.h
  dtkAnchoredBar.h
  dtkColorButton.h
  dtkColorGrid.h
  dtkFlowLayout.h
  dtkFinder.h
  dtkPieChart.h
  dtkPluginsEditor.h
  dtkPopup.h
  dtkProgressIndicator.h
  dtkRecentFilesMenu.h
  dtkScreenMenu.h
  dtkSearchField.h
  dtkSettingsEditor.h
  dtkSpacer.h
  dtkSplitter.h
#  dtkSwitch.h
#  dtkTagCloud.h
  dtkViewManager.h
  dtkViewLayout.h
  dtkViewLayoutItem.h
  dtkViewList.h
  dtkViewListControl.h)

set(${PROJECT_NAME}_SOURCES
  dtkAboutBox.cpp
  dtkAboutPlugin.cpp
  dtkAnchoredBar.cpp
  dtkColorButton.cpp
  dtkColorGrid.cpp
  dtkFlowLayout.cpp
  dtkFinder.cpp
  dtkPieChart.cpp
  dtkPluginsEditor.cpp
  dtkPopup.cpp
  dtkProgressIndicator.cpp
  dtkRecentFilesMenu.cpp
  dtkScreenMenu.cpp
  dtkSettingsEditor.cpp
  dtkSpacer.cpp
  dtkSplitter.cpp
#  dtkSwitch.cpp
#  dtkTagCloud.cpp
  dtkViewManager.cpp
  dtkViewLayout.cpp
  dtkViewLayoutItem.cpp
  dtkViewList.cpp
  dtkViewListControl.cpp)

if(APPLE)
set(${PROJECT_NAME}_SOURCES
  ${${PROJECT_NAME}_SOURCES}
    dtkSearchField.mm)
else(APPLE)
set(${PROJECT_NAME}_SOURCES
  ${${PROJECT_NAME}_SOURCES}
    dtkSearchField.cpp)
endif(APPLE)

set(${PROJECT_NAME}_SOURCES_RCC
  dtkGui.qrc)

## #################################################################
## Build rules
## #################################################################

qt5_add_resources(${PROJECT_NAME}_SOURCES_QRC ${${PROJECT_NAME}_SOURCES_RCC})

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS}
  ${${PROJECT_NAME}_SOURCES_QRC})

#target_link_libraries(${PROJECT_NAME}
#  dtkCoreSupport)

if(DTK_BUILD_SCRIPT)
  target_link_libraries(${PROJECT_NAME}
    dtkScript)
endif(DTK_BUILD_SCRIPT)

if(APPLE)
  target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
endif(APPLE)

## ###################################################################
## Link rules
## ###################################################################

target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_link_libraries(${PROJECT_NAME} Qt5::Gui)
#target_link_libraries(${PROJECT_NAME} Qt5::Quick)
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

install(TARGETS ${PROJECT_NAME}
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT guisupport
         EXPORT dtkDepends)

######################################################################
### CMakeLists.txt ends here
