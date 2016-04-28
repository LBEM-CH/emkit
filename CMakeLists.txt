############################################################################
# CMakeLists.txt for kernel/toolkit                                        #
############################################################################
# Author:                                                                  #
# Nikhil Biyani  03/2016                                                   #
############################################################################
#
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

#==============================#
# Set Environment              #
#==============================#
file(GLOB_RECURSE CPP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)

set(PROJECT_SOURCES ${CPP_SOURCES})
set(PROJECT_HEADERS ${HPP_HEADERS})

include_directories("/usr/local/cina/fftw/3.3.4/include")
include_directories("${CMAKE_SOURCE_DIR}/lib")

message("${CMAKE_SOURCE_DIR}/lib/mrcfile")

#==============================#
# Add library		           #
#==============================#
add_library(emkit ${PROJECT_SOURCES} ${PROJECT_HEADERS})

#add_subdirectory(executables)
