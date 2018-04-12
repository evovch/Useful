#include "UserProc.h"

//TODO test //TODO why not iostream?
#include <Riostream.h>
using std::cerr;
using std::endl;

// Go4
#include <TGo4EventElement.h>
#include <TGo4MbsEvent.h>
#include <TGo4MbsSubEvent.h>

// Project
#include "Support.h"
#include "UserEvent.h"
#include "UserAnalysisHistos.h"

/*static*/ bool UserProc::mInsidePackage = false;

#define PRINTDEBUGINFO

UserProc::UserProc(const char* name) :
	TGo4EventProcessor(name),
	mEventCounter(0),
	mHeadersWords(0),
	mNknownWords(0),
	mNunknownWords(0)
{
	mHistoMan = new UserAnalysisHistos();
	mFileSummary = fopen("summary.txt", "w");
	if (mFileSummary == NULL) {
		//TODO error
	}
}

UserProc::~UserProc()
{
	if (mHistoMan) delete mHistoMan;
	if (mFileSummary != NULL) {
		fclose(mFileSummary);
	}
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

	#ifdef PRINTDEBUGINFO
	cerr << "UserProc: Event " << mEventCounter
	     << " ==========================================================================================================="
	     << endl;
	#endif

	mCurrentOutputEvent = v_outputEvent;

	//this->DumpEventHeader(v_input);

	//TODO I did not actually find the corrent event header length
	mHeadersWords += 4;

	// Set the internal iterator of the subevent array to the beginning of the array.
	// I have no idea why do we have to do this...
	v_input->ResetIterator();

	TGo4MbsSubEvent* v_pSubevent; // Current subevent
	UInt_t v_subEventCounter = 0; // Counter of subevents within the current event
	while ((v_pSubevent = v_input->NextSubEvent()) != NULL)
	{
		#ifdef PRINTDEBUGINFO
		cerr << "SubEvent " << v_subEventCounter
		     << " -----------------------------------------------------------------" << endl;
		#endif

		this->ProcessSubevent(v_pSubevent);

		v_subEventCounter++;
	}

	v_outputEvent->SetValid(v_isValid);

	cerr << "[DEBUG ] " << "End of event " << mEventCounter << ".\t"
	     << "Headers' words: " << mHeadersWords << ",\t"
	     << "Known words: " << mNknownWords << ",\t"
	     << "unknown words: " << mNunknownWords << ",\t"
	     << "total: " << (mHeadersWords+mNknownWords+mNunknownWords)*sizeof(Int_t) << " bytes."
	     << endl;

	if (mFileSummary != NULL) {
		fprintf(mFileSummary, "End of event %ld.\tKnown words: %ld,\tunknown words: %ld,\ttotal: %ld bytes.\n",
		        mEventCounter, mNknownWords, mNunknownWords, (mNknownWords+mNunknownWords)*sizeof(Int_t));
	}

	mEventCounter++;

	return v_isValid;
}

void UserProc::ProcessSubevent(TGo4MbsSubEvent* p_subevent)
{
	//Int_t v_dLen = p_subevent->GetDlen();
	//Short_t v_type = p_subevent->GetType();
	Char_t v_subcrate = p_subevent->GetSubcrate();
	Char_t v_control = p_subevent->GetControl();
	Short_t v_procID = p_subevent->GetProcid();
	//Int_t v_byteLen = p_subevent->GetByteLen();

	Int_t v_intLen = p_subevent->GetIntLen(); // Get the size of the current subevent
	Int_t* v_dataField = p_subevent->GetDataField(); // Get the pointer to the data of the subevent

	//this->DumpSubeventHeader(p_subevent);

	//TODO I did not actually find the corrent subevent header length
	mHeadersWords += 3;

	//this->DumpSubeventData2(v_intLen, v_dataField);
	////this->ProcessSubeventRaw(v_intLen, v_dataField);

	/**
		Сейчас это пока что неверно и оставлено здесь просто для примера.
		В событиии несколько subevent'ов, и для каждого subevent'а нужно писать
		выходной объект. Сейчас эти значения пишутся для каждого события по
		значениям из последнего subevent'а.
	**/
	/*mCurrentOutputEvent->mSubcrate = (Int_t)v_subcrate;
	mCurrentOutputEvent->mControl = (Int_t)v_control;
	mCurrentOutputEvent->mProcID = v_procID;*/

	switch (v_procID) {
	case 100:
		#ifdef PRINTDEBUGINFO
		cerr << "[DEBUG ] Found procID " << v_procID << " which corresponds to VME0" << endl;
		#endif
		this->ProcessSubeventRawVME0(v_intLen, v_dataField);
		break;
	case 200:
		#ifdef PRINTDEBUGINFO
		cerr << "[DEBUG ] Found procID " << v_procID << " which corresponds to VME1" << endl;
		#endif
		this->ProcessSubeventRawVME1(v_intLen, v_dataField);
		break;
	case 101:
		#ifdef PRINTDEBUGINFO
		cerr << "[DEBUG ] Found procID " << v_procID << " which corresponds to CAMAC" << endl;
		#endif
		this->ProcessSubeventRawCAMACmwpc(v_intLen, v_dataField);
		break;
	default:
		#ifdef PRINTDEBUGINFO
		cerr << "[WARN  ] Found procID " << v_procID << " which is unknown. Skipping subevent." << endl;
		#endif
		mNunknownWords += v_intLen;
		break;
	}
}

void UserProc::ProcessSubeventRaw(Int_t p_size, const Int_t* p_startAddress)
{
	for (Int_t v_cursor=0; v_cursor<p_size; /*no action here*/) // v_cursor is incremented inside
	{
		Int_t v_leftSize = p_size - v_cursor;
		Int_t v_footerPosition = 0; //TODO or better init with -1 ?

		switch (UserProc::CheckNextHeader(&p_startAddress[v_cursor])) {
		case MESYTEC:

			#ifdef PRINTDEBUGINFO
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &p_startAddress[v_cursor]) << "  ";
			cerr << "[" << v_cursor << "]\t" << "MESYTEC header" << endl;
			#endif

			v_footerPosition = UserProc::FindMESYTECfooter(v_leftSize, &p_startAddress[v_cursor]);
			if (v_footerPosition > -1) {

				#ifdef PRINTDEBUGINFO
				cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &p_startAddress[v_cursor+v_footerPosition]) << "  ";
				cerr << "[" << v_cursor+v_footerPosition << "]\t" << "MESYTEC footer";
				cerr << "\tshift=" << v_footerPosition << endl;
				#endif

				this->ProcessSubsubevent_MESYTEC(v_footerPosition+1, &p_startAddress[v_cursor]);

				v_cursor += (v_footerPosition + 1); // right after the footer
			} else {
				#ifdef PRINTDEBUGINFO
				cerr << "[WARN  ] MESYTEC footer not found!" << endl;
				#endif
				v_cursor++;
			}

			break;
		case CAEN:

			#ifdef PRINTDEBUGINFO
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &p_startAddress[v_cursor]) << "  ";
			cerr << "[" << v_cursor << "]\t" << "CAEN header" << endl;
			#endif

			v_footerPosition = UserProc::FindCAENfooter(v_leftSize, &p_startAddress[v_cursor]);
			if (v_footerPosition > -1) {

				#ifdef PRINTDEBUGINFO
				cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &p_startAddress[v_cursor+v_footerPosition]) << "  ";
				cerr << "[" << v_cursor+v_footerPosition << "]\t" << "CAEN footer";
				cerr << "\tshift=" << v_footerPosition << endl;
				#endif

				this->ProcessSubsubevent_CAEN(v_footerPosition+1, &p_startAddress[v_cursor]);

				v_cursor += (v_footerPosition + 1); // right after the footer
			} else {
				#ifdef PRINTDEBUGINFO
				cerr << "[WARN  ] CAEN footer not found!" << endl;
				#endif
				v_cursor++;
			}

			break;
		case AFFEAFFE:
			#ifdef PRINTDEBUGINFO
			cerr << "[DEBUG ] AFFEAFFE. Skipping one 32-bit word." << endl;
			#endif
			mNknownWords++;
			v_cursor++;
			break;
		case OTHER:
			#ifdef PRINTDEBUGINFO
			cerr << "[WARN  ] OTHER VENDOR. Skipping one 32-bit word." << endl;
			#endif
			mNunknownWords++;
			v_cursor++;
			break;
		}
	}
}

void UserProc::ProcessSubeventRawVME0(Int_t p_size, const Int_t* p_startAddress)
{
	#ifdef PRINTDEBUGINFO
	cerr << "[DEBUG ] Processing raw subevent from VME0 with size=" << p_size << endl;
	#endif
	this->ProcessSubeventRaw(p_size, p_startAddress);
}

void UserProc::ProcessSubeventRawVME1(Int_t p_size, const Int_t* p_startAddress)
{
	#ifdef PRINTDEBUGINFO
	cerr << "[DEBUG ] Processing raw subevent from VME1 with size=" << p_size << endl;
	#endif
	this->ProcessSubeventRaw(p_size, p_startAddress);
}

void UserProc::ProcessSubeventRawCAMAC(Int_t p_size, const Int_t* p_startAddress)
{
	#ifdef PRINTDEBUGINFO
	cerr << "[DEBUG ] Processing raw subevent from CAMAC with size=" << p_size << endl;
	//cerr << "\t" << "Skipping." << endl;
	#endif

	for (Int_t v_cursor=0; v_cursor<p_size; v_cursor++) {
		Int_t v_curWord = p_startAddress[v_cursor];
		Int_t v_geo = (v_curWord >> 27) & 0x1f; //TODO mask is unknown to me // 5 bits?
		fprintf(stderr, "%d: %08x\tgeo=%u\n", v_cursor, p_startAddress[v_cursor], v_geo);
	}
}

void UserProc::ProcessSubeventRawCAMACmwpc(Int_t p_size, const Int_t* p_startAddress)
{
	#ifdef PRINTDEBUGINFO
	cerr << "[DEBUG ] Processing raw subevent from CAMAC with size=" << p_size << endl;
	#endif

	// Currently we rely on the fact that p_size=16
	// We also rely that the words come in order from the blocks
	// with geo = 20, 21, 22, 23

	if (p_size != 16) {
		cerr << "[ERROR ] CAMAC subevent size != 16. Skipping." << endl;
		return;
	}

	mNknownWords += 16;

	Int_t v_curWord;
	Int_t v_geo;

	// HEADER
	v_curWord = p_startAddress[0];
	v_geo = (v_curWord >> 27) & 0x1f; //TODO mask is unknown to me // 5 bits?
	// DATA0
	v_curWord = p_startAddress[1];
	mCurrentOutputEvent->mCAMAC[0] = (v_curWord & 0xFFFF);
	// DATA1
	v_curWord = p_startAddress[2];
	mCurrentOutputEvent->mCAMAC[1] = (v_curWord & 0xFFFF);
	// FOOTER
	v_curWord = p_startAddress[3];
	v_geo = (v_curWord >> 27) & 0x1f; //TODO mask is unknown to me // 5 bits?

	// HEADER
	v_curWord = p_startAddress[4];
	v_geo = (v_curWord >> 27) & 0x1f; //TODO mask is unknown to me // 5 bits?
	// DATA2
	v_curWord = p_startAddress[5];
	mCurrentOutputEvent->mCAMAC[2] = (v_curWord & 0xFFFF);
	// DATA3
	v_curWord = p_startAddress[6];
	mCurrentOutputEvent->mCAMAC[3] = (v_curWord & 0xFFFF);
	// FOOTER
	v_curWord = p_startAddress[7];
	v_geo = (v_curWord >> 27) & 0x1f; //TODO mask is unknown to me // 5 bits?

	// HEADER
	v_curWord = p_startAddress[8+0];
	v_geo = (v_curWord >> 27) & 0x1f; //TODO mask is unknown to me // 5 bits?
	// DATA4
	v_curWord = p_startAddress[8+1];
	mCurrentOutputEvent->mCAMAC[4] = (v_curWord & 0xFFFF);
	// DATA5
	v_curWord = p_startAddress[8+2];
	mCurrentOutputEvent->mCAMAC[5] = (v_curWord & 0xFFFF);
	// FOOTER
	v_curWord = p_startAddress[8+3];
	v_geo = (v_curWord >> 27) & 0x1f; //TODO mask is unknown to me // 5 bits?

	// HEADER
	v_curWord = p_startAddress[8+4];
	v_geo = (v_curWord >> 27) & 0x1f; //TODO mask is unknown to me // 5 bits?
	// DATA6
	v_curWord = p_startAddress[8+5];
	mCurrentOutputEvent->mCAMAC[6] = (v_curWord & 0xFFFF);
	// DATA7
	v_curWord = p_startAddress[8+6];
	mCurrentOutputEvent->mCAMAC[7] = (v_curWord & 0xFFFF);
	// FOOTER
	v_curWord = p_startAddress[8+7];
	v_geo = (v_curWord >> 27) & 0x1f; //TODO mask is unknown to me // 5 bits?
}

void UserProc::ProcessSubsubevent_MESYTEC(Int_t p_size, const Int_t* p_startAddress)
{
	#ifdef PRINTDEBUGINFO
	cerr << "[DEBUG ] Processing MESYTEC subsubevent with size=" << p_size << endl;
	cerr << "         -----------------------------------------------------------" << endl;
	#endif

	for (Int_t v_cursor=0; v_cursor<p_size; v_cursor++) {

		Int_t tmp = p_startAddress[v_cursor];
		Int_t v_curWord = tmp;

		Int_t v_type = (v_curWord >> 30) & 0x3; // 2 bits

		Int_t v_module_id;
		Int_t v_channel;
		Int_t v_valueQA; // QDC and ADC
		Int_t v_valueT; // TDC

		switch (v_type) {
		case 1: // MESYTEC header
			mInsidePackage = true;
			v_module_id = (v_curWord >> 16) & 0xff; // 8 bits
			mNknownWords++;
			#ifdef PRINTDEBUGINFO
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << "[" << v_cursor << "]\t" << "MESYTEC header module_id=" << v_module_id << endl;
			#endif
			break;
		case 3: // MESYTEC footer
			mInsidePackage = false;
			mNknownWords++;
			#ifdef PRINTDEBUGINFO
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << "[" << v_cursor << "]\t" << "MESYTEC footer" << endl;
			#endif
			break;
		case 0: // MESYTEC data
			//TODO check
			v_channel = (v_curWord >> 16) & 0x1f; // 5 bits
			v_valueQA = v_curWord & 0x1fff; // 13 bits // ADC and QDC
			v_valueT = v_curWord & 0xffff; // 16 bits // TDC
			mNknownWords++;
			#ifdef PRINTDEBUGINFO
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << "[" << v_cursor << "]\t" << "MESYTEC data"
			     << "\tch=" << v_channel << "\tvalueQA=" << v_valueQA << "\tvalueT=" << v_valueT << endl;
			#endif
			if (!mInsidePackage) {
				cerr << "[ERROR ] MESYTEC data word found not between the header and the footer." << endl;
			}
			break;
		default:
			mNunknownWords++;
			#ifdef PRINTDEBUGINFO
			cerr << "[ERROR ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << "[" << v_cursor << "]\t" << "Unknown MESYTEC type" << endl;
			#endif
			break;
		} // end of switch
	} // end of for

	#ifdef PRINTDEBUGINFO
	cerr << "         -----------------------------------------------------------" << endl;
	#endif
}

void UserProc::ProcessSubsubevent_CAEN(Int_t p_size, const Int_t* p_startAddress)
{
	#ifdef PRINTDEBUGINFO
	cerr << "[DEBUG ] Processing CAEN subsubevent with size=" << p_size << endl;
	cerr << "         -----------------------------------------------------------" << endl;
	#endif

	for (Int_t v_cursor=0; v_cursor<p_size; v_cursor++) {

		Int_t tmp = p_startAddress[v_cursor];
		Int_t v_curWord = tmp;

		Int_t v_type = (v_curWord >> 24) & 0x7; // 3 bits
		Int_t v_geo = (v_curWord >> 27) & 0x1f; // 5 bits
		Int_t v_channel;
		Int_t v_value;

		switch (v_type) {
		case 2: // CAEN header
			mInsidePackage = true;
			mNknownWords++;
			#ifdef PRINTDEBUGINFO
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << "[" << v_cursor << "]\t" << "CAEN header" << "\ttype=" << v_type << "\tgeo=" << v_geo << endl;
			#endif
			break;
		case 4: // CAEN footer
			mInsidePackage = false;
			mNknownWords++;
			#ifdef PRINTDEBUGINFO
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << "[" << v_cursor << "]\t" << "CAEN footer" << "\ttype=" << v_type << "\tgeo=" << v_geo << endl;
			#endif
			break;
		case 6: // CAEN no valid data
			mNknownWords++;
			#ifdef PRINTDEBUGINFO
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << "[" << v_cursor << "]\t" << "CAEN no valid data" << "\ttype=" << v_type << "\tgeo=" << v_geo << endl;
			#endif
			break;
		case 0: // CAEN data
			v_channel = (v_curWord >> 16) & 0x1f; // 5 bits //TODO check
			v_value = v_curWord & 0xfff; // 12 bits //TODO check
			mNknownWords++;
			#ifdef PRINTDEBUGINFO
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << "[" << v_cursor << "]\t" << "CAEN data" << "\ttype=" << v_type << "\tgeo=" << v_geo
			     << " ch=" << v_channel << " value=" << v_value << endl;
			#endif
			if (!mInsidePackage) {
				cerr << "[ERROR ] CAEN data word found not between the header and the footer." << endl;
			}
			break;
		default:
			mNunknownWords++;
			#ifdef PRINTDEBUGINFO
			cerr << "[ERROR ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << "[" << v_cursor << "]\t" << "CAEN unknown" << "\ttype=" << v_type << "\tgeo=" << v_geo << endl;
			#endif
			break;
		} // end of switch
	} // end of for

	#ifdef PRINTDEBUGINFO
	cerr << "         -----------------------------------------------------------" << endl;
	#endif
}

/*static*/
enu_VENDOR UserProc::CheckNextHeader(const Int_t* p_startAddress)
{
	Int_t tmp = p_startAddress[0];
	Int_t v_curWord = tmp;
	////cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "\t";

	int v_mesytec_flag = (v_curWord >> 24) & 0xff; // 8 bits // = 0x40
	int v_caen_header = (v_curWord >> 24) & 0x7; // 3 bits // = 0x2

	if (v_mesytec_flag == 0x40) {
		#ifdef PRINTDEBUGINFO
		cerr << "[DEBUG ] Checking "
		     << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "\t";
		cerr << "Identified as MESYTEC block" << endl;
		#endif
		return MESYTEC;
	} else {
		if (v_caen_header == 2) { // 010 (binary)
			#ifdef PRINTDEBUGINFO
			cerr << "[DEBUG ] Checking "
			     << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "\t";
			cerr << "Identified as CAEN block" << endl;
			#endif
			return CAEN;
		} else {
			if (v_curWord == 0xaffeaffe) {
				#ifdef PRINTDEBUGINFO
				cerr << "[DEBUG ] Checking "
				     << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "\t";
				cerr << "Identified as AFFEAFFE block" << endl;
				#endif
				return AFFEAFFE;
			} else {
				#ifdef PRINTDEBUGINFO
				cerr << "[DEBUG ] Checking "
				     << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "\t";
				cerr << "Identified as UNKNOWN block" << endl;
				#endif
				return OTHER;
			}
		}
	}
}

/**
 * Return -1 if not found!
*/
/*static*/
Int_t UserProc::FindMESYTECfooter(Int_t p_maxSize, const Int_t* p_startAddress)
{
	for (Int_t v_cursor=0; v_cursor<p_maxSize; v_cursor++) {
		Int_t tmp = p_startAddress[v_cursor];
		Int_t v_curWord = tmp;
		Int_t v_type = (v_curWord >> 30) & 0x3; // 2 bits
		////fprintf(stderr, "%08x type=%d\n", v_curWord, v_type);
		if (v_type == 3) {
			// type==3 - MESYTEC footer
			return v_cursor;
		}
	}
	return -1;
}

/**
 * Return -1 if not found!
*/
/*static*/
Int_t UserProc::FindCAENfooter(Int_t p_maxSize, const Int_t* p_startAddress)
{
	for (Int_t v_cursor=0; v_cursor<p_maxSize; v_cursor++) {
		Int_t tmp = p_startAddress[v_cursor];
		Int_t v_curWord = tmp;
		Int_t v_type = (v_curWord >> 24) & 0x7; // 3 bits
		////fprintf(stderr, "%08x type=%d\n", v_curWord, v_type);
		if (v_type == 4) {
			// type==4 - CAEN footer
			return v_cursor;
		}
	}
	return -1;
}

/*static*/
void UserProc::DumpEventHeader(/*const*/ TGo4MbsEvent* p_event)
{
	s_filhe* head = p_event->GetMbsSourceHeader();
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

/*static*/
void UserProc::DumpSubeventHeader(/*const*/ TGo4MbsSubEvent* p_subevent)
{
	Int_t v_dLen = p_subevent->GetDlen();
	Short_t v_type = p_subevent->GetType();
	Char_t v_subcrate = p_subevent->GetSubcrate();
	Char_t v_control = p_subevent->GetControl();
	Short_t v_procID = p_subevent->GetProcid();
	Int_t v_byteLen = p_subevent->GetByteLen();

	Int_t v_intLen = p_subevent->GetIntLen(); // Get the size of the current subevent
	Int_t* v_dataField = p_subevent->GetDataField(); // Get the pointer to the data of the subevent

	cerr << "[DEBUG ] Subevent header: "
	     << "dLen=" << v_dLen << "\t"
	     << "type=" << v_type << "\t"
	     << "subcrate=" << (Int_t)v_subcrate << "\t"
	     << "control=" << (Int_t)v_control << "\t"
	     << "procID=" << v_procID << "\t"
	     << "intLen=" << v_intLen << "\t"
	     << "byteLen=" << v_byteLen
	     << endl;
}

/*static*/
void UserProc::DumpSubeventData(Int_t p_size, const Int_t* p_startAddress)
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

/*static*/
void UserProc::DumpSubeventData2(Int_t p_size, const Int_t* p_startAddress)
{
	cerr << "SubEvent size = " << p_size << endl;
	for (Int_t i=0; i<p_size; i++) {
		Int_t v_curWord = p_startAddress[i];
		fprintf(stderr, "%02x", ((v_curWord >> 24) & 0xff));
		fprintf(stderr, "%02x", ((v_curWord >> 16) & 0xff));
		fprintf(stderr, "%02x", ((v_curWord >> 8) & 0xff));
		fprintf(stderr, "%02x", ((v_curWord >> 0) & 0xff));
		fprintf(stderr, " ");
		if ((i+1)%8 == 0) { cerr << endl; }
	}
	cerr << endl;
}

ClassImp(UserProc)
