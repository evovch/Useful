# ===============================================

cmake_minimum_required(VERSION 3.5)

# ===============================================

project(Polynom)

# ===============================================

# ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED)

# Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

# ===============================================

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wpedantic -std=c++11")
endif()

# ===============================================

set(SOURCE_EXE main.cpp)

add_executable(polynom ${SOURCE_EXE})

target_link_libraries(polynom Core Hist Gpad)
