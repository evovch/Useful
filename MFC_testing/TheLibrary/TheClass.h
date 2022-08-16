#pragma once

#include "TheLibrary.h"

class TheClass
{
private:
	float _data;
public:
	LIBRARY_API TheClass(void);
	LIBRARY_API ~TheClass(void);
public:
	LIBRARY_API const float& GetData(void) const;
	LIBRARY_API void SetData(const float data);
};

extern "C" {

LIBRARY_API float DoStuff(const float x, const float y);

};
