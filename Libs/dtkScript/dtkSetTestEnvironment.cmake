### dtkSetTestEnvironment.cmake.in ---
##
## Author: Julien Wintz
##
######################################################################

#
# TEST_INCLUDE_FILE for use with tests.
# Sets the PATH environment so that dlls are found.
#

# Choose a configuration with which to drive CTest tests.

set(PATHS_FOR_TESTS "$ENV{PATH}")
if(Linux STREQUAL Windows)
    list(APPEND PATHS_FOR_TESTS "/home/hgj/Project/dtk_pro/dtk/build/bin/RelWithDebInfo")
    list(APPEND PATHS_FOR_TESTS "/usr/lib/qt5/bin")
endif()

set(ENV{PATH} "${PATHS_FOR_TESTS}")
