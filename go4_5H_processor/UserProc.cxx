#include "UserProc.h"

//TODO test //TODO why not iostream?
#include <Riostream.h>
using std::cout;
using std::cerr;
using std::endl;

#include <TGo4EventElement.h>
#include <TGo4MbsEvent.h>
#include <TGo4MbsSubEvent.h>

#include "UserEvent.h"

UserProc::UserProc(const char* name) :
	TGo4EventProcessor(name),
	mEventCounter(0)
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
	variables without any identifiable prefix are usually the local ones -
		thid is usually the case for simple one-letter names:
		i, j, k, x, y, z, n, etc...
*/
Bool_t UserProc::BuildEvent(TGo4EventElement* p_dest)
{
	Bool_t v_isValid = kFALSE;
	UserEvent* v_outputEvent = (UserEvent*)p_dest;

	TGo4MbsEvent* v_input = (TGo4MbsEvent*)GetInputEvent();
	if (v_input == NULL)
	{
		cout << "UserProc::BuildEvent(): no input event!" << endl;
		v_outputEvent->SetValid(v_isValid);
		return v_isValid;
	}
	v_isValid = kTRUE;

	cerr << "UserProc: Event " << mEventCounter << endl;

	////this->DumpEventHeader(v_input);

    // Set the internal iterator of the subevent array to the beginning of the array.
    // I have no idea why do we have to do this...
	v_input->ResetIterator();

	TGo4MbsSubEvent* v_pSubevent; // Current subevent
	UInt_t v_subEventCounter = 0; // Counter of subevents within the current event
	while ((v_pSubevent = v_input->NextSubEvent()) != NULL)
	{
		cerr << "\t" << "SubEvent " << v_subEventCounter << endl;

		Int_t v_intLen = v_pSubevent->GetIntLen(); // Get the size of the current subevent
		Int_t* v_dataField = v_pSubevent->GetDataField(); // Get the pointer to the data of the subevent

		//this->DumpSubeventData(v_intLen, v_dataField);
		this->ProcessSubevent(v_intLen, v_dataField);

		v_subEventCounter++;
	}

	v_outputEvent->SetValid(v_isValid);

	mEventCounter++;

	return v_isValid;
}

void UserProc::ProcessSubevent(Int_t p_size, Int_t* p_startAddress)
{
	//TODO Do your processing here
}

void UserProc::DumpEventHeader(TGo4MbsEvent* p_inp_evt) const
{
	s_filhe* head = p_inp_evt->GetMbsSourceHeader();
	if (head != NULL)
	{
		cerr << "found filhe structure:" << endl;
		cerr << "\tdatalen: " << head->filhe_dlen << endl;
		cerr << "\tfilename_l: " << head->filhe_file_l << endl;
		cerr << "\tfilename: " << head->filhe_file << endl;
		cerr << "\ttype: " << head->filhe_type << endl;
		cerr << "\tsubtype: " << head->filhe_subtype << endl;
		cerr << "\t#commentlines: " << head->filhe_lines << endl;
	} else {
		cerr << "zero file header" << endl;
	}
}

void UserProc::DumpSubeventData(Int_t p_size, Int_t* p_startAddress) const
{
	cerr << "\tSubEvent size = " << p_size << endl;
	for (Int_t i=0; i<p_size; i++) {
		Int_t v_curWord = p_startAddress[i];
		fprintf(stderr, "%02x", ((v_curWord >> 0) & 0xff));
		fprintf(stderr, "%02x ", ((v_curWord >> 8) & 0xff));
		fprintf(stderr, "%02x", ((v_curWord >> 16) & 0xff));
		fprintf(stderr, "%02x", ((v_curWord >> 24) & 0xff));
		fprintf(stderr, " ");
	}
	cerr << endl;
}

ClassImp(UserProc)
