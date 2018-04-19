#include "UserAnalysis.h"

// STD
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <TGo4Version.h> // for CheckVersion

#include <TGo4StepFactory.h>

#include "setupconfig/pureC/functions.h"

UserAnalysis::UserAnalysis(const char* name) :
	TGo4Analysis(name),
	mEventCounter(0),
	//TODO not used by now ---------
	mMbsEvent(nullptr),
	mUserEventUnpack(nullptr),
	mUserEventMonitoring(nullptr),
	mUserEventStep2(nullptr)
	//------------------------------
{
	this->Construct();
	cout << "UserAnalysis constructed." << endl;
}

UserAnalysis::UserAnalysis(int argc, char** argv) :
	TGo4Analysis(argc, argv),
	mEventCounter(0),
	//TODO not used by now ---------
	mMbsEvent(nullptr),
	mUserEventUnpack(nullptr),
	mUserEventMonitoring(nullptr),
	mUserEventStep2(nullptr)
	//------------------------------
{
	this->Construct();
	cout << "UserAnalysis constructed." << endl;
}

UserAnalysis::~UserAnalysis()
{
	cout << "UserAnalysis destructed." << endl;
}

void UserAnalysis::Construct(void)
{
	cout << "UserAnalysis::Construct()." << endl;

	//TODO check this method implementation

	//TODO maybe comment this out?
	if (!TGo4Version::CheckVersion(__GO4BUILDVERSION__)) {
		cout << "Go4 version mismatch! Aborting." << endl;
		exit(-1);
	}

	// Import XML setup config file
	stc_setup_config obj1;
	InitStcSetupConfig(&obj1);
	ImportXML(&obj1, "usr/setup.xml");

	//TODO
	//SetStepChecking(kFALSE); // necessary for non-subsequent mesh analysis

	// STEP1 - source - unpacker ==================================================================

	TGo4StepFactory* factoryUnpacking = new TGo4StepFactory("FactoryUnpacking");
	factoryUnpacking->DefEventProcessor("UserProcUnpacking1", "UserProcUnpacking"); // object name, class name
	factoryUnpacking->DefOutputEvent("UserEventUnpacking1", "UserEventUnpacking"); // object name, class name

	TGo4AnalysisStep* stepUnpacking = new TGo4AnalysisStep("UserAnalysisStepUnpacking", factoryUnpacking);

	stepUnpacking->SetSourceEnabled(kTRUE);
	stepUnpacking->SetProcessEnabled(kTRUE);
	stepUnpacking->SetErrorStopEnabled(kTRUE);

	TGo4FileStoreParameter* storeUnpacking = new TGo4FileStoreParameter("outputUnpacking.root");
	stepUnpacking->SetEventStore(storeUnpacking);
	stepUnpacking->SetStoreEnabled(kTRUE);

	AddAnalysisStep(stepUnpacking);

	// STEP2 - processor - monitoring =============================================================

	TGo4StepFactory* factoryMonitoring = new TGo4StepFactory("FactoryMonitoring");
	factoryMonitoring->DefInputEvent("UserEventUnpacking1", "UserEventUnpacking"); // object name, class name
	factoryMonitoring->DefEventProcessor("UserProcMonitoring1", "UserProcMonitoring"); // object name, class name
	factoryMonitoring->DefOutputEvent("UserEventMonitoring1", "UserEventMonitoring"); // object name, class name

	TGo4AnalysisStep* stepMonitoring = new TGo4AnalysisStep("UserAnalysisStepMonitoring", factoryMonitoring);

	stepMonitoring->SetSourceEnabled(kFALSE);
	stepMonitoring->SetProcessEnabled(kTRUE);
	stepMonitoring->SetErrorStopEnabled(kTRUE); //TODO probably for monitoring this should be false

	TGo4FileStoreParameter* storeMonitoring = new TGo4FileStoreParameter("outputMonitoring.root");
	stepMonitoring->SetEventStore(storeMonitoring);
	stepMonitoring->SetStoreEnabled(kTRUE);

	AddAnalysisStep(stepMonitoring);

	// STEP2 - processor - step2 ==================================================================
/*
//TODO
	TGo4StepFactory* factoryStep2 = new TGo4StepFactory("FactoryStep2");
	factoryStep2->DefInputEvent("UserEventUnpacking1", "UserEventUnpacking"); // object name, class name
	factoryStep2->DefEventProcessor("UserProcStep2_1", "UserProcStep2"); // object name, class name
	factoryStep2->DefOutputEvent("UserEventStep2_1", "UserEventStep2"); // object name, class name

	TGo4AnalysisStep* step2 = new TGo4AnalysisStep("UserAnalysisStep2", factoryStep2);

	step2->SetSourceEnabled(kFALSE);
	step2->SetProcessEnabled(kTRUE);
	step2->SetErrorStopEnabled(kTRUE);

	TGo4FileStoreParameter* storeStep2 = new TGo4FileStoreParameter("outputStep2.root");
	step2->SetEventStore(storeStep2);
	step2->SetStoreEnabled(kTRUE);

	AddAnalysisStep(step2);
*/
	// ============================================================================================
}

Int_t UserAnalysis::UserPreLoop(void)
{
	//cout << "UserAnalysis::UserPreLoop()." << endl;
	cerr << "Starting UserAnalysis." << endl;
	mEventCounter = 0;
	return 0;
}

// This function is called once for each event after all steps
Int_t UserAnalysis::UserEventFunc(void)
{
	//cout << "UserAnalysis::UserEventFunc()." << endl;
	mEventCounter++;
	return 0;
}

Int_t UserAnalysis::UserPostLoop(void)
{
	//cout << "UserAnalysis::UserPostLoop()." << endl;
	cerr << "Finished UserAnalysis. Total " << mEventCounter << " events." << endl;
	return 0;
}

ClassImp(UserAnalysis)
