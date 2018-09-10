set(SOURCE_LIB_BREP cls_circle.cpp
                    cls_planar_patch.cpp
                    cls_bezier_spline.cpp
                    cls_bezier_surface.cpp
                    cls_b_spline.cpp
                    cls_spline_base.cpp
#                    cls_bezier_spline_2.cpp
                    )

include_directories(${CMAKE_SOURCE_DIR})

add_library(brep SHARED ${SOURCE_LIB_BREP})

target_link_libraries(brep base algo)
