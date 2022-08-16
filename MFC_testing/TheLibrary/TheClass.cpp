#include "TheClass.h"

TheClass::TheClass(void)
	: _data(0.0f) {
}

TheClass::~TheClass(void) {
}

const float& TheClass::GetData(void) const {
	return _data;
}

void TheClass::SetData(const float data) {
	_data = data;
}

//static
float DoStuff(const float x, const float y) {
	return x + y;
}
