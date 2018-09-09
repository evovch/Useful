#include "baseClass.h"

#include <iostream>
using std::cout;
using std::endl;

template <typename T>
baseClass<T>::baseClass()
{
	cout << "baseClass constructor" << endl;
	protMemBase = 1;
	privMemBase = 1;
}

template <typename T>
baseClass<T>::~baseClass()
{
	cout << "baseClass destructor" << endl;
}

template <typename T>
void baseClass<T>::method2(T param)
{
	this->method1(param);
}
