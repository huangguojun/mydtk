project(app1)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  ccMainWindow.h)

set(${PROJECT_NAME}_HEADERS_MOC
  ccMainWindow.h)

set(${PROJECT_NAME}_SOURCES
  ccMainWindow.cpp
  main.cpp)

## #################################################################
## Build rules
## #################################################################

qt5_wrap_cpp(${PROJECT_NAME}_SOURCES_MOC ${${PROJECT_NAME}_HEADERS_MOC})

add_executable(${PROJECT_NAME} MACOSX_BUNDLE
  ${${PROJECT_NAME}_HEADERS}
  ${${PROJECT_NAME}_SOURCES_MOC}
  ${${PROJECT_NAME}_SOURCES})

target_link_libraries(${PROJECT_NAME}
  ${QT_LIBRARIES}
  ${VTK_LIBRARIES}
  ${Boost_LIBRARIES}
  dtkLog
  dtkCore
  dtkGui
  dtkWidgets
  )
target_link_libraries(${PROJECT_NAME} dtkVisualization)

## #################################################################
## Installation
## #################################################################

install(TARGETS ${PROJECT_NAME}
   BUNDLE DESTINATION bin
   RUNTIME DESTINATION bin)
