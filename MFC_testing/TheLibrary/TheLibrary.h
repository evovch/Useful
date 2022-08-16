#pragma once

#ifdef _MSC_VER

#ifdef TheLibrary_EXPORTS
#define LIBRARY_API _declspec(dllexport)
#else
#define LIBRARY_API _declspec(dllimport)
#endif

#else // not MSVS - assume GCC, incl. mingw

#ifdef TheLibrary_EXPORTS
#define LIBRARY_API __attribute__((visibility("default")))
#else
#define LIBRARY_API
#endif

#endif
