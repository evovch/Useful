#pragma once

#ifdef UserLibrary_EXPORTS
	#define MYDLLAPI __declspec(dllexport)
#else
	#define MYDLLAPI __declspec(dllimport)
#endif
