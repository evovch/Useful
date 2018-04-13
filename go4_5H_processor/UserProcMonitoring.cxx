#include "UserProcMonitoring.h"

UserProcMonitoring::UserProcMonitoring(const char* name) :
	TGo4EventProcessor(name)
{
}

UserProcMonitoring::~UserProcMonitoring()
{
}

Bool_t UserProcMonitoring::BuildEvent(TGo4EventElement* p_dest)
{
	mEventCounter++;

	return kTRUE;
}

ClassImp(UserProcMonitoring)
