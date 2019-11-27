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

project(dtkLog)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  dtkLog
  dtkLog.h
  dtkLogger.h
  dtkLogger_p.h
  dtkLogEngine.h
  dtkLogDestination.h
  dtkLogModel.h)

set(${PROJECT_NAME}_SOURCES
  dtkLog.cpp
  dtkLogger.cpp
  dtkLogEngine.cpp
  dtkLogDestination.cpp
  dtkLogModel.cpp)

## #################################################################
## Wrapping
## #################################################################

set(${PROJECT_NAME}_SOURCES_WRAP)

if(DTK_BUILD_WRAPPERS AND SWIG_FOUND)

  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/dtkLog.i ${CMAKE_BINARY_DIR}/dtkLog.i COPYONLY)

  set(${PROJECT_NAME}_WRAP_DEPENDS ${CMAKE_BINARY_DIR}/dtkLog.i ${${PROJECT_NAME}_HEADERS})

  if (DTK_WRAPPING_PYTHON)
    if (PYTHONLIBS_FOUND)
      dtk_wrap(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP dtklog python ${PROJECT_SOURCE_DIR}/dtkLog.i "${${PROJECT_NAME}_WRAP_DEPENDS}" "${CMAKE_SOURCE_DIR}/src;${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_SOURCE_DIR}/wrp/dtkBase;${CMAKE_SOURCE_DIR}/wrp/QtCore")

      set_property(SOURCE dtkLog_wrap_python.cpp PROPERTY SKIP_AUTOMOC ON)
    endif()
  endif()

  if (DTK_WRAPPING_TCL)
    if (TCL_FOUND)
      dtk_wrap(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP dtklog tcl ${PROJECT_SOURCE_DIR}/dtkLog.i "${${PROJECT_NAME}_WRAP_DEPENDS}" ".")
    endif()
  endif()

  if (DTK_WRAPPING_CSHARP)
    dtk_wrap(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP dtklog csharp ${PROJECT_SOURCE_DIR}/dtkLog.i "${${PROJECT_NAME}_WRAP_DEPENDS}" ".")
  endif()
endif()

## #################################################################
## Build rules
## #################################################################

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_SOURCES_WRAP}
  ${${PROJECT_NAME}_HEADERS})

## #################################################################
## Link rules
## #################################################################

target_link_libraries(${PROJECT_NAME} Qt5::Core)

if(DTK_BUILD_WRAPPERS AND SWIG_FOUND)

 if (DTK_WRAPPING_TCL AND TCL_FOUND)
   target_link_libraries(${PROJECT_NAME} ${TCL_LIBRARY})
 endif()

  if(DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND)
      target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES})
  endif()

endif(DTK_BUILD_WRAPPERS AND SWIG_FOUND)

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
## Handling of generated script modules
## #################################################################

set(${PROJECT_NAME}_MODULES)

if(DTK_BUILD_WRAPPERS AND SWIG_FOUND)

  if(DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND )

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      DEPENDS ${PROJECT_SOURCE_DIR}/dtkLog.i
      COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}
      COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${${PROJECT_NAME}_BINARY_DIR}/dtklog.py ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}
      COMMENT "-- Moving python modules to ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}")

    set(${PROJECT_NAME}_MODULES ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/dtklog.py)

    if(APPLE)
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS ${PROJECT_SOURCE_DIR}/dtkLog.i
        COMMAND ln -sf ../../libdtkLog.dylib _dtklog.so
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH})

      set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/_dtklog.so)
    endif(APPLE)

    if(UNIX AND NOT APPLE)
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS ${PROJECT_SOURCE_DIR}/dtkLog.i
        COMMAND ln -fs ../../libdtkLog.so _dtklog.so
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH})

      set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/_dtklog.so)
    endif(UNIX AND NOT APPLE)

  endif(DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND )

  if (DTK_WRAPPING_CSHARP)

    file(GLOB DTK_CSHARP_SOURCES "${${PROJECT_NAME}_BINARY_DIR}/*.cs")

    foreach(DTK_CSHARP_SOURCE ${DTK_CSHARP_SOURCES})
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS ${PROJECT_SOURCE_DIR}/dtkLog.i
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
    COMPONENT log)

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT log
         EXPORT dtkDepends)

## ###################################################################
## Install rules - modules
## ###################################################################

if(SWIG_FOUND)
  if (DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND)
    install(FILES ${${PROJECT_NAME}_MODULES} DESTINATION ${DTK_PYTHON_INSTALL_PATH})
    install(FILES ${CMAKE_BINARY_DIR}/dtkLog.i DESTINATION wrp/dtkLog)
  endif()
endif()

######################################################################
### CMakeLists.txt ends here
