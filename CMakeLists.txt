cmake_minimum_required (VERSION 2.8)
project (TsUtil)

FIND_PACKAGE( Boost 1.54 COMPONENTS program_options system filesystem  REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

#dirs
set (SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set (HEADER_DIR ${CMAKE_SOURCE_DIR}/include)
#version
set (TsUtil_VERSION_MAJOR 0)
set (TsUtil_VERSION_MINOR 1)

#config file
configure_file (
    "${SOURCE_DIR}/config.h.in"
    "${HEADER_DIR}/config.h"
    )
    
include_directories("${HEADER_DIR}")


#Source files
SET(TS_UTIL_SRC_FILES "${SOURCE_DIR}/ts_util.cpp")
SET(TS_STREAMER_SRC_FILES "${SOURCE_DIR}/ts_rtp_streamer.cpp")
#Executables


#library
ADD_LIBRARY(ts_util "${TS_UTIL_SRC_FILES}")
ADD_LIBRARY(ts_rtp_streamer "${TS_STREAMER_SRC_FILES}")

install(FILES "${HEADER_DIR}/ts_util.hpp" DESTINATION include/ts_util)
install(FILES "${HEADER_DIR}/ts_rtp_streamer.hpp" DESTINATION include/ts_util)
install(TARGETS ts_util ts_rtp_streamer DESTINATION lib)


#link libraries

TARGET_LINK_LIBRARIES(ts_util ${Boost_LIBRARIES} )
TARGET_LINK_LIBRARIES(ts_rtp_streamer ${Boost_LIBRARIES})