#include "UserProcMonitoring.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// ROOT
#include <TClonesArray.h>

// Project
#include "Support.h"
#include "UserParameter.h"
#include "UserEventMonitoring.h"
//#include "data_events/UserEventNew.h"
#include "UserHistosMonitoring.h"
#include "data/RawMessage.h"
#include "unpacking/UserEventUnpacking.h"
#include "setupconfigcppwrapper/SetupConfiguration.h"

/**
  Uncomment this if you want to see all the debug information.
  This allows you to analyze the raw bytes and bits by your eyes.
  This option produces A LOT OF DATA - run your analysis with a
  small number of events (~10-100)
*/
//#define PRINTDEBUGINFO

UserProcMonitoring::UserProcMonitoring(const char* name) :
	TGo4EventProcessor(name),
	fEventCounter(0)
{
	fHistoMan = new UserHistosMonitoring();

	// Get the all-accessible parameter-set object
	UserParameter* v_params = (UserParameter*)GetParameter("UserParameter");
	// Extract the setup configuration file name which was set during UserAnalysis initialization
	TString v_setupconfigfilename = v_params->GetSetupConfigFilename();
	// Construct SetupConfiguration, which includes the input of the XML file
	fSetupConfiguration = new SetupConfiguration(v_setupconfigfilename);
}

UserProcMonitoring::~UserProcMonitoring()
{
	if (fHistoMan) delete fHistoMan;
	if (fSetupConfiguration) delete fSetupConfiguration;
}

Bool_t UserProcMonitoring::BuildEvent(TGo4EventElement* p_dest)
{
	Bool_t v_isValid = kFALSE;
	UserEventMonitoring* v_outputEvent = (UserEventMonitoring*)p_dest;
	//UserEventNew* v_outputEvent = (UserEventNew*)p_dest;

	UserEventUnpacking* v_input = (UserEventUnpacking*)GetInputEvent();
	if (v_input == NULL)
	{
		cerr << "[WARN  ] " << "UserProcMonitoring::BuildEvent(): no input event!" << endl;
		v_outputEvent->SetValid(v_isValid);
		return v_isValid;
	}
	v_isValid = kTRUE;

	#ifdef PRINTDEBUGINFO
	cerr << "[DEBUG ] " << "UserProcMonitoring: Event " << fEventCounter
	     << " ======================================================================================================"
	     << endl;
	#endif

	fCurrentOutputEvent = v_outputEvent;

	// Clear the output event!!!
	//TODO check that this is not done by the framework
	// Seems that indeed this is done by the framework
	//fCurrentOutputEvent->Clear();

	//TODO do the processing of raw messages here
	UInt_t v_messCounter = 0;
	TIter next(v_input->fRawMessages);
	while (RawMessage* v_curMessage = (RawMessage*)next())
	{

		#ifdef PRINTDEBUGINFO
		cerr << v_messCounter << ": ";
		v_curMessage->Dump(false);
		cerr << "\t\t";
		#endif

		this->ProcessMessageUniversal(v_curMessage);

		//TODO check
		// Ptocess trigger
		fCurrentOutputEvent->trigger = v_input->fTrigger;

		v_messCounter++;
	} // end of while

	//TODO do the processing of CAMAC MWPC words here
	this->ProcessCAMACmwpcWords(v_input);

	// --------------------------

	v_outputEvent->SetValid(v_isValid);

	fEventCounter++;

	return v_isValid;
}

void UserProcMonitoring::ProcessMessageUniversal(const RawMessage* p_message)
{
	unsigned short v_procid = (unsigned short)p_message->fSubeventProcID;
	unsigned short v_addr;
	support::enu_VENDOR v_messVendor = support::VendorFromChar(p_message->fSubsubeventVendor);
	if (v_messVendor == support::enu_VENDOR::MESYTEC) {
		v_addr = (unsigned short)p_message->fSubsubeventModule;
	} else if (v_messVendor == support::enu_VENDOR::CAEN) {
		v_addr = (unsigned short)p_message->fSubsubeventGeo;
	} else {
		cerr << "[ERROR ]" << " UserProcMonitoring::ProcessMessageUniversal() Unknown vendor." << endl;
		return;
	}
	unsigned short v_ch = (unsigned short)p_message->fChannel;

	//TODO check that fSetupConfiguration is not NULL
	TString v_detector;
	TString v_folder;
	unsigned short v_detChannel = fSetupConfiguration->GetOutput(v_procid, v_addr, v_ch, &v_detector, &v_folder);

	#ifdef PRINTDEBUGINFO
	cerr << "[DEBUG ] " << v_folder << " /\t" << v_detector << "[" << v_ch << "] =\t"
	     << p_message->fValueQA << "\t(" << p_message->fValueT << ")" << endl;
	#endif

	UShort_t* eventDatField = fCurrentOutputEvent->GetFieldByName(v_detector);

	if (eventDatField != NULL) {
		//TODO check that the channel has allowed value
		//FIXME or p_message->mValueT ?
		eventDatField[v_detChannel] = p_message->fValueQA;
	}
}

void UserProcMonitoring::ProcessCAMACmwpcWords(const UserEventUnpacking* p_inputEvent)
{
	const Short_t* v_inputCAMAC = p_inputEvent->fCAMAC;
/*
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
*/
	// Transform pairs of shorts into normal ints
	UInt_t v_line[4];
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
	cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_line[0]) << "\t0x"
	     << support::GetHexRepresentation(sizeof(Int_t), &v_line[0]) << "\t"
	     << v_line[0] << endl;
	cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_line[1]) << "\t0x"
	     << support::GetHexRepresentation(sizeof(Int_t), &v_line[1]) << "\t"
	     << v_line[1] << endl;
	cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_line[2]) << "\t0x"
	     << support::GetHexRepresentation(sizeof(Int_t), &v_line[2]) << "\t"
	     << v_line[2] << endl;
	cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_line[3]) << "\t0x"
	     << support::GetHexRepresentation(sizeof(Int_t), &v_line[3]) << "\t"
	     << v_line[3] << endl;
	cerr << "--------------------------------" << endl;
	#endif
/*
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
