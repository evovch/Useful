#ifndef MYCLASS1_H
#define MYCLASS1_H

#include <TObject.h>

class myClass2;

class myClass1 : public TObject
{
public:
	myClass1();
	virtual ~myClass1();

	void Generate(void);

	void Print(Option_t* opt = "") const; // *MENU*

private:
	// Just some data members
	Int_t mMember1;

	Int_t mNobjects;

	myClass2* mArray; //[mNobjects]

	ClassDef(myClass1, 1);
};

#endif // MYCLASS1_H
