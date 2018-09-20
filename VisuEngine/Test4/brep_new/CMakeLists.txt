set(SOURCE_LIB_BREP B_spline_basis.hpp
                    cls_spline_base.cpp
                    cls_bezier_spline.cpp
                    cls_b_spline_curve.cpp
                    cls_surface_base.cpp
                    cls_bezier_surface.cpp
                    cls_b_spline_surface.cpp
                    )

include_directories(${CMAKE_SOURCE_DIR})

add_library(brep_new SHARED ${SOURCE_LIB_BREP})

target_link_libraries(brep_new base)
