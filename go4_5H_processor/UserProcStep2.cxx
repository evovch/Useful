#include "UserProcStep2.h"

//TODO test //TODO why not iostream?
#include <Riostream.h>
using std::cout;
using std::cerr;
using std::endl;

UserProcStep2::UserProcStep2(const char* name) :
	TGo4EventProcessor(name),
	mEventCounter(0)
{
}

UserProcStep2::~UserProcStep2()
{
}

Bool_t UserProcStep2::BuildEvent(TGo4EventElement* p_dest)
{
	cerr << "UserProcStep2: Event " << mEventCounter << endl;
	mEventCounter++;
	return kTRUE;
}

ClassImp(UserProcStep2)
