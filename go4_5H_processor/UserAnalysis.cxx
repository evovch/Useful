#include "UserAnalysis.h"

//TODO test //TODO why not iostream?
#include <Riostream.h>
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

	// STEP1 - source - unpacker ==================================================================

	TGo4StepFactory* factory1 = new TGo4StepFactory("Factory1");
	factory1->DefEventProcessor("UserProc1", "UserProc"); // object name, class name
	factory1->DefOutputEvent("UserEvent1", "UserEvent"); // object name, class name

	TGo4AnalysisStep* step1 = new TGo4AnalysisStep("UserAnalysisStep1", factory1);

	step1->SetSourceEnabled(kTRUE);
	step1->SetStoreEnabled(kFALSE);
	step1->SetProcessEnabled(kTRUE);
	step1->SetErrorStopEnabled(kTRUE);

	TGo4FileStoreParameter* store = new TGo4FileStoreParameter("output.root");
	step1->SetEventStore(store);
	step1->SetStoreEnabled(kTRUE);

	// STEP2 - processor - analysis ===============================================================

	TGo4StepFactory* factory2 = new TGo4StepFactory("Factory2");
	factory2->DefInputEvent("UserEvent1", "UserEvent"); // object name, class name
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
	AddAnalysisStep(step1);
	AddAnalysisStep(step2);
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
