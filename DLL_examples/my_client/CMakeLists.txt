set (SOURCE_CLIENT main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/my_lib)

add_executable(my_client ${SOURCE_CLIENT})
target_link_libraries(my_client my_lib)

install(TARGETS my_client DESTINATION bin)
