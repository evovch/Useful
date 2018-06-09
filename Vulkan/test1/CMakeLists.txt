# ===============================================

cmake_minimum_required(VERSION 3.5)

# ===============================================

# Give your project a name
project(Vulkan_test1)

# Switch in on for more verbose make output
# set(CMAKE_VERBOSE_MAKEFILE ON)

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -Wextra -Wpedantic")
    # -std=c++11 -g -Wall -Wextra -Wpedantic
endif()

# ===============================================

# TODO check VULKAN_SDK env. variable

include_directories($ENV{VULKAN_SDK}/include)
link_directories($ENV{VULKAN_SDK}/lib)

# ===============================================

set(SOURCE_EXE main.cpp
               cls_VulkanRenderer.cpp
               )

add_executable(prog ${SOURCE_EXE})

# ===============================================

target_link_libraries(prog vulkan)

# ===============================================
