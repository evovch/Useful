#pragma once

#ifdef MAKEDLL
#define MYPROJECTAPI __declspec(dllexport)
#else
#define MYPROJECTAPI __declspec(dllimport)
#endif
