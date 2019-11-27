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

project(dtkCore)

## #################################################################
## Input
## #################################################################

set(${PROJECT_NAME}_HEADERS
  dtkApplicationPrivate.h
  dtkArray
  dtkArray.h
  dtkArray.tpp
  dtkArrayData
  dtkArrayData.h
  dtkAssert
  dtkAssert.h
  dtkCore
  dtkCore.h
  dtkCoreApplication.h
  dtkCoreLayerManager
  dtkCoreLayerManager.h
  dtkCorePlugin
  dtkCorePlugin.h
  dtkCorePluginBase.h
  dtkCorePluginFactory
  dtkCorePluginFactory.h
  dtkCorePluginFactory_p.h
  dtkCorePluginFactory.tpp
  dtkCorePluginFactoryRecorder
  dtkCorePluginFactoryRecorder.h
  dtkCorePluginFactoryTemplate
  dtkCorePluginFactoryTemplate.h
  dtkCorePluginManager
  dtkCorePluginManager.h
  dtkCorePluginManager.tpp
  dtkCorePluginManagerHandler
  dtkCorePluginManagerHandler.h
  dtkCorePluginManagerRecorder
  dtkCorePluginManagerRecorder.h
  dtkCorePluginWidgetManager
  dtkCorePluginWidgetManager.h
  dtkCoreSettings
  dtkCoreSettings.h
  dtkHash
  dtkHash.h
  dtkIterator
  dtkIterator.h
  dtkObjectManager
  dtkObjectManager.h
  dtkSpinLock.h
  dtkStaticArray
  dtkStaticArray.h)

set(${PROJECT_NAME}_SOURCES
  dtkApplicationPrivate.cpp
  dtkCoreApplication.cpp
  dtkCoreSettings.cpp
  dtkArrayData.cpp
  dtkCoreLayerManager.cpp
  dtkCorePluginFactory_p.cpp
  dtkCorePluginFactory.cpp
  dtkCorePluginFactoryRecorder.cpp
  dtkCorePluginManagerHandler.cpp
  dtkCorePluginManagerRecorder.cpp
  dtkCorePluginWidgetManager.cpp
  dtkObjectManager.cpp)

## #################################################################
## Deps
## #################################################################

find_package(ZLIB QUIET)

if (DTK_HAVE_ZLIB)
  set(${PROJECT_NAME}_HEADERS ${${PROJECT_NAME}_HEADERS}
    dtkIOCompressor
    dtkIOCompressor.h)

  set(${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_SOURCES}
    dtkIOCompressor.cpp)
endif (DTK_HAVE_ZLIB)

## #################################################################
## Wrapping
## #################################################################

set(${PROJECT_NAME}_SOURCES_WRAP)

if(DTK_BUILD_WRAPPERS)
  set(${PROJECT_NAME}_WRAP_DEPENDS ${${PROJECT_NAME}_HEADERS})

  if(SIP_FOUND)
    set(${PROJECT_NAME}_SIP_WRAPPERS
      sip${PROJECT_NAME}dtkCorePluginBase.cpp
      sip${PROJECT_NAME}dtkArray1800.cpp
      sip${PROJECT_NAME}dtkArray2400.cpp
      sip${PROJECT_NAME}dtkArray3800.cpp
      sip${PROJECT_NAME}dtkCoreLayerManager.cpp
      sip${PROJECT_NAME}dtkCorePluginManagerBase.cpp
      sip${PROJECT_NAME}dtkObjectManager.cpp)

    dtk_wrap_sip(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP dtkCore ${CMAKE_SOURCE_DIR}/sip/dtkCore/dtkCoremod.sip "${${PROJECT_NAME}_SIP_WRAPPERS}" "${${PROJECT_NAME}_WRAP_DEPENDS}")

    set_property(SOURCE sipdtkCorecmodule.cpp PROPERTY SKIP_AUTOMOC ON)
    set_property(SOURCE sipdtkCoredtkCorePluginBase.cpp PROPERTY SKIP_AUTOMOC ON)
    set_property(SOURCE sipdtkCoredtkArray1800.cpp PROPERTY SKIP_AUTOMOC ON)
    set_property(SOURCE sipdtkCoredtkArray2400.cpp PROPERTY SKIP_AUTOMOC ON)
    set_property(SOURCE sipdtkCoredtkArray3800.cpp PROPERTY SKIP_AUTOMOC ON)
    set_property(SOURCE sipdtkCoredtkCoreLayerManager.cpp PROPERTY SKIP_AUTOMOC ON)
    set_property(SOURCE sipdtkCoredtkCorePluginManagerBase.cpp PROPERTY SKIP_AUTOMOC ON)
    set_property(SOURCE sipdtkCoredtkObjectManager.cpp PROPERTY SKIP_AUTOMOC ON)

  endif(SIP_FOUND)

  if(SWIG_FOUND)
    set(${PROJECT_NAME}_WRAP_DEPENDS ${${PROJECT_NAME}_WRAP_DEPENDS} ${CMAKE_SOURCE_DIR}/wrp/dtkCore/dtkCore.i)

    if (DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND)

      configure_file(${CMAKE_CURRENT_SOURCE_DIR}/dtkCore.i ${CMAKE_SOURCE_DIR}/wrp/dtkCore/dtkCore.i)

      dtk_wrap(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP dtkcore python ${PROJECT_SOURCE_DIR}/dtkCore.i "${${PROJECT_NAME}_WRAP_DEPENDS}" "${CMAKE_SOURCE_DIR}/src;${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_SOURCE_DIR}/wrp/dtkBase;${CMAKE_SOURCE_DIR}/wrp/QtCore")

      set_property(SOURCE dtkCore_wrap_python.cpp PROPERTY SKIP_AUTOMOC ON)
    endif()

    if (DTK_WRAPPING_TCL AND TCL_FOUND)
      dtk_wrap(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP dtkcore tcl ${PROJECT_SOURCE_DIR}/dtkCore.i "${${PROJECT_NAME}_WRAP_DEPENDS}" ".")
    endif()

    if (DTK_WRAPPING_CSHARP)
      dtk_wrap(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP dtkcore csharp ${PROJECT_SOURCE_DIR}/dtkCore.i "${${PROJECT_NAME}_WRAP_DEPENDS}" ".")
    endif()
  endif(SWIG_FOUND)

endif(DTK_BUILD_WRAPPERS)

## #################################################################
## Build rules
## #################################################################

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_SOURCES_WRAP}
  ${${PROJECT_NAME}_HEADERS})

## ###################################################################
## Link rules
## ###################################################################

if (DTK_HAVE_ZLIB)
  target_link_libraries(${PROJECT_NAME} ${ZLIB_LIBRARIES} )
endif (DTK_HAVE_ZLIB)

target_link_libraries(${PROJECT_NAME} dtkMeta dtkLog)
target_link_libraries(${PROJECT_NAME} Qt5::Core)

if(DTK_BUILD_WRAPPERS)
 if (DTK_WRAPPING_TCL AND TCL_FOUND)
   target_link_libraries(${PROJECT_NAME} ${TCL_LIBRARY})
 endif()
 if(DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND)
      target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES})
    endif()
endif(DTK_BUILD_WRAPPERS)

## #################################################################
## Target properties
## #################################################################

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

#generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")
#generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export")

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
      DEPENDS ${CMAKE_SOURCE_DIR}/sip/${PROJECT_NAME}/${PROJECT_NAME}mod.sip
      COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}
      COMMENT "-- Moving python modules to ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}")

    if(APPLE)
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS ${CMAKE_SOURCE_DIR}/sip/${PROJECT_NAME}/${PROJECT_NAME}mod.sip
        COMMAND ln -sf ../../lib${PROJECT_NAME}.dylib ${PROJECT_NAME}.so
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH})

      set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/${PROJECT_NAME}.so)
    endif(APPLE)

    if(UNIX AND NOT APPLE)
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS ${CMAKE_SOURCE_DIR}/sip/${PROJECT_NAME}/${PROJECT_NAME}mod.sip
        COMMAND ln -fs ../../lib${PROJECT_NAME}.so ${PROJECT_NAME}.so
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH})

      set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/${PROJECT_NAME}.so)
    endif(UNIX AND NOT APPLE)

endif(DTK_BUILD_WRAPPERS AND SIP_FOUND)

if(DTK_BUILD_WRAPPERS AND SWIG_FOUND)

  if(DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND )

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      DEPENDS ${PROJECT_SOURCE_DIR}/dtkCore.i
      COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}
      COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${${PROJECT_NAME}_BINARY_DIR}/dtkcore.py ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}
      COMMENT "-- Moving python modules to ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}")

    set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/dtkcore.py)

    if(APPLE)
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS ${PROJECT_SOURCE_DIR}/dtkCore.i
        COMMAND ln -sf ../../libdtkCore.dylib _dtkcore.so
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH})

      set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/_dtkcore.so)
    endif(APPLE)

    if(UNIX AND NOT APPLE)
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS ${PROJECT_SOURCE_DIR}/dtkCore.i
        COMMAND ln -fs ../../libdtkCore.so _dtkcore.so
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH})

      set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/_dtkcore.so)
    endif(UNIX AND NOT APPLE)

  endif(DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND )

  if (DTK_WRAPPING_CSHARP)

    file(GLOB DTK_CSHARP_SOURCES "${${PROJECT_NAME}_BINARY_DIR}/*.cs")

    foreach(DTK_CSHARP_SOURCE ${DTK_CSHARP_SOURCES})
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS ${PROJECT_SOURCE_DIR}/dtkCore.i
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
    COMPONENT core)

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT core
         EXPORT dtkDepends)

## ###################################################################
## Install rules - modules
## ###################################################################

if(SWIG_FOUND)
  if (DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND)
    install(FILES ${${PROJECT_NAME}_MODULES} DESTINATION ${DTK_PYTHON_INSTALL_PATH})
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/dtkCore.i DESTINATION wrp/dtkCore/)
  endif()
endif()

######################################################################
### CMakeLists.txt ends here
