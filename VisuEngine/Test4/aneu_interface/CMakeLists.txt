set(SOURCE_LIB_ANEU_INTERFACE cls_aneu_file.cpp
                             cls_aneu_interface.cpp)

include_directories(${CMAKE_SOURCE_DIR})

add_library(aneu_interface SHARED ${SOURCE_LIB_ANEU_INTERFACE})

target_link_libraries(aneu_interface base)
