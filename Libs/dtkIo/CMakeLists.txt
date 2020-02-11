### CMakeLists.txt ---
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

## #################################################################
## Input
## #################################################################

project(dtkIo)

set(${PROJECT_NAME}_HEADERS
  dtkIo
  dtkIo.h
  dtkIoSettings
  dtkIoSettings.h
  dtkIoDataModel
  dtkIoDataModel.h
  PacketReceiver.h
  NetworkPacket.h
  NetworkSource.h
  PacketConsumer.h
  PacketFileWriter.h
  vtkLidarStream.h
  )
  #dtkIoDataModelHdf5.h)

set(${PROJECT_NAME}_SOURCES
  dtkIo.cpp
  dtkIoSettings.cpp
  dtkIoDataModel.cpp
  PacketReceiver.cpp
  NetworkPacket.cpp
  NetworkSource.cpp
  PacketConsumer.cpp
  PacketFileWriter.cpp
  vtkLidarStream.cpp
  vtkLidarProvider.cpp

  )
  #dtkIoDataModelHdf5.cpp)

# if(DTK_BUILD_DISTRIBUTED)
#   set(${PROJECT_NAME}_HEADERS 
#     ${${PROJECT_NAME}_HEADERS}
#     dtkIoDataModelPHdf5.h)
  
#   set(${PROJECT_NAME}_SOURCES
#     ${${PROJECT_NAME}_SOURCES}
#     dtkIoDataModelPHdf5.cpp)
# endif()

## #################################################################
## Build rules
## #################################################################

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS})

set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_RPATH 0)
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")

## ###################################################################
## Link rules
## ###################################################################

target_link_libraries(${PROJECT_NAME} Qt5::Core)

target_link_libraries(${PROJECT_NAME} dtkCore)
target_link_libraries(${PROJECT_NAME} dtkLog)

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})


# if(DTK_BUILD_DISTRIBUTED)
#   target_link_libraries(${PROJECT_NAME} dtkDistributed)
#   target_link_libraries(${PROJECT_NAME} ${MPI_LIBRARIES})
# endif(DTK_BUILD_DISTRIBUTED)

# target_link_libraries(${PROJECT_NAME} ${HDF5_LIBRARIES})

## #################################################################
## Export header file
## #################################################################

#add_compiler_export_flags()

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
  DESTINATION include/${PROJECT_NAME}
    COMPONENT io)

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT io
         EXPORT dtkIoDepends)

######################################################################
### CMakeLists.txt ends here
