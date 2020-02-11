### CMakeLists.txt ---

project(dtkMathSupport)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  dtkMath.h
  dtkMath.tpp
  dtkMatrix.h
  dtkMatrix.tpp
  dtkMatrixSquared.h
  dtkMatrixSquared.tpp
  dtkQuaternion.h
  dtkQuaternion.tpp
  dtkVector.h
  dtkVector.tpp
  dtkVector3D.h
  dtkVector3D.tpp)

set(${PROJECT_NAME}_SOURCES
  dtkMath.cpp
  dtkMatrix.cpp)

## #################################################################
## Build rules
## #################################################################

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS})

target_link_libraries(${PROJECT_NAME}
  dtkLog)

## ###################################################################
## Link rules
## ###################################################################

target_link_libraries(${PROJECT_NAME} Qt5::Core)

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

#install(TARGETS ${PROJECT_NAME}
#    DESTINATION ${CMAKE_INSTALL_LIBDIR}
#      COMPONENT mathsupport
#         EXPORT dtkDepends)
