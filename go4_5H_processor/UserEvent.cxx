#include "UserEvent.h"

//TODO test //TODO why not iostream?
/*#include <Riostream.h>
using std::cout;
using std::endl;*/

UserEvent::UserEvent(const char* name) :
	TGo4EventElement(name)
{
}

UserEvent::~UserEvent()
{
}

void UserEvent::Clear(Option_t* t)
{
}

ClassImp(UserEvent)
