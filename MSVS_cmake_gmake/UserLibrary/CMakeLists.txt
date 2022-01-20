set(SOURCES source/TheClass.cpp)
set(HEADERS include/TheClass.h include/import_export_spec.h)
add_library(UserLibrary SHARED ${SOURCES} ${HEADERS})
target_include_directories(UserLibrary PUBLIC include)
