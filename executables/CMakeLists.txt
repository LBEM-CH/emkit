
cmake_minimum_required(VERSION 2.6)

#==============================#
# Set Environment              #
#==============================#

# Executables
file(GLOB EXEC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
set(RUNNER_SOURCES ${EXEC_SOURCES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")


#==============================#
# Include Directories          #
#==============================#
include_directories(${CMAKE_SOURCE_DIR}/include)
SET(TCLAP_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/tclap)
include_directories(${TCLAP_INCLUDE_DIR})

#=============================#
# Create Executables          # 
#=============================#
# For Executables
foreach(i ${RUNNER_SOURCES})
    get_filename_component(EXECUTABLE ${i}  NAME_WE)
    
    add_executable(${EXECUTABLE} ${i})
    
    #Link to the library created in step above
    target_link_libraries(${EXECUTABLE} LINK_PUBLIC emkit)
    target_link_libraries(${EXECUTABLE} /usr/local/cina/fftw/3.3.4/lib/libfftw3.a)

    list(APPEND EXECUTABLES_VP ${EXECUTABLE})
endforeach(i ${RUNNER_SOURCES})

#==============================#
# Installation                 #
#==============================#
get_filename_component(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} NAME)
if(INSTALL_BIN_DIR)
	set(INSTALL_EXEC_DIR ${INSTALL_BIN_DIR})
else(INSTALL_BIN_DIR)
	set(INSTALL_EXEC_DIR .)
endif(INSTALL_BIN_DIR)

install(TARGETS ${EXECUTABLES_VP} DESTINATION ${INSTALL_EXEC_DIR})