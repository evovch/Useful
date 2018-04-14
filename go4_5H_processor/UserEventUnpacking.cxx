#include "UserEventUnpacking.h"

// STD
/*#include <iostream>
using std::cerr;
using std::endl;*/

// ROOT
#include <TClonesArray.h>

// Project
#include "data/RawMessage.h"

UserEventUnpacking::UserEventUnpacking(const char* name) :
	TGo4EventElement(name)
{
	mRawMessages = new TClonesArray("RawMessage");
}

UserEventUnpacking::~UserEventUnpacking()
{
	//TODO delete mRawMessages?
}

void UserEventUnpacking::Clear(Option_t* t)
{
	//TODO zero all data members!

	mRawMessages->Clear();

	for (UInt_t i=0; i<8; i++) {
		mCAMAC[i] = 0;
	}
}

ClassImp(UserEventUnpacking)
