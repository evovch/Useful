#pragma once

#ifdef UserLibrary2_EXPORTS
	#define MYDLLAPI __declspec(dllexport)
#else
	#define MYDLLAPI __declspec(dllimport)
#endif
