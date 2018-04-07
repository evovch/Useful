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

	TGo4AnalysisStep* step = new TGo4AnalysisStep("UserAnalysisStep1", factory1);

	step->SetSourceEnabled(kTRUE);
	step->SetStoreEnabled(kFALSE);
	step->SetProcessEnabled(kTRUE);
	step->SetErrorStopEnabled(kTRUE);

	// STEP2 - processor - analysis ===============================================================

	TGo4StepFactory* factory2 = new TGo4StepFactory("Factory2");
	factory2->DefInputEvent("UserEvent1", "UserEvent"); // object name, class name
	factory2->DefEventProcessor("UserProc2", "UserProcStep2"); // object name, class name
	factory2->DefOutputEvent("UserEvent2", "UserEventStep2"); // object name, class name

	TGo4AnalysisStep* step2 = new TGo4AnalysisStep("UserAnalysisStep2", factory2);

	step->SetSourceEnabled(kFALSE);
	step->SetStoreEnabled(kFALSE);
	step->SetProcessEnabled(kTRUE);
	step->SetErrorStopEnabled(kTRUE);

	// ============================================================================================

	// Add STEPs to the analysis
	AddAnalysisStep(step);
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
