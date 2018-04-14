#include "UserAnalysis.h"

// STD
#include <iostream>
using std::cout;
using std::endl;

#include <TGo4Version.h> // for CheckVersion

#include <TGo4StepFactory.h>

UserAnalysis::UserAnalysis(const char* name) :
	TGo4Analysis(name)
{
	this->Construct();
	cout << "UserAnalysis constructed." << endl;
}

UserAnalysis::UserAnalysis(int argc, char** argv) :
	TGo4Analysis(argc, argv)
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

	//TODO
	SetStepChecking(kFALSE); // necessary for non-subsequent mesh analysis

	// STEP1 - source - unpacker ==================================================================

	TGo4StepFactory* FactoryUnpacking = new TGo4StepFactory("FactoryUnpacking");
	FactoryUnpacking->DefEventProcessor("UserProcUnpacking1", "UserProcUnpacking"); // object name, class name
	FactoryUnpacking->DefOutputEvent("UserEventUnpacking1", "UserEventUnpacking"); // object name, class name

	TGo4AnalysisStep* stepUnpacking = new TGo4AnalysisStep("UserAnalysisStepUnpacking", FactoryUnpacking);

	stepUnpacking->SetSourceEnabled(kTRUE);
	stepUnpacking->SetStoreEnabled(kFALSE);
	stepUnpacking->SetProcessEnabled(kTRUE);
	stepUnpacking->SetErrorStopEnabled(kTRUE);

	TGo4FileStoreParameter* store = new TGo4FileStoreParameter("output.root");
	stepUnpacking->SetEventStore(store);
	stepUnpacking->SetStoreEnabled(kTRUE);

	// STEP2 - processor - monitoring =============================================================

	TGo4StepFactory* factoryMonitoring = new TGo4StepFactory("FactoryMonitoring");
	factoryMonitoring->DefInputEvent("UserEventUnpacking1", "UserEventUnpacking"); // object name, class name
	factoryMonitoring->DefEventProcessor("UserProcMonitoring1", "UserProcMonitoring"); // object name, class name
	factoryMonitoring->DefOutputEvent("UserEventMonitoring1", "UserEventMonitoring"); // object name, class name

	TGo4AnalysisStep* stepMonitoring = new TGo4AnalysisStep("UserAnalysisStepMonitoring", factoryMonitoring);

	stepMonitoring->SetSourceEnabled(kFALSE);
	stepMonitoring->SetStoreEnabled(kFALSE);
	stepMonitoring->SetProcessEnabled(kTRUE);
	stepMonitoring->SetErrorStopEnabled(kTRUE); //TODO probably for monitoring this should be false

	TGo4FileStoreParameter* store3 = new TGo4FileStoreParameter("output3.root");
	stepMonitoring->SetEventStore(store3);
	stepMonitoring->SetStoreEnabled(kTRUE);

	// STEP2 - processor - analysis ===============================================================

	TGo4StepFactory* factory2 = new TGo4StepFactory("Factory2");
	factory2->DefInputEvent("UserEventUnpacking1", "UserEventUnpacking"); // object name, class name
	factory2->DefEventProcessor("UserProc2", "UserProcStep2"); // object name, class name
	factory2->DefOutputEvent("UserEvent2", "UserEventStep2"); // object name, class name

	TGo4AnalysisStep* step2 = new TGo4AnalysisStep("UserAnalysisStep2", factory2);

	step2->SetSourceEnabled(kFALSE);
	step2->SetStoreEnabled(kFALSE);
	step2->SetProcessEnabled(kTRUE);
	step2->SetErrorStopEnabled(kTRUE);

	TGo4FileStoreParameter* store2 = new TGo4FileStoreParameter("output2.root");
	step2->SetEventStore(store2);
	step2->SetStoreEnabled(kTRUE);

	// ============================================================================================

	// Add STEPs to the analysis
	AddAnalysisStep(stepUnpacking);
	AddAnalysisStep(stepMonitoring);
	AddAnalysisStep(step2); //TODO
}

Int_t UserAnalysis::UserPreLoop(void)
{
	cout << "UserAnalysis::UserPreLoop()." << endl;
	return 0;
}

// This function is called once for each event after all steps
Int_t UserAnalysis::UserEventFunc(void)
{
	//cout << "UserAnalysis::UserEventFunc()." << endl;
	return 0;
}

Int_t UserAnalysis::UserPostLoop(void)
{
	cout << "UserAnalysis::UserPostLoop()." << endl;
	return 0;
}

ClassImp(UserAnalysis)
