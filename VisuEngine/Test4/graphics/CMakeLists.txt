set(SOURCE_LIB_GRAPHICS cls_camera.cpp
                        cls_model.cpp
                        cls_renderer.cpp
                        cls_offscreen_renderer.cpp
                        cls_scene.cpp)

include_directories(${CMAKE_SOURCE_DIR})

add_library(graphics SHARED ${SOURCE_LIB_GRAPHICS})
