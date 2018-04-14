#include "UserEventMonitoring.h"

UserEventMonitoring::UserEventMonitoring(const char* name) :
	TGo4EventElement(name)
{
}

UserEventMonitoring::~UserEventMonitoring()
{
}

void UserEventMonitoring::Clear(Option_t* t)
{
	//TODO zero all data members!

	for (UInt_t i=0; i<4; i++) {
		F3[i] = 0;
		tF3[i] = 0;
		F5[i] = 0;
		tF5[i] = 0;
		F6[i] = 0;
		tF6[i] = 0;
		tMWPC[i] = 0;
	}
}

ClassImp(UserEventMonitoring)
