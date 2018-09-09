#include "childClass.h"

int main(int /*argc*/, char** /*argv*/)
{
	childClass<int> obj1;
	obj1.method1(1);
	obj1.method2(1);
}