set(SOURCE_LIB_BREP cls_circle.cpp
                    cls_planar_patch.cpp)

include_directories(${CMAKE_SOURCE_DIR})

add_library(brep SHARED ${SOURCE_LIB_BREP})

target_link_libraries(brep algo)
