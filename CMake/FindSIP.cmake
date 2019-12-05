# Find SIP
# ~~~~~~~~
#
# SIP website: http://www.riverbankcomputing.co.uk/sip/index.php
#
# Find the installed version of SIP. FindSIP should be called after Python
# has been found.
#
# This file defines the following variables:
#
# SIP_VERSION - The version of SIP found expressed as a 6 digit hex number
#     suitable for comparison as a string.
#
# SIP_VERSION_STR - The version of SIP found as a human readable string.
#
# SIP_EXECUTABLE - Path and filename of the SIP command line executable.
#
# SIP_INCLUDE_DIR - Directory holding the SIP C++ header file.
#
# SIP_DEFAULT_SIP_DIR - Default directory where .sip files should be installed
#     into.

# Copyright (c) 2007, Simon Edwards <simon@simonzone.com>
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

# IF(SIP_VERSION)
#   # Already in cache, be silent
#   message("SIP already found")
#   SET(SIP_FOUND TRUE)
# ELSE(SIP_VERSION)

find_file(_find_sip_py FindSIP.py PATHS ${CMAKE_MODULE_PATH})

execute_process(COMMAND ${PYTHON_EXECUTABLE} ${_find_sip_py} OUTPUT_VARIABLE sip_config)

if(sip_config)
  string(REGEX REPLACE "^sip_version:([^\n]+).*$" "\\1" SIP_VERSION_output ${sip_config})
  set(SIP_VERSION ${SIP_VERSION_output} CACHE STRING "Sip version" FORCE)
  string(REGEX REPLACE ".*\nsip_version_str:([^\n]+).*$" "\\1" SIP_VERSION_STR ${sip_config})
  string(REGEX REPLACE ".*\nsip_bin:([^\n]+).*$" "\\1" SIP_EXECUTABLE_output ${sip_config})
  set(SIP_EXECUTABLE ${SIP_EXECUTABLE_output} CACHE STRING "Sip executable" FORCE)
  if(NOT SIP_DEFAULT_SIP_DIR)
    string(REGEX REPLACE ".*\ndefault_sip_dir:([^\n]+).*$" "\\1" SIP_DEFAULT_SIP_DIR ${sip_config})
    set(SIP_MODULE_DIR ${SIP_DEFAULT_SIP_DIR} CACHE STRING "Sip default dir" FORCE)
    set(PYQT5_MODULE_DIR "${SIP_DEFAULT_SIP_DIR}/PyQt5" CACHE STRING "PyQt5 module dir" FORCE)
  endif(NOT SIP_DEFAULT_SIP_DIR)
  string(REGEX REPLACE ".*\nsip_inc_dir:([^\n]+).*$" "\\1" SIP_INCLUDE_DIR ${sip_config})
  file(TO_CMAKE_PATH ${SIP_DEFAULT_SIP_DIR} SIP_DEFAULT_SIP_DIR)
  file(TO_CMAKE_PATH ${SIP_INCLUDE_DIR} SIP_INCLUDE_DIR)
  if(EXISTS ${SIP_EXECUTABLE})
    set(SIP_FOUND TRUE)
    set(SIP_VERSION)
  else()
    message(STATUS "Found SIP configuration but the sip executable could not be found.")
  endif()
else(sip_config)
  message(STATUS "Found SIP configuration but the sip executable did not output anything.")
endif(sip_config)

if(SIP_FOUND)
  if(NOT SIP_FIND_QUIETLY)
    message(STATUS "Found SIP version: ${SIP_VERSION_STR}")
  endif(NOT SIP_FIND_QUIETLY)
else(SIP_FOUND)
  if(SIP_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find SIP")
  endif(SIP_FIND_REQUIRED)
endif(SIP_FOUND)

# ENDIF(SIP_VERSION)
