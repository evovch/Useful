#pragma once

namespace my_project {

class SomeDataClass {
public:
	explicit SomeDataClass(void) : _x(0.0f) {}
	explicit SomeDataClass(const float& x) : _x(x) {}
public:
	float _x;
};

} // end of namespace my_project
