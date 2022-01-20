#pragma once

#include "import_export_spec2.h"

class MYDLLAPI TheClass2
{
private:
	bool _data;
public:
	explicit TheClass2(void) : _data(false) {}
	~TheClass2(void) {}
	TheClass2(const TheClass2& other) = delete;
	TheClass2(TheClass2&& other) noexcept = delete;
	TheClass2& operator=(const TheClass2& other) = delete;
	TheClass2& operator=(TheClass2&& other) noexcept = delete;
public:
	bool TheFunction(void) const;
};
