set(SOURCE_LIB_STL_PARSING stl_parsing.flex.cpp stl_parsing.tab.cpp)

add_library(stl_parsing STATIC ${SOURCE_LIB_STL_PARSING})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

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
