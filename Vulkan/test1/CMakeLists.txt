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

set (GLFWDIR "C:\\Users\\evovch\\source\\vendor\\glfw-3.3.2.bin.WIN64\\")

include_directories(${GLFWDIR}/include)
link_directories(${GLFWDIR}/lib-vc2019)

# ===============================================

set(SOURCE_EXE main.cpp
               vulkan_renderer.cpp
               )

add_executable(prog ${SOURCE_EXE})

# ===============================================

target_link_libraries(prog vulkan-1 glfw3)

# ===============================================

#TODO compile shaders?

# ===============================================
