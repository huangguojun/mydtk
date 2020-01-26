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

project(dtkWidgets)

## #################################################################
## Input
## #################################################################

set(${PROJECT_NAME}_HEADERS
  dtkApplication.h
#dtkDistributedGuiApplication.h
  dtkNotifiable.h
  dtkNotification.h
  dtkNotificationDisplay.h
  dtkNotificationEvent.h
  dtkNotificationQueue.h
  dtkObjectEditor.h
  dtkPropertyEditor.h
  dtkPropertyEditorFactory.h
  dtkRecentFilesMenu.h
  dtkScreenMenu.h
  dtkSpacer.h
  dtkSplitter.h
  dtkToolBox.h
  dtkViewController
  dtkViewController.h
  dtkViewLayout
  dtkViewLayout.h
  dtkViewLayoutItem
  dtkViewLayoutItem.h
  dtkViewList
  dtkViewList.h
  dtkViewListControl
  dtkViewListControl.h
  dtkViewManager
  dtkViewManager.h
  dtkViewWidget
  dtkViewWidget.h
  dtkWidgets
  dtkWidgetsLogView.h
  dtkWidgetsLogView_p.h
  dtkWidgetsTag.h
  dtkWidgetsTagCloud.h
  dtkWidgetsTagCloudController.h
  dtkWidgetsTagCloudDesc.h
  dtkWidgetsTagCloudItem.h
  dtkWidgetsTagCloudLayout.h
  dtkWidgetsTagCloudList.h
  dtkWidgetsTagCloudScope.h
  dtkWidgetsTagCloudScopeItem.h
  dtkWidgetsTagCloudView.h
  dtkWidgetsLayout.h
  dtkWidgetsController.h
  dtkWidgetsLayoutItem.h
#dtkWidgetsMainWindow.h
  dtkWidgetsWidget.h
  )

if(DTK_BUILD_SCRIPT)
  set(${PROJECT_NAME}_HEADERS ${${PROJECT_NAME}_HEADERS} dtkInterpreter.h)
endif(DTK_BUILD_SCRIPT)

set(${PROJECT_NAME}_SOURCES
  dtkApplication.cpp
# dtkDistributedGuiApplication.cpp
  dtkNotification.cpp
  dtkNotificationDisplay.cpp
  dtkNotificationEvent.cpp
  dtkNotificationQueue.cpp
  dtkObjectEditor.cpp
  dtkPropertyEditor.cpp
  dtkPropertyEditorFactory.cpp
  dtkRecentFilesMenu.cpp
  dtkScreenMenu.cpp
  dtkSpacer.cpp
  dtkSplitter.cpp
  dtkToolBox.cpp
  dtkViewController.cpp
  dtkViewLayout.cpp
  dtkViewLayoutItem.cpp
  dtkViewList.cpp
  dtkViewListControl.cpp
  dtkViewManager.cpp
  dtkViewWidget.cpp
  dtkWidgetsLogView.cpp
  dtkWidgetsTag.cpp
  dtkWidgetsTagCloud.cpp
  dtkWidgetsTagCloudController.cpp
  dtkWidgetsTagCloudDesc.cpp
  dtkWidgetsTagCloudItem.cpp
  dtkWidgetsTagCloudLayout.cpp
  dtkWidgetsTagCloudList.cpp
  dtkWidgetsTagCloudScope.cpp
  dtkWidgetsTagCloudScopeItem.cpp
  dtkWidgetsTagCloudView.cpp
  dtkWidgetsLayout.cpp
  dtkWidgetsController.cpp
  dtkWidgetsLayoutItem.cpp
  dtkWidgetsMainWindow.cpp
  dtkWidgetsWidget.cpp
  dtkVisualizationWidgetsActorList.cpp
  dtkVisualizationWidgetsClutEditor.cpp
  dtkVisualizationWidgetsColorMapEditor.cpp
  dtkVisualizationWidgetsFieldScalar.cpp
  dtkVisualizationWidgetsSliceControls.cpp
  dtkVisualizationWidgetsVideoControls.cpp
  dtkWidgetsHUD.cpp
  dtkWidgetsHUDItem.cpp
  dtkWidgetsHUDInfo.cpp
  dtkWidgetsOverlayPane.cpp
  dtkWidgetsOverlayPaneItem.cpp
  dtkWidgetsMenu.cpp
  dtkWidgetsMenuItem.cpp
 #dtkWidgetsMenuItemTheme.cpp
  dtkWidgetsMenuBar.cpp
  dtkWidgetsOverlayPaneSlider.cpp
  dtkWidgetsMenuSpy.cpp
  dtkWidgetsMenuItem+custom.cpp  
  dtkWidgetsMenu+ux.cpp
  dtkWidgetsParameter.cpp 
  dtkWidgetsParameterFactory.cpp

  )

if(DTK_BUILD_SCRIPT)
  set(${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_SOURCES} dtkInterpreter.cpp)
endif(DTK_BUILD_SCRIPT)

set(${PROJECT_NAME}_SOURCES_RCC
  dtkNotification.qrc
  dtkWidgetsTagCloud.qrc
  dtkVisualizationWidgets.qrc)

set_property(SOURCE qrc_dtkNotification.cpp PROPERTY SKIP_AUTOMOC ON)
set_property(SOURCE qrc_dtkWidgetsTagCloud.cpp PROPERTY SKIP_AUTOMOC ON)
 
## #################################################################
## Wrapping
## #################################################################

set(${PROJECT_NAME}_SOURCES_WRAP)

if(DTK_BUILD_WRAPPERS)

  set(${PROJECT_NAME}_WRAP_DEPENDS ${${PROJECT_NAME}_HEADERS})

  if(SIP_FOUND)
    set(${PROJECT_NAME}_SIP_WRAPPERS
      sip${PROJECT_NAME}dtkViewController.cpp
      sip${PROJECT_NAME}dtkViewManager.cpp
      sip${PROJECT_NAME}dtkViewWidget.cpp
      sip${PROJECT_NAME}dtkViewLayout.cpp
      sip${PROJECT_NAME}dtkViewLayoutItem.cpp)

    dtk_wrap_sip(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP dtkWidgets ${CMAKE_SOURCE_DIR}/sip/dtkWidgets/dtkWidgetsmod.sip "${${PROJECT_NAME}_SIP_WRAPPERS}" "${${PROJECT_NAME}_WRAP_DEPENDS}")

    set_property(SOURCE sipdtkWidgetscmodule.cpp PROPERTY SKIP_AUTOMOC ON)
    set_property(SOURCE sipdtkWidgetsdtkViewController.cpp PROPERTY SKIP_AUTOMOC ON)
    set_property(SOURCE sipdtkWidgetsdtkViewManager.cpp PROPERTY SKIP_AUTOMOC ON)
    set_property(SOURCE sipdtkWidgetsdtkViewWidget.cpp PROPERTY SKIP_AUTOMOC ON)
    set_property(SOURCE sipdtkWidgetsdtkViewLayout.cpp PROPERTY SKIP_AUTOMOC ON)
    set_property(SOURCE sipdtkWidgetsdtkViewLayoutItem.cpp PROPERTY SKIP_AUTOMOC ON)
  endif(SIP_FOUND)

  if(SWIG_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/dtkWidgets.i ${CMAKE_BINARY_DIR}/dtkWidgets.i COPYONLY)

    if (DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND)
      dtk_wrap(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP dtkwidgets python ${PROJECT_SOURCE_DIR}/dtkWidgets.i "${${PROJECT_NAME}_WRAP_DEPENDS}" "${CMAKE_SOURCE_DIR}/src;${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_SOURCE_DIR}/wrp/dtkBase;${CMAKE_SOURCE_DIR}/wrp/dtkWidgets;${CMAKE_SOURCE_DIR}/wrp/QtCore;${CMAKE_SOURCE_DIR}/wrp/QtWidgets")

      set_property(SOURCE dtkWidgets_wrap_python.cpp PROPERTY SKIP_AUTOMOC ON)
    endif()
    if (DTK_WRAPPING_TCL AND TCL_FOUND)
      dtk_wrap(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP dtkwidgets tcl ${PROJECT_SOURCE_DIR}/dtkWidgets.i "${${PROJECT_NAME}_WRAP_DEPENDS}" ".")
    endif()
    if (DTK_WRAPPING_CSHARP)
      dtk_wrap(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP dtkwidgets csharp ${PROJECT_SOURCE_DIR}/dtkWidgets.i "${${PROJECT_NAME}_WRAP_DEPENDS}" ".")
    endif()
  endif(SWIG_FOUND)
endif()

## #################################################################
## Build rules
## #################################################################

qt5_add_resources(${PROJECT_NAME}_SOURCES_QRC ${${PROJECT_NAME}_SOURCES_RCC})

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES_QRC}
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_SOURCES_WRAP}
  ${${PROJECT_NAME}_HEADERS})

target_link_libraries(${PROJECT_NAME}
  dtkCore
  dtkLog
  dtkThemes
  dtkFonts
#dtkDistributed
  )

if(DTK_BUILD_SCRIPT)
  target_link_libraries(${PROJECT_NAME} dtkScript)
endif(DTK_BUILD_SCRIPT)

if(DTK_BUILD_WRAPPERS)

 if (DTK_WRAPPING_TCL AND TCL_FOUND)
   target_link_libraries(${PROJECT_NAME} ${TCL_LIBRARY})
 endif()

  if(DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND)
      target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES})
  endif()

endif(DTK_BUILD_WRAPPERS)

## ###################################################################
## Link rules
## ###################################################################

target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_link_libraries(${PROJECT_NAME} Qt5::Gui)
target_link_libraries(${PROJECT_NAME} Qt5::Xml)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

## ###################################################################
## Target properties
## ###################################################################

if(NOT SIP_FOUND)
  set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)
  set_target_properties(${PROJECT_NAME} PROPERTIES VISIBILITY_INLINES_HIDDEN 1)
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_RPATH 0)
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION   ${dtk_VERSION}
                                                 SOVERSION ${dtk_VERSION_MAJOR})

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
## Handling of generated script modules
## #################################################################

set(${PROJECT_NAME}_MODULES)

if(DTK_BUILD_WRAPPERS AND SIP_FOUND)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      DEPENDS ${PROJECT_SOURCE_DIR}/dtkWidgets.i
      COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}
      COMMENT "-- Moving python modules to ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}")

    if(APPLE)
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS ${PROJECT_SOURCE_DIR}/dtkWidgets.i
        COMMAND ln -sf ../../libdtkWidgets.dylib dtkWidgets.so
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH})

      set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/dtkWidgets.so)
    endif(APPLE)

    if(UNIX AND NOT APPLE)
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS ${PROJECT_SOURCE_DIR}/dtkWidgets.i
        COMMAND ln -fs ../../libdtkWidgets.so dtkWidgets.so
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH})

      set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/dtkWidgets.so)
    endif(UNIX AND NOT APPLE)

endif(DTK_BUILD_WRAPPERS AND SIP_FOUND)

if(DTK_BUILD_WRAPPERS AND SWIG_FOUND)

  if(DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND )

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      DEPENDS ${PROJECT_SOURCE_DIR}/dtkWidgets.i
      COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}
      COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${${PROJECT_NAME}_BINARY_DIR}/dtkwidgets.py ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}
      COMMENT "-- Moving python modules to ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}")

    set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/dtkwidgets.py)

    if(APPLE)
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS ${PROJECT_SOURCE_DIR}/dtkWidgets.i
        COMMAND ln -sf ../../libdtkWidgets.dylib _dtkwidgets.so
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH})

      set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/_dtkwidgets.so)
    endif(APPLE)

    if(UNIX AND NOT APPLE)
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS ${PROJECT_SOURCE_DIR}/dtkWidgets.i
        COMMAND ln -fs ../../libdtkWidgets.so _dtkwidgets.so
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH})

      set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/_dtkwidgets.so)
    endif(UNIX AND NOT APPLE)

  endif(DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND )

  if (DTK_WRAPPING_CSHARP)

    file(GLOB DTK_CSHARP_SOURCES "${${PROJECT_NAME}_BINARY_DIR}/*.cs")

    foreach(DTK_CSHARP_SOURCE ${DTK_CSHARP_SOURCES})
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS ${PROJECT_SOURCE_DIR}/dtkWidgets.i
        COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${DTK_CSHARP_SOURCE} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}
        COMMENT "-- Moving csharp ${DTK_CSHARP_SOURCE} source to ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}")
    endforeach(DTK_CSHARP_SOURCE ${DTK_CSHARP_SOURCES})
  endif (DTK_WRAPPING_CSHARP)

endif(DTK_BUILD_WRAPPERS AND SWIG_FOUND)

## ###################################################################
## Install rules - files
## ###################################################################

install(FILES ${${PROJECT_NAME}_HEADERS}
  DESTINATION include/${PROJECT_NAME}
    COMPONENT widgets)

## ###################################################################
## Install rules - targets
## ###################################################################

#install(TARGETS ${PROJECT_NAME}
#    DESTINATION ${CMAKE_INSTALL_LIBDIR}
#      COMPONENT widgets
#         EXPORT dtkDepends)

## ###################################################################
## Install rules - modules
## ###################################################################

if(SWIG_FOUND)
  if (DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND)
    install(FILES ${${PROJECT_NAME}_MODULES} DESTINATION ${DTK_PYTHON_INSTALL_PATH})
    install(FILES ${CMAKE_BINARY_DIR}/dtkWidgets.i DESTINATION wrp/dtkWidgets)
  endif()
endif()

if(SIP_FOUND)
  if (DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND)
    install(FILES
      ${CMAKE_SOURCE_DIR}/sip/${PROJECT_NAME}/dtkViewController.sip
      ${CMAKE_SOURCE_DIR}/sip/${PROJECT_NAME}/dtkViewLayout.sip
      ${CMAKE_SOURCE_DIR}/sip/${PROJECT_NAME}/dtkViewLayoutItem.sip
      ${CMAKE_SOURCE_DIR}/sip/${PROJECT_NAME}/dtkViewManager.sip
      ${CMAKE_SOURCE_DIR}/sip/${PROJECT_NAME}/dtkViewWidget.sip
      ${CMAKE_SOURCE_DIR}/sip/${PROJECT_NAME}/dtkWidgetsmod.sip
      DESTINATION sip/dtkWidgets)
  endif()
endif()

######################################################################
### CMakeLists.txt ends here
