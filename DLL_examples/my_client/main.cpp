#include <iostream>

#include "my_lib.h"
#include "some_data_class.h"

int main(int argc, char** argv)
{
	my_project::MyClass obj1;

	obj1.Method1();

	const my_project::SomeDataClass& data1 = obj1.GetMember1();
	const my_project::SomeDataClass& data2 = obj1.GetMember2();
	const my_project::SomeDataClass& data3 = obj1.GetMember3();
	const my_project::SomeDataClass& data4 = obj1.GetMember4();

	std::cout << "data1=" << data1._x << std::endl;
	std::cout << "data2=" << data2._x << std::endl;
	std::cout << "data3=" << data3._x << std::endl;
	std::cout << "data4=" << data4._x << std::endl;

	return 0;
}
