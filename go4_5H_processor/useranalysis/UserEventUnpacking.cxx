#include "UserEventUnpacking.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// ROOT
#include <TClonesArray.h>

// Project
#include "data/RawMessage.h"

UserEventUnpacking::UserEventUnpacking(const char* name) :
	TGo4EventElement(name)
{
	mRawMessages = new TClonesArray("RawMessage");

	//cerr << "UserEventUnpacking::UserEventUnpacking() -> ";
	this->Clear();
}

UserEventUnpacking::~UserEventUnpacking()
{
	//TODO delete mRawMessages?
}

void UserEventUnpacking::Clear(Option_t* t)
{
	//TODO zero all data members!
	//cerr << "UserEventUnpacking::Clear()" << endl;

	mRawMessages->Clear();

	for (UInt_t i=0; i<8; i++) {
		mCAMAC[i] = 0;
	}
}

void UserEventUnpacking::Dump(void) const
{
	cerr << "UserEventUnpacking contains " << mRawMessages->GetEntries() << " raw messages." << endl;
	//TODO implement
}

ClassImp(UserEventUnpacking)
