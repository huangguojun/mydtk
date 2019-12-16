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

project(dtkFonts VERSION ${dtkFonts_VERSION})

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  dtkFonts
  dtkFontAllTheIcons
  dtkFontAllTheIcons.h
  dtkFontAwesome
  dtkFontAwesome.h
  dtkFontFileIcons
  dtkFontFileIcons.h
  dtkFontOcticon
  dtkFontOcticon.h
  dtkFontSourceCodePro
  dtkFontSourceCodePro.h
  dtkFontSourceSansPro
  dtkFontSourceSansPro.h)

set(${PROJECT_NAME}_SOURCES
  dtkFontAllTheIcons.cpp
  dtkFontAwesome.cpp
  dtkFontFileIcons.cpp
  dtkFontOcticon.cpp
  dtkFontSourceCodePro.cpp
  dtkFontSourceSansPro.cpp)

set(${PROJECT_NAME}_SOURCES_RCC
  dtkFontAllTheIcons.qrc
  dtkFontAwesome.qrc
  dtkFontFileIcons.qrc
  dtkFontOcticon.qrc
  dtkFontSourceCodePro.qrc
  dtkFontSourceSansPro.qrc)

set_property(SOURCE qrc_dtkFontAllTheIcons.cpp   PROPERTY SKIP_AUTOMOC ON)
set_property(SOURCE qrc_dtkFontAwesome.cpp       PROPERTY SKIP_AUTOMOC ON)
set_property(SOURCE qrc_dtkFontFileIcons.cpp     PROPERTY SKIP_AUTOMOC ON)
set_property(SOURCE qrc_dtkFontOcticon.cpp       PROPERTY SKIP_AUTOMOC ON)
set_property(SOURCE qrc_dtkFontSourceCodePro.cpp PROPERTY SKIP_AUTOMOC ON)
set_property(SOURCE qrc_dtkFontSourceSansPro.cpp PROPERTY SKIP_AUTOMOC ON)

## #################################################################
## Build rules
## #################################################################

qt5_add_resources(${PROJECT_NAME}_SOURCES_QRC ${${PROJECT_NAME}_SOURCES_RCC})

if(NOT IOS)
add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES_QRC}
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS})
else(NOT IOS)
add_library(${PROJECT_NAME} STATIC
  ${${PROJECT_NAME}_SOURCES_QRC}
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS})
endif(NOT IOS)

target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Gui)

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${layer_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
  $<INSTALL_INTERFACE:include>)

if(IOS)
  target_link_libraries(${PROJECT_NAME} PRIVATE "-framework CoreText")
  target_link_libraries(${PROJECT_NAME} PRIVATE "-framework CoreGraphics")
  target_link_libraries(${PROJECT_NAME} PRIVATE "-framework CoreServices")
  target_link_libraries(${PROJECT_NAME} PRIVATE "-framework UIKit")

  target_link_libraries(${PROJECT_NAME} PRIVATE qtlibpng)
  target_link_libraries(${PROJECT_NAME} PRIVATE qtharfbuzz)
  target_link_libraries(${PROJECT_NAME} PRIVATE qtpcre2)
  target_link_libraries(${PROJECT_NAME} PRIVATE z)
endif(IOS)

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
