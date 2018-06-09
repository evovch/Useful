set(SOURCE_LIB_ALGO cls_quick_hull.cpp
                    )

include_directories(${CMAKE_SOURCE_DIR})

add_library(algo SHARED ${SOURCE_LIB_ALGO})
