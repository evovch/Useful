set(SOURCE_LIB_BREP cls_circle.cpp)

add_library(brep SHARED ${SOURCE_LIB_BREP})

include_directories(${CMAKE_SOURCE_DIR})
