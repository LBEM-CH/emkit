############################################################################
# CMakeLists.txt for kernel/toolkit                                        #
############################################################################
# Author:                                                                  #
# Nikhil Biyani  03/2016                                                   #
############################################################################
#
cmake_minimum_required(VERSION 2.8)
#
#==============================#
# ADD CMAKE MODULES            #
#==============================#
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/share/cmake ${CMAKE_MODULE_PATH})
#

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -lpthread")

#==============================#
# Set Environment              #
#==============================#
file(GLOB_RECURSE CPP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)

set(PROJECT_SOURCES ${CPP_SOURCES})
set(PROJECT_HEADERS ${HPP_HEADERS})

#==============================#
# External Libraries           #
#==============================#
set(USE_FFTWD TRUE)
find_package(FFTW REQUIRED)
if(FFTWD_FOUND)
        message("FFTW_LIBS found: ${FFTWD_LIBS}")
	list(APPEND PER_LIBRARIES  ${FFTWD_LIBS})
	include_directories(${FFTW_INCLUDE_PATH})
        message("FFTW_INCLUDE_PATH is: ${FFTW_INCLUDE_PATH}")
elseif(FFTWF_FOUND)
	message(FATAL_ERROR "FFTW not found!")
endif(FFTWF_FOUND)

include_directories("${CMAKE_SOURCE_DIR}/external")

#==============================#
# Add library		           #
#==============================#
add_library(emkit ${PROJECT_SOURCES} ${PROJECT_HEADERS})

add_subdirectory(executables)
