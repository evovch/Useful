# Already not necessary, but you can play around with this:
# Currently not specified in CMakeLists.txt - need to set after MSVS project generation by CMake:
# Only for this project (MFC_testing): "Use MFC in a shared DLL" and "Use Unicode Character Set"

set(SOURCES
	main.cpp)

include_directories(${PROJECT_SOURCE_DIR}/TheLibrary)

add_executable(MFC_testing ${SOURCES})

if(UNIX)
target_link_libraries(MFC_testing dl)
endif()

install(TARGETS MFC_testing)
