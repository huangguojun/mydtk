find_package(Sphinx REQUIRED)

set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/Sphinx)
set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)

# Only regenerate Sphinx when:
#  - Our doc files have been updated
#  - The Sphinx config has been updated

add_custom_command(OUTPUT ${SPHINX_INDEX_FILE}
	COMMAND 
        ${SPHINX_EXECUTABLE} -b html
        #${SPHINX_EXECUTABLE} -b latex
		${SPHINX_SOURCE} ${SPHINX_BUILD}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	DEPENDS 
		# Other docs files you want to track should go here (or in some variable)
		${CMAKE_CURRENT_SOURCE_DIR}/index.rst
		${CMAKE_CURRENT_SOURCE_DIR}/*.rst
	MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
	COMMENT "Generating documentation with Sphinx")

# Nice named target so we can run the job easily
add_custom_target(Sphinx ALL DEPENDS ${SPHINX_INDEX_FILE})

#include(GNUInstallDirs)
#install(DIRECTORY ${SPHINX_BUILD} DESTINATION ${CMAKE_INSTALL_DOCDIR})
