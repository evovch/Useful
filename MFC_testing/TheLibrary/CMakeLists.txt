set(SOURCES
	TheClass.cpp)

add_library(TheLibrary SHARED ${SOURCES})

install(TARGETS TheLibrary)
