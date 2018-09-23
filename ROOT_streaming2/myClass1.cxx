#include "myClass1.h"

#include "myClass2.h"

#include <iostream>

myClass1::myClass1() :
	TObject(),
	mMember1(10),
	mNobjects(0),
	//TODO
	mArray(nullptr)
	//TODO try removing this nullptr-initialization (and the comma of cause)
	// You will get no problems neither during compilation
	// nor during writing out.
	// However ROOT will crash at output root file import.
	// This is very hard to debug...

{
	std::cout << "myClass1 constructor" << std::endl;
}

myClass1::~myClass1()
{
	// Should we do that?
	if (mArray) { delete [] mArray; mArray = nullptr; }
}

void myClass1::Generate(void)
{
	mNobjects = 4;
	mArray = new myClass2[mNobjects];
	for (Int_t i=0; i<mNobjects; i++) {
		mArray[i].SetData(i);
	}
}

void myClass1::Print(Option_t* /*opt*/) const
{
	std::cout << std::endl;
	std::cout << "==== myClass1 ====" << std::endl;
	for (Int_t i=0; i<mNobjects; i++) {
		std::cout << mArray[i].GetData() << std::endl;
	}
	std::cout << "==================" << std::endl;
}

ClassImp(myClass1)
