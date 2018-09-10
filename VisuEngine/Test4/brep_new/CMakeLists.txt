set(SOURCE_LIB_BREP cls_spline_base.cpp
                    cls_bezier_spline.cpp
                    cls_b_spline.cpp
                    )

include_directories(${CMAKE_SOURCE_DIR})

add_library(brep_new SHARED ${SOURCE_LIB_BREP})

target_link_libraries(brep_new base)
