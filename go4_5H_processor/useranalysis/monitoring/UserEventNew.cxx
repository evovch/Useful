#include "UserEventNew.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// Project
#include "UserEventLeftTele.h"
#include "UserEventRightTele.h"

UserEventNew::UserEventNew(const char* name) :
	TGo4CompositeEvent(name, name)
{
	addEventElement(new UserEventLeftTele());
	addEventElement(new UserEventRightTele());

	//cerr << "UserEventNew::UserEventNew() -> ";
	this->Clear();
}

UserEventNew::~UserEventNew()
{
}

void UserEventNew::Clear(Option_t* t)
{
	//TODO zero all data members!
	//cerr << "UserEventNew::Clear()" << endl;

}

void UserEventNew::Dump(void) const
{
	//TODO dump all data members!

}

ClassImp(UserEventNew)
