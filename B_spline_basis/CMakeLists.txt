# =============================================================================

cmake_minimum_required(VERSION 3.5)

# Give your project a name
project(B_spline_basis)

# Switch it on for more verbose make output
# set(CMAKE_VERBOSE_MAKEFILE ON)

# =============================================================================
# External packages

# See: https://root.cern.ch/how/integrate-root-my-project-cmake

# ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED)

# Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

# =============================================================================

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -Wextra -Wpedantic")
endif()

# =============================================================================

set (SOURCES_EXE main.cpp
                 )

add_executable(prog ${SOURCES_EXE})

target_link_libraries(prog Core Hist Graf Gpad)
