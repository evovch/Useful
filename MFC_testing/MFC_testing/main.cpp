//#define MFC_PRESENT

#define _CRT_SECURE_NO_WARNINGS

#ifdef MFC_PRESENT

#include <afx.h>

#endif // MFC_PRESENT

#ifdef _WIN32

#else
#include <dlfcn.h>
#endif

#include <cstdio>
#include <cstdlib>

#include "TheClass.h"

#ifdef MFC_PRESENT

int CreateArchive(const char* const filename)
{
	CFile file1;

	const BOOL file1_opened = file1.Open(filename, CFile::modeCreate | CFile::modeWrite);
	if (!file1_opened) return 1; // error

	CArchive archive1(&file1, CArchive::store);

	// dataset 1

	constexpr UINT N = 10;
	constexpr float data1[N] = { 0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f };
	constexpr UINT data1_size = N * sizeof(float);

	archive1.Write(data1, data1_size);

	// dataset 2

	constexpr UINT M = 9;
	constexpr unsigned char data2[M] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	constexpr UINT data2_size = M * sizeof(unsigned char);

	archive1.Write(data2, data2_size);

	return 0; // ok
}

int PrintArchiveRaw(const char* const filename)
{
	FILE* file1 = nullptr;
	fopen_s(&file1, filename, "r");
	if (!file1) return 1; // error

	fseek(file1, 0, SEEK_END);
	const long file1_size = ftell(file1);
	fseek(file1, 0, SEEK_SET);

	const long buffer_size = file1_size + 1;
	unsigned char* const buffer = (unsigned char*)malloc(buffer_size);
	if (!buffer) return 1; // error

	fread_s(buffer, buffer_size, 1, file1_size, file1);
	buffer[buffer_size - 1] = '\0';

	size_t w = 0;
	for (size_t i = 0; i < buffer_size; i++) {
		printf("%02x ", buffer[i]);
		w++;
		if (w == 16) { printf("\n"); w = 0; }
	}
	printf("\n");

	free(buffer);

	return 0; // ok
}

#endif // MFC_PRESENT

#ifdef _WIN32

#include <Windows.h>

typedef float (WINAPI* TheLibrary_GetData)(const float, const float);

// Note that the "MFC_testing" project does not reference the "TheLibrary" project.
// There is no build-time dependency.

// Also note that there is a post-build step which copies the target files (exe and dll)
// into the $(SolutionDir)install directory and the debugger is configured to work there,
// not in a standard build directory.

int TestLoadLibrary_Win(void)
{
#ifdef _MSC_VER
	constexpr char libName_[] = "TheLibrary.dll";
#else // not MSVS - assume GCC, incl. mingw
	constexpr char libName_[] = "libTheLibrary.dll";
#endif

	//constexpr size_t libNameLen = sizeof(libName_);
	//wchar_t* libName = new wchar_t[libNameLen];
	//mbstowcs(libName, libName_, libNameLen);

	const HMODULE lib = LoadLibrary(libName_);
	if (!lib) {
		printf("Failed to load %s\n", libName_);
		//delete[] libName;
		return 1;
	}

	constexpr char funcName_[] = "DoStuff";
	TheLibrary_GetData func = (TheLibrary_GetData)GetProcAddress(lib, funcName_);
	if (!func) {
		printf("Failed to get %s from %s\n", funcName_, libName_);
		//delete[] libName;
		return 1;
	}

	////TheClass obj1;
	////obj1.SetData(5.0f);

	// You should add the build-time reference from "MFC_testing" to "TheLibrary"
	// to use this functionality
	//const float data1 = DoStuff(1.0f, 2.0f);
	//printf("data=%f\n", data1);

	const float data2 = func(1.0f, 2.0f);
	printf("data=%f\n", data2);

	//delete[] libName;
	return 0;
}

#endif // _WIN32

#ifndef _WIN32

int TestLoadLibrary_Linux(void)
{
	constexpr char libName_[] = "../lib/libTheLibrary.so";
	//constexpr size_t libNameLen = sizeof(libName_);
	//wchar_t* libName = new wchar_t[libNameLen];
	//mbstowcs(libName, libName_, libNameLen);

	void* lib = dlopen(libName_, RTLD_NOW);
	if (!lib) {
		printf("Failed to load %s\n", libName_);
		//delete[] libName;
		return 1;
	}

	constexpr char funcName_[] = "DoStuff";
	float (*func)(const float, const float);
	func = (float (*)(const float, const float))dlsym(lib, funcName_);
	if (!func) {
		printf("Failed to get %s from %s\n", funcName_, libName_);
		//delete[] libName;
		return 1;
	}

	////TheClass obj1;
	////obj1.SetData(5.0f);

	// You should add the build-time reference from "MFC_testing" to "TheLibrary"
	// to use this functionality
	//const float data1 = DoStuff(1.0f, 2.0f);
	//printf("data=%f\n", data1);

	const float data2 = func(1.0f, 2.0f);
	printf("data=%f\n", data2);

	//delete[] libName;
	return 0;
}

#endif // !_WIN32

void TestLoadLibrary(void)
{
#ifdef _WIN32
	TestLoadLibrary_Win();
#else
	TestLoadLibrary_Linux();
#endif
}

int main(int argc, char** argv)
{
#ifdef MFC_PRESENT

	constexpr char filename[] = R"(C:\tmp\MFC_testing\archive1.bin)";
	CreateArchive(filename);
	PrintArchiveRaw(filename);

#endif // MFC_PRESENT

	TestLoadLibrary();

	return 0;
}
