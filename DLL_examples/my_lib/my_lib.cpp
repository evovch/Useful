#include "my_lib.h"

#include <iostream>

namespace my_project {

//static
const SomeDataClass MyClass::_member3 = SomeDataClass(3.0f);

//static
const SomeDataClass MyClass::_member4 = SomeDataClass(4.0f);

void MyClass::Method1(void) const {
	std::cout << "MyClass::Method1()" << std::endl;
}

const SomeDataClass& MyClass::GetMember1(void) const {
	return _member1;
}

//static
const SomeDataClass& MyClass::GetMember3(void) {
	return _member3;
}

} // end of namespace my_project
