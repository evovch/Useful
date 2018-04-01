cmake_minimum_required(VERSION 3.5)

project(Test4)

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

set(SOURCE_EXE main.cpp)

set(SOURCE_LIB cls_camera.cpp
               cls_model.cpp
               cls_renderer.cpp
               cls_scene.cpp
               support.cpp)

set(SOURCE_LIB_STL cls_stl_file.cpp)

add_library(graphics STATIC ${SOURCE_LIB})
add_library(stl STATIC ${SOURCE_LIB_STL})
add_executable(prog ${SOURCE_EXE})

target_link_libraries(prog graphics stl)
