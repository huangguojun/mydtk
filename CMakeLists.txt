#################################################
#
#
##################################################

cmake_minimum_required(VERSION 3.5)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_C_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "-Wall -std=c++11")


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

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/Libs
  ${CMAKE_CURRENT_SOURCE_DIR}/Libs/dtkLog
  ${CMAKE_CURRENT_SOURCE_DIR}/Libs/dtkCore
  ${CMAKE_CURRENT_SOURCE_DIR}/Libs/dtkMeta
  ${CMAKE_CURRENT_SOURCE_DIR}/Libs/dtkWidgets
  ${CUDA_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${VTK_INCLUDE_DIRS}
  /usr/local/cuda/lib64
)

########################################
## add subdirectory
#######################################
add_subdirectory(App)
add_subdirectory(Libs)

