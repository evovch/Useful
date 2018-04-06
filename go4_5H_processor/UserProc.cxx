#include "UserProc.h"

//TODO test //TODO why not iostream?
#include <Riostream.h>
using std::cout;
using std::endl;

#include <TGo4EventElement.h>
#include <TGo4MbsEvent.h>
#include <TGo4MbsSubEvent.h>

#include "UserEvent.h"

UserProc::UserProc(const char* name) :
	TGo4EventProcessor(name)
{
}

UserProc::~UserProc()
{
}

/**
	Rather official ROOT coding convention:
	Variables starting with 'f' are the class data members.
	They are known everywhere inside the class methods.
	Do not create local variables starting with 'f'.
	This will make your code so hard to understand and create
	potential naming clash...

	Unofficial coding convention:
	variables starting with 'f' or 'm' are the class data members;
	variables starting with 'p_' are the parameters of the the method;
		in case you want to specify explicitly:
		starting with 'i_' are the input parameters (usually byval);
		starting with 'o_' are the output parameters (usually byref);
	variables starting with v_ are the local variables.
	variables without any identifiable prefix are usually the local ones.
*/
Bool_t UserProc::BuildEvent(TGo4EventElement* p_dest)
{
	UserEvent* outputEvent = (UserEvent*)p_dest;
	TGo4MbsSubEvent* v_pSubevent;

	TGo4MbsEvent* v_input = (TGo4MbsEvent*)GetInputEvent();
	if (v_input == NULL)
	{
		cout << "UserProc::BuildEvent(): no input event!" << endl;
		return kFALSE;
	}

	v_input->ResetIterator();
	while ((v_pSubevent = v_input->NextSubEvent()) != NULL)
	{
		Int_t v_intLen = v_pSubevent->GetIntLen();
		Int_t* v_dataField = v_pSubevent->GetDataField();

		cout << "intLen = " << v_intLen << endl;
	}

	return kTRUE;
}

ClassImp(UserProc)
