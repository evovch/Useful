#pragma once

#include "mydlldefines.h"
#include "some_data_class.h"

namespace my_project {

class MyClass {
public:
	MyClass(void)
		: _member1(1.0f)
		, _member2(2.0f)
	{}
private:
	// non-static data member
	const SomeDataClass _member1;
	// non-static data member
	const SomeDataClass _member2;
	// static data member
	static const SomeDataClass _member3;
	// static data member
	static const SomeDataClass _member4;
public:
	MYPROJECTAPI void Method1(void) const;

	// non-static data member, method implementation in the source file
	MYPROJECTAPI const SomeDataClass& GetMember1(void) const;
	// non-static data member, method implementation in the header file
	MYPROJECTAPI const SomeDataClass& GetMember2(void) const {
		return _member2;
	}
	// static data member, method implementation in the source file
	MYPROJECTAPI static const SomeDataClass& GetMember3(void);
	// static data member, method implementation in the header file
	MYPROJECTAPI static const SomeDataClass& GetMember4(void) {
		return _member4;
	}
};

} // end of namespace my_project
