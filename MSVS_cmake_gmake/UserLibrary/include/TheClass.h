#pragma once

#include "import_export_spec.h"

class MYDLLAPI TheClass
{
private:
	bool _data;
public:
	explicit TheClass(void) : _data(false) {}
	~TheClass(void) {}
	TheClass(const TheClass& other) = delete;
	TheClass(TheClass&& other) noexcept = delete;
	TheClass& operator=(const TheClass& other) = delete;
	TheClass& operator=(TheClass&& other) noexcept = delete;
public:
	bool TheFunction(void) const;
};
