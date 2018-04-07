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
#include "UserAnalysisHistos.h"

UserProc::UserProc(const char* name) :
	TGo4EventProcessor(name),
	mEventCounter(0)
{
	mHistoMan = new UserAnalysisHistos();
}

UserProc::~UserProc()
{
	if (mHistoMan) delete mHistoMan;
}

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

	cerr << "UserProc: Event " << mEventCounter
	     << "==========================================================================================================="
	     << endl;

	mCurrentOutputEvent = v_outputEvent;

	////this->DumpEventHeader(v_input);

    // Set the internal iterator of the subevent array to the beginning of the array.
    // I have no idea why do we have to do this...
	v_input->ResetIterator();

	TGo4MbsSubEvent* v_pSubevent; // Current subevent
	UInt_t v_subEventCounter = 0; // Counter of subevents within the current event
	while ((v_pSubevent = v_input->NextSubEvent()) != NULL)
	{
		cerr << "\t" << "SubEvent " << v_subEventCounter << endl;

		this->ProcessSubevent(v_pSubevent);

		v_subEventCounter++;
	}

	v_outputEvent->SetValid(v_isValid);

	mEventCounter++;

	return v_isValid;
}

void UserProc::ProcessSubevent(TGo4MbsSubEvent* p_subevent)
{
	Int_t v_dLen = p_subevent->GetDlen();
	Short_t v_type = p_subevent->GetType();
	Char_t v_subcrate = p_subevent->GetSubcrate();
	Char_t v_control = p_subevent->GetControl();
	Short_t v_procID = p_subevent->GetProcid();

	Int_t v_intLen = p_subevent->GetIntLen(); // Get the size of the current subevent
	Int_t* v_dataField = p_subevent->GetDataField(); // Get the pointer to the data of the subevent

	cerr << "\t" << "v_dLen=" << v_dLen << "\t"
	             << "v_type=" << v_type << "\t"
	             << "v_subcrate=" << (Int_t)v_subcrate << "\t"
	             << "v_control=" << (Int_t)v_control << "\t"
	             << "procID=" << v_procID << endl;

	/**
		Сейчас это пока что неверно и оставлено здесь просто для примера.
		В событиии несколько subevent'ов, и для каждого subevent'а нужно писать
		выходной объект. Сейчас эти значения пишутся для каждого собтия по
		значениям из последнего subevent'а.
	**/
	mCurrentOutputEvent->mSubcrate = (Int_t)v_subcrate;
	mCurrentOutputEvent->mControl = (Int_t)v_control;
	mCurrentOutputEvent->mProcID = v_procID;

	//this->DumpSubeventData(v_intLen, v_dataField);
	this->ProcessSubeventRaw(v_intLen, v_dataField);
}

void UserProc::ProcessSubeventRaw(Int_t p_size, Int_t* p_startAddress)
{
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
		if ((i+1)%8 == 0) { cerr << endl; }
	}
	cerr << endl;
}

ClassImp(UserProc)
