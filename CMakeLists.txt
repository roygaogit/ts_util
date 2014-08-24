cmake_minimum_required (VERSION 2.8)
project (TsUtil)

FIND_PACKAGE( Boost 1.54 COMPONENTS program_options system filesystem REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

#dirs
set (BINARY_DIR "${CMAKE_SOURCE_DIR}/bin")
set (SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set (HEADER_DIR ${CMAKE_SOURCE_DIR}/include)
set (EXECUTABLE_OUTPUT_PATH "${BINARY_DIR}")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BINARY_DIR}")
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BINARY_DIR}")
#version
set (TsUtil_VERSION_MAJOR 0)
set (TsUtil_VERSION_MINOR 1)

#config file
configure_file (
    "${SOURCE_DIR}/config.h.in"
    "${HEADER_DIR}/config.h"
    )
    
include_directories("${HEADER_DIR}")
include_directories("${SOURCE_DIR}")

SET(SRC_FILES "${SOURCE_DIR}/ts_util.cpp")

# testing
#library
ADD_LIBRARY(ts_util "${SRC_FILES}")

install(FILES "${HEADER_DIR}/ts_util.hpp" DESTINATION include/ts_util)
install(TARGETS ts_util DESTINATION lib)


#link libraries

TARGET_LINK_LIBRARIES(ts_util ${Boost_LIBRARIES} )
