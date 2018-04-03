add_custom_command(
    OUTPUT stl_parsing.flex.cpp
    COMMAND flex
    ARGS -o stl_parsing.flex.cpp
         stl_parsing.l
    )

add_custom_command(
    OUTPUT stl_parsing.tab.cpp stl_parsing.tab.h
    COMMAND bison
    ARGS --defines=stl_parsing.tab.h
         -o stl_parsing.tab.cpp
         stl_parsing.y
    )
