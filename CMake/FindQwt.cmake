### FindQwt.cmake --- 

# QWT_FOUND - system has Qwt
# QWT_INCLUDE_DIR - where to find qwt.h
# QWT_LIBRARIES - the libraries to link against to use Qwt
# QWT_LIBRARY - where to find the Qwt library (not for general use)

set(QWT_FOUND "NO")

set(QWT_NAMES ${QWT_NAMES} qwt libqwt qwt-qt5)

if(QWT_DIR)
  find_path(QWT_INCLUDE_DIR
    NAMES qwt.h
    PATHS ${QWT_DIR}/src ${QWT_DIR}/include
    NO_DEFAULT_PATH )

  find_library(QWT_LIBRARY
    NAMES ${QWT_NAMES}
    PATHS ${QWT_DIR}/lib ${QWT_DIR}/lib64 NO_DEFAULT_PATH)

else(QWT_DIR)

  find_path(QWT_INCLUDE_DIR
    NAMES qwt.h
    PATHS $ENV{QWT_DIR}/src $ENV{QWT_DIR}/include $ENV{QWT_DIR}/qwt
    NO_DEFAULT_PATH)

  find_library(QWT_LIBRARY
    NAMES ${QWT_NAMES}
    PATHS $ENV{QWT_DIR}/lib $ENV{QWT_DIR}/lib64 NO_DEFAULT_PATH)

endif(QWT_DIR)



find_path(QWT_INCLUDE_DIR
  NAMES qwt.h
  PATHS
  /usr/local/qwt/include
  /usr/local/include
  /opt/qwt/include
  /opt/qwt-qt5/include
  /usr/include/qwt
  /usr/include/qt5/qwt
  /usr/include)

find_library(QWT_LIBRARY
  NAMES ${QWT_NAMES}
  PATHS /opt/qwt/lib /opt/qwt-qt5/lib /usr/local/qwt/lib /usr/local/lib /usr/lib)

if(QWT_INCLUDE_DIR AND QWT_LIBRARY)
  set(QWT_LIBRARIES ${QWT_LIBRARY})
  set(QWT_FOUND "YES")
  if (CYGWIN)
    if(NOT BUILD_SHARED_LIBS)
      set(QWT_DEFINITIONS -DQWT_STATIC)
    endif(NOT BUILD_SHARED_LIBS)
  endif(CYGWIN)
endif(QWT_INCLUDE_DIR AND QWT_LIBRARY)

mark_as_advanced(QWT_INCLUDE_DIR QWT_LIBRARY QWT_DIR)
