cmake_minimum_required(VERSION 3.5)

project(ROOT_streaming2)

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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wpedantic")
endif()

# =============================================================================

set(ROOTCLING_HEADERS myClass1.h
                      myClass2.h
                      )

ROOT_GENERATE_DICTIONARY(G__UserClasses ${ROOTCLING_HEADERS} LINKDEF LinkDef.h)

# =============================================================================
# Library

set(SOURCE_LIB G__UserClasses.cxx
               myClass1.cxx
               myClass2.cxx
               )

add_library(UserClasses SHARED ${SOURCE_LIB})

# =============================================================================
# Executable

set(SOURCE_EXE main.cxx
               )

add_executable(prog ${SOURCE_EXE})

target_link_libraries(prog UserClasses Core RIO)
