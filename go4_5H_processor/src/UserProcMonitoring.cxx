#include "UserProcMonitoring.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// ROOT
#include <TClonesArray.h>
#include <TH1.h>

// Project
#include "UserEventUnpacking.h"
#include "UserEventMonitoring.h"
#include "UserHistosMonitoring.h"
#include "Support.h"
#include "data/RawMessage.h"

/**
  Uncomment this if you want to see all the debug information.
  This allows you to analyze the raw bytes and bits by your eyes.
  This option produces A LOT OF DATA - run your analysis with a
  small number of events (~10-100)
*/
#define PRINTDEBUGINFO

UserProcMonitoring::UserProcMonitoring(const char* name) :
	TGo4EventProcessor(name),
	mEventCounter(0)
{
	mHistoMan = new UserHistosMonitoring();
}

UserProcMonitoring::~UserProcMonitoring()
{
	if (mHistoMan) delete mHistoMan;
}

Bool_t UserProcMonitoring::BuildEvent(TGo4EventElement* p_dest)
{
	Bool_t v_isValid = kFALSE;
	UserEventMonitoring* v_outputEvent = (UserEventMonitoring*)p_dest;

	UserEventUnpacking* v_input = (UserEventUnpacking*)GetInputEvent();
	if (v_input == NULL)
	{
		cerr << "UserProcMonitoring::BuildEvent(): no input event!" << endl;
		v_outputEvent->SetValid(v_isValid);
		return v_isValid;
	}
	v_isValid = kTRUE;

	#ifdef PRINTDEBUGINFO
	cerr << "[DEBUG ] " << "UserProcMonitoring: Event " << mEventCounter
	     << " ======================================================================================================"
	     << endl;
	#endif

	mCurrentOutputEvent = v_outputEvent;

	// Clear the output event!!!
	//TODO check that this is not done by the framework
	// Seems that indeed this is done by the framework
	//mCurrentOutputEvent->Clear();

	//TODO do the processing here

	UInt_t v_messCounter = 0;
	TIter next(v_input->mRawMessages);
	while (RawMessage* v_curMessage = (RawMessage*)next())
	{

		#ifdef PRINTDEBUGINFO
		cerr << v_messCounter << ": ";
		v_curMessage->Dump(false);
		cerr << "\t\t";
		#endif

		support::enu_VENDOR v_messVendor = support::VendorFromChar(v_curMessage->mSubsubeventVendor);

		switch (v_messVendor) {
		case support::enu_VENDOR::MESYTEC:
			this->ProcessMESYTECmessage(v_curMessage);
			break;
		case support::enu_VENDOR::CAEN:
			this->ProcessCAENmessage(v_curMessage);
			break;
		default:
			//// All fine
			break;
		}; // end of switch

		v_messCounter++;
	} // end of while

	//TODO process CAMAC MWPC words here

	this->ProcessCAMACmwpcWords(v_input);

	// --------------------------

	v_outputEvent->SetValid(v_isValid);

	mEventCounter++;

	return v_isValid;
}

void UserProcMonitoring::ProcessMESYTECmessage(const RawMessage* p_message)
{
	switch (p_message->mSubeventProcID) {
	case 100: // VME0
		this->ProcessMESYTECmessageVME0(p_message);
		break;
	case 200: // VME1
		this->ProcessMESYTECmessageVME1(p_message);
		break;
	default:
		#ifdef PRINTDEBUGINFO
		cerr << "unidentified MESYTEC message: "; // << endl;
		p_message->ExtDump();
		#endif
		break;
	};
}

void UserProcMonitoring::ProcessMESYTECmessageVME0(const RawMessage* p_message)
{
	Int_t v_ch = p_message->mChannel;

	switch (p_message->mSubsubeventModule) {
	case 5:  // MESYTEC mADC: SQX left
		if (v_ch<32) {
			#ifdef PRINTDEBUGINFO
			cerr << "SQX_L[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->SQX_L[v_ch] = p_message->mValueQA;
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=32) SQX_L[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	case 6:  // MESYTEC mADC: SQX right
		if (v_ch<32) {
			#ifdef PRINTDEBUGINFO
			cerr << "SQX_R[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->SQX_R[v_ch] = p_message->mValueQA;
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=32) SQX_R[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	case 7:  // MESYTEC mADC: SQY left and right
		if (v_ch<16) {
			#ifdef PRINTDEBUGINFO
			cerr << "SQY_L[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->SQY_L[v_ch] = p_message->mValueQA;
		} else if (v_ch<32) {
			#ifdef PRINTDEBUGINFO
			cerr << "SQY_R[" << v_ch-16 << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->SQY_R[v_ch-16] = p_message->mValueQA;
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=32) SQY_(L/R)[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	case 8:  // MESYTEC mADC: CsI left and right
		if (v_ch<16) {
			#ifdef PRINTDEBUGINFO
			cerr << "CsI_L[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->CsI_L[v_ch] = p_message->mValueQA;
		} else if (v_ch<32) {
			#ifdef PRINTDEBUGINFO
			cerr << "CsI_R[" << v_ch-16 << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->CsI_R[v_ch-16] = p_message->mValueQA;
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=32) CsI_(L/R)[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	case 9:  // MESYTEC mADC: SQ20
		if (v_ch<16) {
			#ifdef PRINTDEBUGINFO
			cerr << "SQ20[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->SQ20[v_ch] = p_message->mValueQA;
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=16) SQ20[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	case 10: // MESYTEC mQDC: F3, F5, F6
		if (v_ch<4) {
			#ifdef PRINTDEBUGINFO
			cerr << "F3[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->F3[v_ch] = p_message->mValueQA;
			//if (p_message->mValueQA > 0) { mHistoMan->d1F3[v_ch]->Fill(p_message->mValueQA); } //TODO which value to use?
		} else if (v_ch<8) {
			#ifdef PRINTDEBUGINFO
			cerr << "F5[" << v_ch-4 << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->F5[v_ch-4] = p_message->mValueQA;
			//if (p_message->mValueQA > 0) { mHistoMan->d1F5[v_ch-4]->Fill(p_message->mValueQA); } //TODO which value to use?
		} else if (v_ch<12) {
			#ifdef PRINTDEBUGINFO
			cerr << "F6[" << v_ch-8 << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->F6[v_ch-8] = p_message->mValueQA;
			//if (p_message->mValueQA > 0) { mHistoMan->d1F6[v_ch-8]->Fill(p_message->mValueQA); } //TODO which value to use?
		} else if (v_ch<16) {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=12 && ch<16) [" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=16) [" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	case 11: // MESYTEC mTDC: tF3, tF5, tF6, tMWPC - time
		//TODO T-bit analysis???
		if (v_ch<4) {
			#ifdef PRINTDEBUGINFO
			cerr << "tF3[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->tF3[v_ch] = p_message->mValueT;
			//if (p_message->mValueT > 0) { mHistoMan->d1tF3[v_ch]->Fill(p_message->mValueT); } //TODO which value to use?
		} else if (v_ch<8) {
			#ifdef PRINTDEBUGINFO
			cerr << "tF5[" << v_ch-4 << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->tF5[v_ch-4] = p_message->mValueT;
			//if (p_message->mValueT > 0) { mHistoMan->d1tF5[v_ch-4]->Fill(p_message->mValueT); } //TODO which value to use?
		} else if (v_ch<12) {
			#ifdef PRINTDEBUGINFO
			cerr << "tF6[" << v_ch-8 << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->tF6[v_ch-8] = p_message->mValueT;
			//if (p_message->mValueT > 0) { mHistoMan->d1tF6[v_ch-8]->Fill(p_message->mValueT); } //TODO which value to use?
		} else if (v_ch<16) {
			#ifdef PRINTDEBUGINFO
			cerr << "tMWPC[" << v_ch-12 << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->tMWPC[v_ch-12] = p_message->mValueT;
			//if (p_message->mValueT > 0) { mHistoMan->d1tMWPC[v_ch-12]->Fill(p_message->mValueT); } //TODO which value to use?
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=16) [" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	case 12: // MESYTEC mTDC: tSQ20 - time
		//TODO T-bit analysis???
		if (v_ch<16) {
			#ifdef PRINTDEBUGINFO
			cerr << "tSQ20[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->tSQ20[v_ch] = p_message->mValueT;
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=16) tSQ20[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	default:
		#ifdef PRINTDEBUGINFO
		cerr << "unidentified MESYTEC message from VME0: "; // << endl;
		p_message->ExtDump();
		#endif
		break;
	};
}

void UserProcMonitoring::ProcessMESYTECmessageVME1(const RawMessage* p_message)
{
	Int_t v_ch = p_message->mChannel;

	switch (p_message->mSubsubeventModule) {
	case 2:  // MESYTEC ???? //TODO
		#ifdef PRINTDEBUGINFO
		cerr << "neutTDC[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
		#endif
		break;
	default:
		#ifdef PRINTDEBUGINFO
		cerr << "unidentified MESYTEC message from VME1: "; // << endl;
		p_message->ExtDump();
		#endif
		break;
	};
}

void UserProcMonitoring::ProcessCAENmessage(const RawMessage* p_message)
{
	switch (p_message->mSubeventProcID) {
	case 100: // VME0
		this->ProcessCAENmessageVME0(p_message);
		break;
	case 200: // VME1
		this->ProcessCAENmessageVME1(p_message);
		break;
	default:
		#ifdef PRINTDEBUGINFO
		cerr << "unidentified CAEN message: "; // << endl;
		p_message->ExtDump();
		#endif
		break;
	};
}

void UserProcMonitoring::ProcessCAENmessageVME0(const RawMessage* p_message)
{
	Int_t v_ch = p_message->mChannel;

	switch (p_message->mSubsubeventGeo) {
	case 0:  // CAEN V560 scaler
		#ifdef PRINTDEBUGINFO
		cerr << "CAEN V560 scaler: "
		     << support::GetHexRepresentation(sizeof(Int_t), &(p_message->mRawWord)) << "  "
		     << support::GetBinaryRepresentation(sizeof(Int_t), &(p_message->mRawWord))
		     << endl;
		#endif
		break;
	case 1:  // CAEN TDC: tSQX left - time
		if (v_ch<32) {
			#ifdef PRINTDEBUGINFO
			cerr << "tSQX_L[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->tSQX_L[v_ch] = p_message->mValueT;
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=32) tSQX_L[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	case 2:  // CAEN TDC: tSQX right - time
		if (v_ch<32) {
			#ifdef PRINTDEBUGINFO
			cerr << "tSQX_R[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->tSQX_R[v_ch] = p_message->mValueT;
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=32) tSQX_R[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	case 3:  // CAEN TDC: tSQY left and right - time
		if (v_ch<16) {
			#ifdef PRINTDEBUGINFO
			cerr << "tSQY_L[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->tSQY_L[v_ch] = p_message->mValueT;
		} else if (v_ch<32) {
			#ifdef PRINTDEBUGINFO
			cerr << "tSQY_R[" << v_ch-16 << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->tSQY_R[v_ch-16] = p_message->mValueT;
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=32) tSQY_(L/R)[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	case 4:  // CAEN TDC: tCsI left and right - time
		if (v_ch<16) {
			#ifdef PRINTDEBUGINFO
			cerr << "tCsI_L[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->tCsI_L[v_ch] = p_message->mValueT;
		} else if (v_ch<32) {
			#ifdef PRINTDEBUGINFO
			cerr << "tCsI_R[" << v_ch-16 << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->tCsI_R[v_ch-16] = p_message->mValueT;
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=32) tCsI_(L/R)[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	case 30:  // RIO machine time, not really CAEN
		#ifdef PRINTDEBUGINFO
		cerr << "RIO machine time: "
		     << support::GetHexRepresentation(sizeof(Int_t), &(p_message->mRawWord)) << "  "
		     << support::GetBinaryRepresentation(sizeof(Int_t), &(p_message->mRawWord))
		     << endl;
		#endif
		break;
	default:
		#ifdef PRINTDEBUGINFO
		cerr << "unidentified CAEN message from VME0: "; // << endl;
		p_message->ExtDump();
		#endif
		break;
	};
}

void UserProcMonitoring::ProcessCAENmessageVME1(const RawMessage* p_message)
{
	Int_t v_ch = p_message->mChannel;

	switch (p_message->mSubsubeventGeo) {
	case 0:  // CAEN ???? //TODO
		if (v_ch<32) {
			if (v_ch%2 == 0) {
				#ifdef PRINTDEBUGINFO
				cerr << "neutAmp[" << (v_ch/2) << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
				#endif
				mCurrentOutputEvent->neutAmp[v_ch/2] = p_message->mValueQA; //TODO which value to use?
			} else {
				#ifdef PRINTDEBUGINFO
				cerr << "neutTAC[" << (v_ch/2) << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
				#endif
				mCurrentOutputEvent->neutTAC[v_ch/2] = p_message->mValueT; //TODO which value to use?
			}
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=32) neut(Amp/TAC)[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	case 1:  // CAEN ???? //TODO
		if (v_ch<32) {
			if (v_ch%2 == 0) {
				#ifdef PRINTDEBUGINFO
				cerr << "neutAmp[" << (v_ch/2)+16 << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
				#endif
				mCurrentOutputEvent->neutAmp[(v_ch/2)+16] = p_message->mValueQA; //TODO which value to use?
			} else {
				#ifdef PRINTDEBUGINFO
				cerr << "neutTAC[" << (v_ch/2)+16 << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
				#endif
				mCurrentOutputEvent->neutTAC[(v_ch/2)+16] = p_message->mValueT; //TODO which value to use?
			}
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=32) neut(Amp/TAC)[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	default:
		#ifdef PRINTDEBUGINFO
		cerr << "unidentified CAEN message from VME1: "; // << endl;
		p_message->ExtDump();
		#endif
		break;
	};
}

void UserProcMonitoring::ProcessCAMACmwpcWords(const UserEventUnpacking* p_inputEvent)
{
	const Short_t* v_inputCAMAC = p_inputEvent->mCAMAC;

	// Just print - shorts
	#ifdef PRINTDEBUGINFO
	cerr << "--------------------------------" << endl;
	cerr << support::GetBinaryRepresentation(sizeof(Short_t), &v_inputCAMAC[1]);
	cerr << support::GetBinaryRepresentation(sizeof(Short_t), &v_inputCAMAC[0]) << endl;
	cerr << support::GetBinaryRepresentation(sizeof(Short_t), &v_inputCAMAC[3]);
	cerr << support::GetBinaryRepresentation(sizeof(Short_t), &v_inputCAMAC[2]) << endl;
	cerr << support::GetBinaryRepresentation(sizeof(Short_t), &v_inputCAMAC[5]);
	cerr << support::GetBinaryRepresentation(sizeof(Short_t), &v_inputCAMAC[4]) << endl;
	cerr << support::GetBinaryRepresentation(sizeof(Short_t), &v_inputCAMAC[7]);
	cerr << support::GetBinaryRepresentation(sizeof(Short_t), &v_inputCAMAC[6]) << endl;
	cerr << "--------------------------------" << endl;
	#endif
/*
	// Transform pairs of shorts into normal ints
	Int_t v_line[4];
	v_line[0] = ((v_inputCAMAC[1] << 16) & 0xffff0000) |
	            ((v_inputCAMAC[0] << 0)  & 0x0000ffff);
	v_line[1] = ((v_inputCAMAC[3] << 16) & 0xffff0000) |
	            ((v_inputCAMAC[2] << 0)  & 0x0000ffff);
	v_line[2] = ((v_inputCAMAC[5] << 16) & 0xffff0000) |
	            ((v_inputCAMAC[4] << 0)  & 0x0000ffff);
	v_line[3] = ((v_inputCAMAC[7] << 16) & 0xffff0000) |
	            ((v_inputCAMAC[6] << 0)  & 0x0000ffff);

	// Just print - ints
	#ifdef PRINTDEBUGINFO
	cerr << "--------------------------------" << endl;
	cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_line[0]) << endl;
	cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_line[1]) << endl;
	cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_line[2]) << endl;
	cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_line[3]) << endl;
	cerr << "--------------------------------" << endl;
	#endif

	// Just print - bits
	#ifdef PRINTDEBUGINFO
	cerr << "--------------------------------" << endl;
	for (unsigned int i=0; i<4; i++) {
		for (unsigned char v_wire=0; v_wire<32; v_wire++) {
			unsigned char v_bitValue = (v_line[i] >> (32-v_wire-1)) & 0x1;
			cerr << (int)v_bitValue;
		}
		cerr << endl;
	}
	cerr << "--------------------------------" << endl;
	#endif
*/
}

ClassImp(UserProcMonitoring)
