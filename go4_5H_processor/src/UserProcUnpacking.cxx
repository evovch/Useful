#include "UserProcUnpacking.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// ROOT
#include <TClonesArray.h>

// Go4
#include <TGo4EventElement.h>
#include <TGo4MbsEvent.h>
#include <TGo4MbsSubEvent.h>

// Project
#include "UserEventUnpacking.h"
#include "UserHistosUnpacking.h"

/*static*/
RawMessage UserProcUnpacking::mCurMessage;

/*static*/
bool UserProcUnpacking::mInsidePackage = false;

/**
  Uncomment this if you want to see all the debug information.
  This allows you to analyze the raw bytes and bits by your eyes.
  This option produces A LOT OF DATA - run your analysis with a
  small number of events (~10-100)
*/
#define PRINTDEBUGINFO

//TODO test
#define DORESET

UserProcUnpacking::UserProcUnpacking(const char* name) :
	TGo4EventProcessor(name),
	mEventCounter(0),
	mSubEventCounter(0),
	mHeadersWords(0),
	mNknownWords(0),
	mNunknownWords(0)
{
	mHistoMan = new UserHistosUnpacking();
	mFileSummary = fopen("summary.txt", "w");
	if (mFileSummary == NULL) {
		//TODO error
	}
}

UserProcUnpacking::~UserProcUnpacking()
{
	if (mHistoMan) delete mHistoMan;
	if (mFileSummary != NULL) {
		fclose(mFileSummary);
	}
}

Bool_t UserProcUnpacking::BuildEvent(TGo4EventElement* p_dest)
{
	Bool_t v_isValid = kFALSE;
	UserEventUnpacking* v_outputEvent = (UserEventUnpacking*)p_dest;

	TGo4MbsEvent* v_input = (TGo4MbsEvent*)GetInputEvent();
	if (v_input == NULL)
	{
		cerr << "UserProcUnpacking::BuildEvent(): no input event!" << endl;
		v_outputEvent->SetValid(v_isValid);
		return v_isValid;
	}
	v_isValid = kTRUE;

	#ifdef PRINTDEBUGINFO
	cerr << "[DEBUG ] " << "UserProcUnpacking: Event " << mEventCounter
	     << " ==========================================================================================================="
	     << endl;
	#endif

	mCurrentOutputEvent = v_outputEvent;

	// Clear the output event!!!
	//TODO check that this is not done by the framework
	// Seems that indeed this is done by the framework
	//mCurrentOutputEvent->Clear();

	//TODO do the processing here

	this->ProcessEventHeader(v_input);

	// Set the internal iterator of the subevent array to the beginning of the array.
	// I have no idea why do we have to do this...
	v_input->ResetIterator();

	TGo4MbsSubEvent* v_pSubevent; // Current subevent
	UInt_t v_subEventCounter = 0; // Counter of subevents within the current event
	while ((v_pSubevent = v_input->NextSubEvent()) != NULL)
	{
		#ifdef PRINTDEBUGINFO
		cerr << "[DEBUG ] " << "UserProcUnpacking: SubEvent " << v_subEventCounter << " (global subevent counter = " << mSubEventCounter << ")"
		     << " -----------------------------------------------------------------" << endl;
		#endif

		this->ProcessSubevent(v_pSubevent);

		v_subEventCounter++; // local subevent counter - within current event
		mSubEventCounter++; // global subevent counter - total
	}

	v_outputEvent->SetValid(v_isValid);

	cerr << "[DEBUG ] " << "End of event " << mEventCounter << ".\t"
	     << "Total subevents: " << mSubEventCounter << ".\t"
	     << "Headers' words: " << mHeadersWords << ",\t"
	     << "known words: " << mNknownWords << ",\t"
	     << "unknown words: " << mNunknownWords << ",\t"
	     << "total: " << (mHeadersWords+mNknownWords+mNunknownWords)*sizeof(Int_t) << " bytes."
	     << endl;

	if (mFileSummary != NULL) {
		fprintf(mFileSummary,
		        "End of event %ld.\tTotal subevents: %ld.\tHeaders' words: %ld,\tknown words: %ld,\tunknown words: %ld,\ttotal: %ld bytes.\n",
		        mEventCounter, mSubEventCounter, mHeadersWords, mNknownWords, mNunknownWords,
		        (mHeadersWords+mNknownWords+mNunknownWords)*sizeof(Int_t));
	}

	this->FinishEvent();

	mEventCounter++;

	return v_isValid;
}

void UserProcUnpacking::ProcessEventHeader(TGo4MbsEvent* p_event)
{
	mCurMessage.mEventType = p_event->GetType();
	mCurMessage.mEventSubtype = p_event->GetSubtype();
	mCurMessage.mEventDummy = p_event->GetDummy();
	mCurMessage.mEventTrigger = p_event->GetTrigger();
	mCurMessage.mEventCount = p_event->GetCount();

	#ifdef PRINTDEBUGINFO
	cerr << "[DEBUG ] Event header:" << "\t"
	     << "type="    << (Int_t)mCurMessage.mEventType << "\t"
	     << "subtype=" << (Int_t)mCurMessage.mEventSubtype << "\t"
	     << "dummy="   << (Int_t)mCurMessage.mEventDummy << "\t"
	     << "trigger=" << (Int_t)mCurMessage.mEventTrigger << "\t"
	     << "count="   << (Int_t)mCurMessage.mEventCount
	     << endl;
	////UserProcUnpacking::DumpEventHeader(p_event);
	#endif

	//TODO I did not actually find the corrent event header length
	mHeadersWords += 4;
}

void UserProcUnpacking::FinishEvent()
{
	#ifdef DORESET
	mCurMessage.mEventType = -1;
	mCurMessage.mEventSubtype = -1;
	mCurMessage.mEventDummy = -1;
	mCurMessage.mEventTrigger = -1;
	mCurMessage.mEventCount = -1;
	#endif // DORESET
}

void UserProcUnpacking::ProcessSubeventHeader(TGo4MbsSubEvent* p_subevent)
{
	mCurMessage.mSubeventDlen = p_subevent->GetDlen();
	mCurMessage.mSubeventType = p_subevent->GetType();
	mCurMessage.mSubeventSubcrate = p_subevent->GetSubcrate();
	mCurMessage.mSubeventControl = p_subevent->GetControl();
	mCurMessage.mSubeventFullID = p_subevent->GetFullId();
	mCurMessage.mSubeventProcID = p_subevent->GetProcid();

	#ifdef PRINTDEBUGINFO
	cerr << "[DEBUG ] Subevent header:" << "\t"
	     << "dlen="     << (Int_t)mCurMessage.mSubeventDlen << "\t"
	     << "type="     << (Int_t)mCurMessage.mSubeventType << "\t"
	     << "subcrate=" << (Int_t)mCurMessage.mSubeventSubcrate << "\t"
	     << "control="  << (Int_t)mCurMessage.mSubeventControl << "\t"
	     << "fullid="   << (Int_t)mCurMessage.mSubeventFullID << "\t"
	     << "procid="   << (Int_t)mCurMessage.mSubeventProcID
	     << endl;
	////UserProcUnpacking::DumpSubeventHeader(p_subevent);
	#endif

	//TODO I did not actually find the corrent subevent header length
	mHeadersWords += 3;
}

void UserProcUnpacking::FinishSubevent()
{
	#ifdef DORESET
	mCurMessage.mSubeventDlen = -1;
	mCurMessage.mSubeventType = -1;
	mCurMessage.mSubeventSubcrate = -1;
	mCurMessage.mSubeventControl = -1;
	mCurMessage.mSubeventFullID = -1;
	mCurMessage.mSubeventProcID = -1;
	#endif // DORESET
}

void UserProcUnpacking::ProcessSubevent(TGo4MbsSubEvent* p_subevent)
{
	this->ProcessSubeventHeader(p_subevent);

	//// Extract only what is needed to go deeper

	Short_t v_procID = p_subevent->GetProcid();
	//// Get the size of the current subevent data payload in words as integer
	Int_t v_intLen = p_subevent->GetIntLen();
	//// Get the pointer to the data of the subevent
	Int_t* v_dataField = p_subevent->GetDataField();

	#ifdef PRINTDEBUGINFO
	UserProcUnpacking::DumpSubeventData2(v_intLen, v_dataField);
	#endif

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

	this->FinishSubevent();
}

void UserProcUnpacking::ProcessSubeventRaw(Int_t p_size, const Int_t* p_startAddress)
{
	for (Int_t v_cursor=0; v_cursor<p_size; /*no action here*/) // v_cursor is incremented inside
	{
		Int_t v_leftSize = p_size - v_cursor;
		Int_t v_footerPosition = 0; //TODO or better init with -1 ?
		Int_t v_footerCounter = -1;

		switch (UserProcUnpacking::CheckNextHeader(&p_startAddress[v_cursor])) {
		case support::enu_VENDOR::MESYTEC:

			#ifdef PRINTDEBUGINFO
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &p_startAddress[v_cursor]) << "  ";
			cerr << "[" << v_cursor << "]\t" << "MESYTEC header" << endl;
			#endif

			//// Here we look forward and search for the footer.
			//// Not only we count the number of words in the subsubevent block but also
			//// we extract the counter from the footer and use it for all the data words
			//// of this subsubevent
			v_footerPosition = UserProcUnpacking::FindMESYTECfooter(v_leftSize, &p_startAddress[v_cursor], &v_footerCounter);
			mCurMessage.mSubsubeventFooterCounter = v_footerCounter;

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
		case support::enu_VENDOR::CAEN:

			#ifdef PRINTDEBUGINFO
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &p_startAddress[v_cursor]) << "  ";
			cerr << "[" << v_cursor << "]\t" << "CAEN header" << endl;
			#endif

			//// Here we look forward and search for the footer.
			//// Not only we count the number of words in the subsubevent block but also
			//// we extract the counter from the footer and use it for all the data words
			//// of this subsubevent
			v_footerPosition = UserProcUnpacking::FindCAENfooter(v_leftSize, &p_startAddress[v_cursor], &v_footerCounter);
			mCurMessage.mSubsubeventFooterCounter = v_footerCounter;

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
		case support::enu_VENDOR::AFFEAFFE:
			#ifdef PRINTDEBUGINFO
			cerr << "[DEBUG ] AFFEAFFE. Skipping one 32-bit word." << endl;
			#endif
			mNknownWords++;
			v_cursor++;
			break;
		case support::enu_VENDOR::OTHER:
			#ifdef PRINTDEBUGINFO
			cerr << "[WARN  ] OTHER VENDOR. Skipping one 32-bit word." << endl;
			#endif
			mNunknownWords++;
			v_cursor++;
			break;
		//// default section has no meaning here because OTHER already counts that
		}
	}
}

void UserProcUnpacking::ProcessSubeventRawVME0(Int_t p_size, const Int_t* p_startAddress)
{
	#ifdef PRINTDEBUGINFO
	cerr << "[DEBUG ] Processing raw subevent from VME0 with size=" << p_size << endl;
	#endif
	this->ProcessSubeventRaw(p_size, p_startAddress);
}

void UserProcUnpacking::ProcessSubeventRawVME1(Int_t p_size, const Int_t* p_startAddress)
{
	#ifdef PRINTDEBUGINFO
	cerr << "[DEBUG ] Processing raw subevent from VME1 with size=" << p_size << endl;
	#endif
	this->ProcessSubeventRaw(p_size, p_startAddress);
}

/*void UserProcUnpacking::ProcessSubeventRawCAMAC(Int_t p_size, const Int_t* p_startAddress)
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
}*/

void UserProcUnpacking::ProcessSubeventRawCAMACmwpc(Int_t p_size, const Int_t* p_startAddress)
{
	#ifdef PRINTDEBUGINFO
	cerr << "[DEBUG ] Processing raw subevent from CAMAC with size=" << p_size << " as from MWPC." << endl;
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

	#ifdef PRINTDEBUGINFO
	Short_t v_subword;
	cerr << "         -----------------------------------------------------------" << endl;
	for (UInt_t v_cursor=0; v_cursor<p_size; v_cursor+=4)
	{
		v_curWord = p_startAddress[v_cursor+0];
		cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  "
		     << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
		cerr << "[" << v_cursor+0 << "]\t" << "CAMAC MWPC header geo=" << ((v_curWord >> 27) & 0x1f);
		cerr << endl;

		v_curWord = p_startAddress[v_cursor+1];
		v_subword = v_curWord & 0xFFFF;
		cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  "
		     << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
		cerr << "[" << v_cursor+1 << "]\t" << "CAMAC MWPC data "
		     << support::GetBinaryRepresentation(sizeof(Short_t), &v_subword)
		     << endl;

		v_curWord = p_startAddress[v_cursor+2];
		v_subword = v_curWord & 0xFFFF;
		cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  "
		     << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
		cerr << "[" << v_cursor+2 << "]\t" << "CAMAC MWPC data "
		     << support::GetBinaryRepresentation(sizeof(Short_t), &v_subword)
		     << endl;

		v_curWord = p_startAddress[v_cursor+3];
		cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  "
		     << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
		cerr << "[" << v_cursor+3 << "]\t" << "CAMAC MWPC footer geo=" << ((v_curWord >> 27) & 0x1f);
		cerr << endl;
	}
	cerr << "         -----------------------------------------------------------" << endl;
	#endif // PRINTDEBUGINFO
}

void UserProcUnpacking::ProcessSubsubevent_MESYTEC(Int_t p_size, const Int_t* p_startAddress)
{
	#ifdef PRINTDEBUGINFO
	cerr << "[DEBUG ] Processing MESYTEC subsubevent with size=" << p_size << endl;
	cerr << "         -----------------------------------------------------------" << endl;
	#endif

	mCurMessage.mSubsubeventVendor = (Char_t)support::enu_VENDOR::MESYTEC; // MESYTEC=1 //TODO explicit cast?

	for (Int_t v_cursor=0; v_cursor<p_size; v_cursor++) {

		Int_t tmp = p_startAddress[v_cursor];
		Int_t v_curWord = tmp;

		// Common for all types of messages
		Int_t v_type = (v_curWord >> 30) & 0x3; // 2 bits

		Int_t v_module_id = -1;
		Int_t v_subsubeventSize = -1;
		Int_t v_eventCounter = -1; // event counter or time stamp
		Int_t v_channel = -1;
		Int_t v_valueQA = -1; // QDC and ADC
		Int_t v_valueT = -1; // TDC

		switch (v_type) {
		case 1: // MESYTEC header
			mInsidePackage = true;
			v_module_id = (v_curWord >> 16) & 0xff; // 8 bits
			v_subsubeventSize = v_curWord & 0x3ff; // 10 bits
			mNknownWords++;
			#ifdef PRINTDEBUGINFO
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << "[" << v_cursor << "]\t" << "MESYTEC header"
			     << "\ttype=" << v_type
			     << "\tmodule_id=" << v_module_id
			     << "\tsize=" << v_subsubeventSize
			     << endl;
			#endif

			mCurMessage.mSubsubeventModule = v_module_id;

			break;
		case 3: // MESYTEC footer
			mInsidePackage = false;
			v_eventCounter = UserProcUnpacking::ExtractCounterFromMESYTECfooter(v_curWord);
			mNknownWords++;
			#ifdef PRINTDEBUGINFO
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << "[" << v_cursor << "]\t" << "MESYTEC footer"
			     << "\ttype=" << v_type
			     << "\teventCounter=" << v_eventCounter
			     << endl;
			#endif

			//// IN PRINCIPLE, WE COULD WRITE THIS OUT
			mCurMessage.mSubsubeventFooterCounter = v_eventCounter;

			//// WRITE OUT HERE?

			//// Also this value should be reset somewhere.
			//// In principle one could reset it immediately right after writeout
			#ifdef DORESET
			mCurMessage.mSubsubeventFooterCounter = -1;
			#endif // DORESET

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
			     << "\ttype=" << v_type
			     << "\tch=" << v_channel
			     << "\tvalueQA=" << v_valueQA
			     << "\tvalueT=" << v_valueT
			     << endl;
			#endif

			// HERE WE WRITE OUT
			mCurMessage.mChannel = v_channel;
			mCurMessage.mValueQA = v_valueQA;
			mCurMessage.mValueT = v_valueT;

			this->PushOutputRawMessage();

			#ifdef DORESET
			mCurMessage.mChannel = -1;
			mCurMessage.mValueQA = -1;
			mCurMessage.mValueT = -1;
			#endif // DORESET

			if (!mInsidePackage) {
				cerr << "[ERROR ] MESYTEC data word found not between the header and the footer." << endl;
			}
			break;
		default:
			mNunknownWords++;
			#ifdef PRINTDEBUGINFO
			cerr << "[ERROR ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << "[" << v_cursor << "]\t" << "MESYTEC unknown"
			     << "\ttype=" << v_type << endl;
			#endif
			break;
		} // end of switch
	} // end of for

	//// Don't forget to reset the values defined in the subsubevent header
	mCurMessage.mSubsubeventModule = -1; // Currently we reset module and geo independent of the precompiler variable
	#ifdef DORESET
	mCurMessage.mSubsubeventVendor = -1;
	#endif // DORESET

	#ifdef PRINTDEBUGINFO
	cerr << "         -----------------------------------------------------------" << endl;
	#endif
}

void UserProcUnpacking::ProcessSubsubevent_CAEN(Int_t p_size, const Int_t* p_startAddress)
{
	#ifdef PRINTDEBUGINFO
	cerr << "[DEBUG ] Processing CAEN subsubevent with size=" << p_size << endl;
	cerr << "         -----------------------------------------------------------" << endl;
	#endif

	mCurMessage.mSubsubeventVendor = (Char_t)support::enu_VENDOR::CAEN; // CAEN=2 //TODO explicit cast?

	for (Int_t v_cursor=0; v_cursor<p_size; v_cursor++) {

		Int_t tmp = p_startAddress[v_cursor];
		Int_t v_curWord = tmp;

		// Common for all types of messages
		Int_t v_type = (v_curWord >> 24) & 0x7; // 3 bits
		Int_t v_geo = (v_curWord >> 27) & 0x1f; // 5 bits

		mCurMessage.mSubsubeventGeo = v_geo;

		Int_t v_eventCounter = -1;
		Int_t v_channel = -1;
		Int_t v_value = -1;

		switch (v_type) {
		case 2: // CAEN header
			mInsidePackage = true;
			mNknownWords++;
			#ifdef PRINTDEBUGINFO
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << "[" << v_cursor << "]\t" << "CAEN header"
			     << "\ttype=" << v_type
			     << "\tgeo=" << v_geo
			     << endl;
			#endif
			break;
		case 4: // CAEN footer
			mInsidePackage = false;
			v_eventCounter = UserProcUnpacking::ExtractCounterFromCAENfooter(v_curWord);
			mNknownWords++;
			#ifdef PRINTDEBUGINFO
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << "[" << v_cursor << "]\t" << "CAEN footer"
			     << "\ttype=" << v_type
			     << "\tgeo=" << v_geo
			     << "\teventCounter=" << v_eventCounter
			     << endl;
			#endif

			//// IN PRINCIPLE, WE COULD WRITE THIS OUT
			mCurMessage.mSubsubeventFooterCounter = v_eventCounter;

			//// WRITE OUT HERE?

			//// Also this value should be reset somewhere.
			//// In principle one could reset it immediately right after writeout
			#ifdef DORESET
			mCurMessage.mSubsubeventFooterCounter = -1;
			#endif // DORESET

			break;
		case 6: // CAEN no valid data
			mNknownWords++;
			#ifdef PRINTDEBUGINFO
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << "[" << v_cursor << "]\t" << "CAEN no valid data"
			     << "\ttype=" << v_type
			     << "\tgeo=" << v_geo
			     << endl;
			#endif
			break;
		case 0: // CAEN data
			v_channel = (v_curWord >> 16) & 0x1f; // 5 bits //TODO check
			v_value = v_curWord & 0xfff; // 12 bits //TODO check
			mNknownWords++;
			#ifdef PRINTDEBUGINFO
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << "[" << v_cursor << "]\t" << "CAEN data"
			     << "\ttype=" << v_type
			     << "\tgeo=" << v_geo
			     << "\tch=" << v_channel
			     << "\tvalue=" << v_value
			     << endl;
			#endif

			//// HERE WE WRITE OUT
			mCurMessage.mChannel = v_channel;
			mCurMessage.mValueQA = v_value;
			mCurMessage.mValueT = v_value;

			this->PushOutputRawMessage();

			#ifdef DORESET
			mCurMessage.mChannel = -1;
			mCurMessage.mValueQA = -1;
			mCurMessage.mValueT = -1;
			#endif // DORESET

			if (!mInsidePackage) {
				cerr << "[ERROR ] CAEN data word found not between the header and the footer." << endl;
			}
			break;
		default:
			mNunknownWords++;
			#ifdef PRINTDEBUGINFO
			cerr << "[ERROR ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << "[" << v_cursor << "]\t" << "CAEN unknown"
			     << "\ttype=" << v_type
			     << "\tgeo=" << v_geo
			     << endl;
			#endif
			break;
		} // end of switch
	} // end of for

	//// Don't forget to reset the values defined in the subsubevent header
	//// For this particular case, this geo is read from all types of words
	mCurMessage.mSubsubeventGeo = -1; // Currently we reset module and geo independent of the precompiler variable
	#ifdef DORESET
	mCurMessage.mSubsubeventVendor = -1;
	#endif // DORESET

	#ifdef PRINTDEBUGINFO
	cerr << "         -----------------------------------------------------------" << endl;
	#endif
}

void UserProcUnpacking::PushOutputRawMessage(void)
{
	TClonesArray& v_coll = *(mCurrentOutputEvent->mRawMessages);
	Int_t v_size = v_coll.GetEntriesFast();
	//// The RawMessage class copy constructor is invoked here (I've checked, it is!)
	new(v_coll[v_size])RawMessage(mCurMessage);
}

// static methods =================================================================================

/*static*/
support::enu_VENDOR UserProcUnpacking::CheckNextHeader(const Int_t* p_startAddress)
{
	Int_t tmp = p_startAddress[0];
	Int_t v_curWord = tmp;
	////cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "\t";

	Int_t v_mesytec_flag = (v_curWord >> 24) & 0xff; // 8 bits // = 0x40
	Int_t v_caen_header = (v_curWord >> 24) & 0x7; // 3 bits // = 0x2

	if (v_mesytec_flag == 0x40) {
		#ifdef PRINTDEBUGINFO
		cerr << "[DEBUG ] Checking "
		     << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "\t";
		cerr << "Identified as MESYTEC block" << endl;
		#endif
		return support::enu_VENDOR::MESYTEC;
	} else {
		if (v_caen_header == 2) { // 010 (binary)
			#ifdef PRINTDEBUGINFO
			cerr << "[DEBUG ] Checking "
			     << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "\t";
			cerr << "Identified as CAEN block" << endl;
			#endif
			return support::enu_VENDOR::CAEN;
		} else {
			if (v_curWord == 0xaffeaffe) {
				#ifdef PRINTDEBUGINFO
				cerr << "[DEBUG ] Checking "
				     << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "\t";
				cerr << "Identified as AFFEAFFE block" << endl;
				#endif
				return support::enu_VENDOR::AFFEAFFE;
			} else {
				#ifdef PRINTDEBUGINFO
				cerr << "[DEBUG ] Checking "
				     << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "\t"
				     << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "\t";
				cerr << "Identified as UNKNOWN block" << endl;
				#endif
				return support::enu_VENDOR::OTHER;
			}
		}
	}
}

/*static*/
Int_t UserProcUnpacking::FindMESYTECfooter(Int_t p_maxSize, const Int_t* p_startAddress, Int_t* o_counter)
{
	for (Int_t v_cursor=0; v_cursor<p_maxSize; v_cursor++) {
		Int_t tmp = p_startAddress[v_cursor];
		Int_t v_curWord = tmp;
		Int_t v_type = (v_curWord >> 30) & 0x3; // 2 bits
		////fprintf(stderr, "%08x type=%d\n", v_curWord, v_type);
		if (v_type == 3) {
			// type==3 - MESYTEC footer
			*o_counter = UserProcUnpacking::ExtractCounterFromMESYTECfooter(v_curWord);
			return v_cursor;
		}
	}
	return -1;
}

/*static*/
Int_t UserProcUnpacking::FindCAENfooter(Int_t p_maxSize, const Int_t* p_startAddress, Int_t* o_counter)
{
	for (Int_t v_cursor=0; v_cursor<p_maxSize; v_cursor++) {
		Int_t tmp = p_startAddress[v_cursor];
		Int_t v_curWord = tmp;
		Int_t v_type = (v_curWord >> 24) & 0x7; // 3 bits
		////fprintf(stderr, "%08x type=%d\n", v_curWord, v_type);
		if (v_type == 4) {
			// type==4 - CAEN footer
			*o_counter = UserProcUnpacking::ExtractCounterFromCAENfooter(v_curWord);
			return v_cursor;
		}
	}
	return -1;
}

/*static*/
void UserProcUnpacking::DumpEventHeader(/*const*/ TGo4MbsEvent* p_event)
{
	//TODO chagne to 'normal' interface methods
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
void UserProcUnpacking::DumpSubeventHeader(/*const*/ TGo4MbsSubEvent* p_subevent)
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
void UserProcUnpacking::DumpSubeventData(Int_t p_size, const Int_t* p_startAddress)
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
void UserProcUnpacking::DumpSubeventData2(Int_t p_size, const Int_t* p_startAddress)
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

ClassImp(UserProcUnpacking)