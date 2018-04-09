#include "UserProc.h"

//TODO test //TODO why not iostream?
#include <Riostream.h>
using std::cerr;
using std::endl;

#include <TGo4EventElement.h>
#include <TGo4MbsEvent.h>
#include <TGo4MbsSubEvent.h>

#include "UserEvent.h"
#include "UserAnalysisHistos.h"

/*static*/ bool UserProc::mInsidePackage = false;

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
		cerr << "UserProc::BuildEvent(): no input event!" << endl;
		v_outputEvent->SetValid(v_isValid);
		return v_isValid;
	}
	v_isValid = kTRUE;

	cerr << "UserProc: Event " << mEventCounter
	     << " ==========================================================================================================="
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
		cerr << "SubEvent " << v_subEventCounter
		     << " -----------------------------------------------------------------" << endl;

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

	cerr << "v_dLen=" << v_dLen << "\t"
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
	//this->ProcessSubeventRaw(v_intLen, v_dataField);

	switch (v_procID) {
	case 100:
		cerr << "VME0" << endl;
		this->ProcessSubeventRawVME0(v_intLen, v_dataField);
		break;
	case 200:
		cerr << "VME1" << endl;
		this->ProcessSubeventRawVME1(v_intLen, v_dataField);
		break;
	case 101:
		cerr << "CAMAC" << endl;
		this->ProcessSubeventRawCAMAC(v_intLen, v_dataField);
		break;
	default:
		break;
	}

}

void UserProc::ProcessSubeventRaw(Int_t p_size, Int_t* p_startAddress)
{
	for (unsigned int v_cursor=0; v_cursor<p_size; /*no action here*/) // v_cursor is incremented inside
	{
		//cerr << "v_cursor=" << v_cursor << endl;

		int v_leftSize = p_size - v_cursor;
		int v_footerPosition = 0; //TODO or better init with -1 ?

		switch (UserProc::CheckNextHeader(&p_startAddress[v_cursor])) {
		case MESYTEC:

			cerr << "MESYTEC Header v_cursor=" << v_cursor << endl;

			v_footerPosition = UserProc::FindMESYTECfooter(v_leftSize, &p_startAddress[v_cursor]);
			if (v_footerPosition > -1) {
				cerr << "MESYTEC footer found at v_cursor(" << v_cursor
				     << ") + v_footerPosition(" << v_footerPosition
				     << ") = " << v_cursor+v_footerPosition
				     << endl;

				this->ProcessSubsubevent_MESYTEC(v_footerPosition+1, &p_startAddress[v_cursor]);

				v_cursor += (v_footerPosition + 1); // right after the footer
			} else {
				cerr << "MESYTEC footer not found!" << endl;
				v_cursor++;
			}

			break;
		case CAEN:

			cerr << "CAEN header v_cursor=" << v_cursor << endl;

			v_footerPosition = UserProc::FindCAENfooter(v_leftSize, &p_startAddress[v_cursor]);
			if (v_footerPosition > -1) {
				cerr << "CAEN footer found at v_cursor(" << v_cursor
				     << ") + v_footerPosition(" << v_footerPosition
				     << ") = " << v_cursor+v_footerPosition
				     << endl;

				this->ProcessSubsubevent_CAEN(v_footerPosition+1, &p_startAddress[v_cursor]);

				v_cursor += (v_footerPosition + 1); // right after the footer
			} else {
				cerr << "CAEN footer not found!" << endl;
				v_cursor++;
			}

			break;
		case OTHER:
			cerr << "OTHER VENDOR" << endl;
			v_cursor++;
			break;
		}
	}
}

void UserProc::ProcessSubeventRawVME0(Int_t p_size, Int_t* p_startAddress)
{
	cerr << "Processing raw subevent from VME0. size=" << p_size << endl;
	this->ProcessSubeventRaw(p_size, p_startAddress);
}

void UserProc::ProcessSubeventRawVME1(Int_t p_size, Int_t* p_startAddress)
{
	cerr << "Processing raw subevent from VME1. size=" << p_size << endl;
	this->ProcessSubeventRaw(p_size, p_startAddress);
}

void UserProc::ProcessSubeventRawCAMAC(Int_t p_size, Int_t* p_startAddress)
{
	cerr << "Processing raw subevent from CAMAC. size=" << p_size << endl;
	//cerr << "\t" << "Skipping." << endl;

	for (unsigned int v_cursor=0; v_cursor<p_size; v_cursor++) {

		unsigned int v_curWord = p_startAddress[v_cursor];
		unsigned int v_geo = (v_curWord >> 27) & 0x1f; //TODO mask is unknown to me // 5 bits?

		fprintf(stderr, "%d: %08x\tgeo=%u\n", v_cursor, p_startAddress[v_cursor], v_geo);


	}

}

/*static*/
enu_VENDOR UserProc::CheckNextHeader(Int_t* p_startAddress)
{
	////fprintf(stderr, "%08x\n", p_startAddress[0]);

	int v_mesytec_flag = (p_startAddress[0] >> 24) & 0xff; // = 0x40 // 8 bits
	int v_caen_header = (p_startAddress[0] >> 24) & 0x7; // = 0x2 // 3 bits

	if (v_mesytec_flag == 0x40) {
		cerr << "Identified as MESYTEC block" << endl;
		return MESYTEC;
	} else {
			if (v_caen_header == 2) { // 010 (binary)
				cerr << "Identified as CAEN block" << endl;
				return CAEN;
			} else {
				fprintf(stderr, "%08x\n", p_startAddress[0]);
				cerr << "Identified as OTHER block" << endl;
				return OTHER;
			}
	}
}

/**
	Return -1 if not found!
*/
/*static*/
int UserProc::FindMESYTECfooter(Int_t p_maxSize, Int_t* p_startAddress)
{
	for (unsigned int v_cursor=0; v_cursor<p_maxSize; v_cursor++) {

		Int_t tmp = p_startAddress[v_cursor];
		Int_t v_curWord = tmp;
		unsigned int v_type = (v_curWord >> 30) & 0x3;

		////fprintf(stderr, "%08x type=%d\n", v_curWord, v_type);

		if (v_type == 3) {
			// type==3 - MESYTEC footer
			return v_cursor;
		}
	}
	return -1;
}

/**
	Return -1 if not found!
*/
/*static*/
int UserProc::FindCAENfooter(Int_t p_maxSize, Int_t* p_startAddress)
{
	for (unsigned int v_cursor=0; v_cursor<p_maxSize; v_cursor++) {

		Int_t tmp = p_startAddress[v_cursor];
		Int_t v_curWord = tmp;
		int v_type = (v_curWord >> 24) & 0x7;

		////fprintf(stderr, "%08x type=%d\n", v_curWord, v_type);

		if (v_type == 4) {
			// type==4 - CAEN footer
			return v_cursor;
		}
	}
	return -1;
}

void UserProc::ProcessSubsubevent_MESYTEC(Int_t p_size, Int_t* p_startAddress)
{
	cerr << "ProcessSubsubevent_MESYTEC size=" << p_size << endl;

	for (unsigned int v_cursor=0; v_cursor<p_size; v_cursor++) {

		Int_t tmp = p_startAddress[v_cursor];
		Int_t v_curWord = tmp;

		unsigned int v_type = (v_curWord >> 30) & 0x3;

		fprintf(stderr, "%08x\t", v_curWord);

		int v_module_id;
		int v_channel;
		int v_valueQA; // QDC and ADC
		int v_valueT; // TDC

		switch (v_type) {
		case 1: // MESYTEC header
			v_module_id = (v_curWord >> 16) & 0xff;
			cerr << "header module_id=" << v_module_id << endl;
			break;
		case 3: // MESYTEC footer
			cerr << "footer" << endl;
			break;
		case 0: // MESYTEC data
		 	//TODO check
			v_channel = (v_curWord >> 16) & 0x1f; // 5 bits
			v_valueQA = v_curWord & 0x1fff; // 13 bits // ADC and QDC
			v_valueT = v_curWord & 0xffff; // 16 bits // TDC
			cerr << "MESYTEC data ch=" << v_channel << " valueQA=" << v_valueQA << " valueT=" << v_valueT << endl;
			break;
		default:
			cerr << "unknown MESYTEC type" << endl;
			break;
		}

	}
}

void UserProc::ProcessSubsubevent_CAEN(Int_t p_size, Int_t* p_startAddress)
{
	cerr << "ProcessSubsubevent_CAEN size=" << p_size << endl;

	for (unsigned int v_cursor=0; v_cursor<p_size; v_cursor++) {

		Int_t tmp = p_startAddress[v_cursor];
		Int_t v_curWord = tmp;

		int v_type = (v_curWord >> 24) & 0x7;
		int v_geo = (v_curWord >> 27) & 0x1f;
		int v_channel;
		int v_value;

		fprintf(stderr, "%08x\t", v_curWord);

		switch (v_type) {
		case 2: // CAEN header
			cerr << "\t" << v_cursor << ":\t" << "CAEN header" << "\t\t\ttype=" << v_type << " geo=" << v_geo << endl;
			mInsidePackage = true;
			break;
		case 4: // CAEN footer
			cerr << "\t" << v_cursor << ":\t" << "CAEN footer" << "\t\t\ttype=" << v_type << " geo=" << v_geo << endl;
			mInsidePackage = false;
			break;
		case 6: // CAEN no valid data
			cerr << "\t" << v_cursor << ":\t" << "CAEN no valid data" << "\ttype=" << v_type << " geo=" << v_geo << endl;
			break;
		case 0: // CAEN data

			v_channel = (v_curWord >> 16) & 0x1f; //TODO check
			v_value = v_curWord & 0xfff; //TODO check

			cerr << "\t" << v_cursor << ":\t" << "CAEN data" << "\t\t\ttype=" << v_type << " geo=" << v_geo
			     << " ch=" << v_channel << " adc=" << v_value << endl;

			if (!mInsidePackage) {
				cerr << "[ERROR ] " << "Data word found not between the header and the footer." << endl;
			}
			break;
		default:
			cerr << "\t" << v_cursor << ":\t" << "CAEN unknown" << "\t\t\ttype=" << v_type << " geo=" << v_geo << endl;
			break;
		}

	}
}

/*void UserProc::ProcessSubeventRaw_MESYTEC(Int_t* p_startAddress, Int_t p_cursor)
{
	int v_type = (p_startAddress[p_cursor] >> 30) & 0x3;
	cerr << "VME0 MESYTEC type=" << v_type << endl;

	int v_module_id;

	switch (v_type) {
	case 1: // MESYTEC header
		v_module_id = (p_startAddress[p_cursor] >> 16) & 0xff;
		cerr << "header module_id=" << v_module_id << endl;
		break;
	case 3: // MESYTEC footer
		cerr << "footer" << endl;
		fprintf(stderr, "%02x", p_startAddress[p_cursor]);
		break;
	case 0: // MESYTEC data
	default:
		cerr << "unknown MESYTEC type" << endl;
		fprintf(stderr, "%02x", p_startAddress[p_cursor]);
		break;
	}
}*/

/*void UserProc::ProcessSubeventRaw_CAEN(Int_t* p_startAddress, Int_t p_cursor)
{
	Int_t tmp = p_startAddress[p_cursor];
	////Int_t v_curWord = ((tmp << 16) & 0xffff0000) | ((tmp >> 16) & 0x0000ffff);
	Int_t v_curWord = tmp;

	int v_type = (v_curWord >> 24) & 0x7;
	int v_geo = v_curWord >> 27; // TODO bitmask!
	int v_adc;
	int v_channel;

	////cerr << p_cursor << ": CAEN type=" << v_type << " geo=" << v_geo << endl;

	fprintf(stderr, "%08x", v_curWord);

	switch (v_type) {
	case 2: // CAEN header
		cerr << "\t" << p_cursor << ":\t" << "CAEN header" << "\t\t\ttype=" << v_type << " geo=" << v_geo << endl;
		mInsidePackage = true;
		break;
	case 4: // CAEN footer
		cerr << "\t" << p_cursor << ":\t" << "CAEN footer" << "\t\t\ttype=" << v_type << " geo=" << v_geo << endl;
		mInsidePackage = false;
		break;
	case 6: // CAEN no valid data
		cerr << "\t" << p_cursor << ":\t" << "CAEN no valid data" << "\ttype=" << v_type << " geo=" << v_geo << endl;
		break;
	case 0: // CAEN data

		v_channel = (v_curWord >> 16) & 0x1f;
		v_adc = v_curWord & 0xfff;

		cerr << "\t" << p_cursor << ":\t" << "CAEN data" << "\t\t\ttype=" << v_type << " geo=" << v_geo
		     << " ch=" << v_channel << " adc=" << v_adc << endl;

		if (!mInsidePackage) {
			cerr << "[ERROR ] " << "Data word found not between the header and the footer." << endl;
		}
		break;
	default:
		cerr << "\t" << p_cursor << ":\t" << "CAEN unknown" << "\t\t\ttype=" << v_type << " geo=" << v_geo << endl;
		break;
	}
}*/

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
	cerr << "SubEvent size = " << p_size << endl;
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
