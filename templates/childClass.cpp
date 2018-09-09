#include "childClass.h"

#include <iostream>
using std::cout;
using std::endl;

template <typename T>
childClass<T>::childClass() :
	baseClass<T>()
{
	cout << "childClass constructor" << endl;
	// You HAVE TO use 'this' (try without it!) 
	this->protMemBase = 2;
	// You can't do this because privMemBase is private in the baseClass
	//this->privMemBase = 2;
}

template <typename T>
childClass<T>::~childClass()
{
	cout << "childClass destructor" << endl;
}
