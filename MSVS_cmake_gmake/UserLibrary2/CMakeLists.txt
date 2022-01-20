set(SOURCES source/TheClass2.cpp)
set(HEADERS include/TheClass2.h include/import_export_spec2.h)
add_library(UserLibrary2 SHARED ${SOURCES} ${HEADERS})
target_include_directories(UserLibrary2 PUBLIC include)
