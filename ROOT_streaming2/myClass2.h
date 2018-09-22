#ifndef MYCLASS2_H
#define MYCLASS2_H

#include <TObject.h>

class myClass2 : public TObject
{
public:
	myClass2();
	virtual ~myClass2();

	void SetData(Int_t val) { mMember2 = val; }

	Int_t GetData(void) const { return mMember2; }

private:
	// Just some data members
	Int_t mMember2;

	ClassDef(myClass2, 1);
};

#endif // MYCLASS2_H
