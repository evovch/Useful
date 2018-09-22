#include "myClass1.h"

#include "myClass2.h"

#include <iostream>
using std::cout;
using std::endl;

myClass1::myClass1() :
	TObject(),
	mMember1(10),
	mNobjects(0),
	mArray(nullptr)
{
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
	cout << endl;
	cout << "==== myClass1 ====" << endl;
	for (Int_t i=0; i<mNobjects; i++) {
		cout << mArray[i].GetData() << endl;
	}
	cout << "==================" << endl;
}

ClassImp(myClass1)
