#include "UserEvent.h"

//TODO test //TODO why not iostream?
/*#include <Riostream.h>
using std::cout;
using std::endl;*/

// ROOT
#include <TClonesArray.h>

// Project
#include "data/RawMessage.h"

UserEvent::UserEvent(const char* name) :
	TGo4EventElement(name)
{
	mRawMessages = new TClonesArray("RawMessage");
}

UserEvent::~UserEvent()
{
	//TODO delete mRawMessages?
}

void UserEvent::Clear(Option_t* t)
{
	mRawMessages->Clear();
}

ClassImp(UserEvent)
