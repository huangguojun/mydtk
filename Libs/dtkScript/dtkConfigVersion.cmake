### dtkConfigVersion.cmake.in --- 
## 
## Author: Julien Wintz
## Created: Tue Feb  5 13:10:26 2013 (+0100)
## Version: 
## Last-Updated: Tue Feb  5 13:11:24 2013 (+0100)
##           By: Julien Wintz
##     Update #: 3
######################################################################
## 
### Change Log:
## 
######################################################################

set(PACKAGE_VERSION "1.7.1")

if("${PACKAGE_VERSION}" VERSION_LESS "${PACKAGE_FIND_VERSION}")
  set(PACKAGE_VERSION_COMPATIBLE FALSE)
else()
  set(PACKAGE_VERSION_COMPATIBLE TRUE)
  if ("${PACKAGE_VERSION}" VERSION_EQUAL "${PACKAGE_FIND_VERSION}")
    set(PACKAGE_VERSION_EXACT TRUE)
  endif()
endif()
