add_compile_definitions(MAKEDLL)

set(SOURCE_LIB my_lib.cpp)

add_library(my_lib SHARED ${SOURCE_LIB})

install(TARGETS my_lib DESTINATION bin)
