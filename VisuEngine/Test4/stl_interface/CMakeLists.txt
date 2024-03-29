set(SOURCE_LIB_STL_INTERFACE stl_parsing.flex.cpp
                             stl_parsing.tab.cpp
                             cls_stl_file.cpp
                             cls_stl_interface.cpp)

# CMAKE_CURRENT_SOURCE_DIR is required here because flex/bison are stupid
include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_SOURCE_DIR})

add_custom_command(
    OUTPUT stl_parsing.flex.cpp
    COMMAND flex
    ARGS -o stl_parsing.flex.cpp
         ${CMAKE_CURRENT_SOURCE_DIR}/stl_parsing.l
    VERBATIM
    )

add_custom_command(
    OUTPUT stl_parsing.tab.cpp stl_parsing.tab.h
    COMMAND bison
    ARGS --defines=stl_parsing.tab.h
         -o stl_parsing.tab.cpp
         ${CMAKE_CURRENT_SOURCE_DIR}/stl_parsing.y
    VERBATIM
    )

add_library(stl_interface SHARED ${SOURCE_LIB_STL_INTERFACE})

target_link_libraries(stl_interface base)
