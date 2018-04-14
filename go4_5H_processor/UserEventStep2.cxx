#include "UserEventStep2.h"

// STD
/*#include <iostream>
using std::cerr;
using std::endl;*/

// ROOT
#include <TClonesArray.h>

// Project
#include "data/BeamDetMWPCDigi.h"

UserEventStep2::UserEventStep2(const char* name) :
	TGo4EventElement(name)
{
	//cerr << "UserEventStep2::UserEventStep2" << endl;
	mMWPCdigi = new TClonesArray("BeamDetMWPCDigi");
}

UserEventStep2::~UserEventStep2()
{
}

void UserEventStep2::Clear(Option_t* t)
{
	//TODO zero all data members!

	mMWPCdigi->Clear();
}

ClassImp(UserEventStep2)
