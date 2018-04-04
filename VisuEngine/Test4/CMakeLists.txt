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

# ===============================================

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
endif()

# ===============================================

set(SOURCE_EXE main.cpp)

add_executable(prog ${SOURCE_EXE})

# ===============================================
# Graphics library

set(SOURCE_LIB cls_camera.cpp
               cls_model.cpp
               cls_renderer.cpp
               cls_scene.cpp
               support.cpp)

add_library(graphics STATIC ${SOURCE_LIB})

# ===============================================
# STL interface library

add_subdirectory(stl_interface)



# ===============================================

target_link_libraries(prog graphics stl_interface)
