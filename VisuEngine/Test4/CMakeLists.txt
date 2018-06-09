# ===============================================

cmake_minimum_required(VERSION 3.5)

# ===============================================

project(Test4)

# ===============================================
# External packages

find_package(OpenGL REQUIRED)
if (OpenGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIRS})
    link_libraries(${OPENGL_LIBRARIES})
endif()

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

find_package(GLUT REQUIRED)
if (GLUT_FOUND)
    include_directories(${GLUT_INCLUDE_DIRS})
    link_libraries(${GLUT_LIBRARIES})
endif()

find_package(PNG REQUIRED)
if (PNG_FOUND)
    include_directories(${PNG_INCLUDE_DIRS})
    link_libraries(${PNG_LIBRARIES})
endif()

# GLM

# ===============================================

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wpedantic -std=c++11")
endif()

# set(CMAKE_VERBOSE_MAKEFILE ON)

# ===============================================

set(SOURCE_EXE main.cpp)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(prog ${SOURCE_EXE})

# ===============================================
# Algorithms library

add_subdirectory(algo)

# ===============================================
# Base

add_subdirectory(base)

# ===============================================
# Graphics library

add_subdirectory(graphics)

# ===============================================
# STL interface library

add_subdirectory(stl_interface)

# ===============================================
# ANEU interface library

add_subdirectory(aneu_interface)

# ===============================================
# BREP library

add_subdirectory(brep)

# ===============================================

target_link_libraries(prog algo base graphics stl_interface aneu_interface brep)
