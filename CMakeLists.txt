#################################################
#
#
##################################################

cmake_minimum_required(VERSION 3.2.0)
######################################################################

project(mydtk)

## ###################################################################
## Version setup
## ###################################################################

set(dtk_VERSION_MAJOR 1)
set(dtk_VERSION_MINOR 7)
set(dtk_VERSION_PATCH 1)
set(dtk_VERSION ${dtk_VERSION_MAJOR}.${dtk_VERSION_MINOR}.${dtk_VERSION_PATCH})

## ###################################################################
## Path setup
## ###################################################################

include(GNUInstallDirs)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

## #################################################################
## Install prefix
## #################################################################

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install" CACHE PATH "${PROJECT_NAME} install prefix" FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

## ###################################################################
## Build setup
## ###################################################################

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()


set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(GenerateExportHeader)

if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
endif(UNIX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets OpenGL Xml Multimedia MultimediaWidgets)
#set(QT_LIBRARIES Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network Qt5::OpenGL Qt5::Xml Qt5::Multimedia Qt5::MultimediaWidgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) 
set(CMAKE_AUTORCC ON) 

find_package(KF5ThreadWeaver REQUIRED)

find_package(DCMTK)

find_package(Boost REQUIRED COMPONENTS  system filesystem thread)
include_directories( ${Boost_INCLUDE_DIR} )
link_directories( ${Boost_LIBRARY_DIRS} )

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})


#find_package(CUDA REQUIRED)
#set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} -D_FORCE_INLINES  )

### 解决VTK CUDA 冲突问题
#get_directory_property(dir_defs DIRECTORY ${CMAKE_SOURCE_DIR} COMPILE_DEFINITIONS)
#set(vtk_flags)
#foreach(it ${dir_defs})
#    if(it MATCHES "vtk*")
#    list(APPEND vtk_flags ${it})
#    endif()
#endforeach()

#foreach(d ${vtk_flags})
#    remove_definitions(-D${d})
#endforeach()

## ###################################################################
##
## ###################################################################

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})
#include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/dtkWrap.cmake)
#include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/FindQwt.cmake)
#include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/FindSphinx.cmake)


## #################################################################
## Options
## #################################################################
option(DTK_BUILD_SCRIPT              "Build script layer"     ON)
option(DTK_BUILD_WIDGETS             "Build widgets"          ON)
option(DTK_BUILD_SUPPORT_CORE        "Build support layer"    ON)
option(DTK_BUILD_SUPPORT_GUI         "Build support layer"    ON)
option(DTK_BUILD_SUPPORT_MATH        "Build support layer"    ON)
option(DTK_BUILD_SUPPORT_PLOT        "Build support layer"    ON)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/CMake/dtkConfig.h.in"
  "${PROJECT_BINARY_DIR}/dtkConfig.h")

## #################################################################
## Dependencies - internal
## #################################################################

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Libs)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Libs/dtkCore)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Libs/dtkGui)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Libs/dtkLog)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Libs/dtkMath)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Libs/dtkMeta)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Libs/dtkScript)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Libs/dtkTest)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Libs/dtkWidgets)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Libs/dtkFonts)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Libs/dtkThemes)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Libs/dtkCoreSupport)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Libs/dtkMathSupport)

include_directories(${CUDA_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${VTK_INCLUDE_DIRS})
include_directories(/usr/local/cuda/lib64)

## ###################################################################
## Exporting
## ###################################################################

#export(PACKAGE dtk)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CMake/dtkConfig.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/dtkConfig.cmake" @ONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CMake/dtkConfig.install.cmake.in
 "${PROJECT_BINARY_DIR}/install/dtkConfig.cmake" @ONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CMake/dtkConfigVersion.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/dtkConfigVersion.cmake" @ONLY)



########################################
## add subdirectory
#######################################
add_subdirectory(App)
add_subdirectory(Libs)
#add_subdirectory(Test)
add_subdirectory(Doc)

########################################
## Install
#######################################
#set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/Install)

#install(FILES Script/nvCT.desktop DESTINATION /usr/share/applications)
#install(FILES Script/nvCT.png DESTINATION /usr/share/pixmaps)

########################################
## Package
#######################################
include (InstallRequiredSystemLibraries)
#set (CPACK_RESOURCE_FILE_LICENSE   "${CMAKE_CURRENT_SOURCE_DIR}/RELEASE_NOTES.txt")
#set (CMAKE_INSTALL_LIBDIR lib)
set (CPACK_GENERATOR "DEB")
set (CPACK_PACKAGE_VERSION_MAJOR "${dtk_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${dtk_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "${dtk_VERSION_PATCH}")
include (CPack)

########################################
## CTest 
#######################################
#enable_testing()
#add_test(NAME mydtk COMMAND $<TARGET_FILE:mytest>)
