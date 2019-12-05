#################################################
#
#
##################################################

cmake_minimum_required(VERSION 3.2.0)

## ###################################################################
## Version setup
## ###################################################################

set(dtk_VERSION_MAJOR 1)
set(dtk_VERSION_MINOR 7)
set(dtk_VERSION_PATCH 1)
set(dtk_VERSION ${dtk_VERSION_MAJOR}.${dtk_VERSION_MINOR}.${dtk_VERSION_PATCH})


## ###################################################################
## Build setup
## ###################################################################

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(GenerateExportHeader)

if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
endif(UNIX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_BUILD_TYPE DEBUG)


find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets OpenGL Xml )
set(QT_LIBRARIES Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network Qt5::OpenGL Qt5::Xml)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) 
set(CMAKE_AUTORCC ON) 

find_package(KF5ThreadWeaver REQUIRED)

find_package(DCMTK)
find_package(Boost)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})


find_package(CUDA REQUIRED)
set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} -D_FORCE_INLINES  )

### 解决VTK CUDA 冲突问题
get_directory_property(dir_defs DIRECTORY ${CMAKE_SOURCE_DIR} COMPILE_DEFINITIONS)
set(vtk_flags)
foreach(it ${dir_defs})
    if(it MATCHES "vtk*")
    list(APPEND vtk_flags ${it})
    endif()
endforeach()

foreach(d ${vtk_flags})
    remove_definitions(-D${d})
endforeach()
###

## #################################################################
## Options
## #################################################################
option(DTK_BUILD_SCRIPT              "Build script layer"     ON)
option(DTK_BUILD_WIDGETS             "Build widgets"          ON)
option(DTK_BUILD_SUPPORT_CORE        "Build support layer"    ON)
option(DTK_BUILD_SUPPORT_GUI         "Build support layer"    ON)
option(DTK_BUILD_SUPPORT_MATH        "Build support layer"    ON)
option(DTK_BUILD_SUPPORT_PLOT        "Build support layer"    ON)

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
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Libs/dtkCoreSupport)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Libs/dtkMathSupport)

include_directories(${CUDA_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${VTK_INCLUDE_DIRS})
include_directories(/usr/local/cuda/lib64)


########################################
## add subdirectory
#######################################
add_subdirectory(App)
add_subdirectory(Libs)

