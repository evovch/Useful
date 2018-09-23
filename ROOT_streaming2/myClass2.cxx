#include "myClass2.h"

#include <iostream>

myClass2::myClass2() :
	TObject(),
	mMember2(20)
{
	std::cout << "myClass2 constructor" << std::endl;
}

myClass2::~myClass2()
{
}

ClassImp(myClass2)
