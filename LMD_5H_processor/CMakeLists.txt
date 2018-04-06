# ===============================================

cmake_minimum_required(VERSION 3.5)

# ===============================================

project(LMD_5H_processor)

# ===============================================

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLinux")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLinux")
endif()

set(CMAKE_VERBOSE_MAKEFILE ON)

# ===============================================

set(SOURCE_EXE main.c)

add_executable(prog ${SOURCE_EXE})

# ===============================================
# Go4Analysis library

SET(GO4DIR /home/evovch/soft/go4-5.3.0)

include_directories(${GO4DIR}/include)

add_library(Go4Analysis SHARED IMPORTED)
set_target_properties(Go4Analysis PROPERTIES IMPORTED_LOCATION ${GO4DIR}/lib/libGo4Analysis.so)

# ===============================================

target_link_libraries(prog Go4Analysis)
