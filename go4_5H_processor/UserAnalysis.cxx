#include "UserAnalysis.h"

//TODO test //TODO why not iostream?
#include <Riostream.h>
using std::cout;
using std::endl;

#include <TGo4Version.h> // for CheckVersion

#include <TGo4StepFactory.h>
#include <TGo4MbsEvent.h>

UserAnalysis::UserAnalysis(const char* name) :
	TGo4Analysis(name),
	fMbsEvent(NULL),
	fEvents(0),
	fLastEvent(0)
{
	this->Construct();
	cout << "UserAnalysis constructed." << endl;
}

UserAnalysis::UserAnalysis(int argc, char** argv) :
	TGo4Analysis(argc, argv),
	fMbsEvent(NULL),
	fEvents(0),
	fLastEvent(0)
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

	TGo4StepFactory* factory = new TGo4StepFactory("Factory");
	factory->DefEventProcessor("UserProc1", "UserProc"); // object name, class name
	factory->DefOutputEvent("UserEvent1", "UserEvent"); // object name, class name

	TGo4AnalysisStep* step = new TGo4AnalysisStep("UserAnalysisStep1", factory);

	step->SetSourceEnabled(kTRUE);
	//step->SetStoreEnabled(kTRUE);
	step->SetProcessEnabled(kTRUE);
	step->SetErrorStopEnabled(kTRUE);

	AddAnalysisStep(step);
}

Int_t UserAnalysis::UserPreLoop(void)
{
	cout << "UserAnalysis::UserPreLoop()." << endl;

	// get pointer to input event (used in postloop and event function):
	fMbsEvent = dynamic_cast<TGo4MbsEvent*> (GetInputEvent("Analysis")); // of step "Analysis"
	if (fMbsEvent) {
		// fileheader structure (lmd file only):
		s_filhe* fileheader = fMbsEvent->GetMbsSourceHeader();
		if (fileheader)
		{
			cout << "\nInput file: " << fileheader->filhe_file << endl;
			cout << "Tapelabel:\t" << fileheader->filhe_label << endl;
			cout << "UserName:\t" << fileheader->filhe_user << endl;
			cout << "RunID:\t" << fileheader->filhe_run << endl;
			cout << "Explanation: " << fileheader->filhe_exp << endl;
			cout << "Comments: "<< endl;
			Int_t numlines=fileheader->filhe_lines;
			for(Int_t i=0; i<numlines;++i)
			{
				cout << "\t" << fileheader->s_strings[i].string << endl;
			}
		}
	}
	fEvents = 0; // event counter
	fLastEvent = 0; // number of last event processed

	return 0;
}

// This function is called once for each event after all steps
Int_t UserAnalysis::UserEventFunc(void)
{
	//cout << "UserAnalysis::UserEventFunc()." << endl;

	if (fMbsEvent) {
		fEvents++;
		fLastEvent = fMbsEvent->GetCount();
	}
	if (fEvents == 1 || IsNewInputFile()) {
		cout << "First event #: " << fLastEvent << endl;
		SetNewInputFile(kFALSE); // we have to reset the newfile flag
	}

	return 0;
}

Int_t UserAnalysis::UserPostLoop(void)
{
	cout << "UserAnalysis::UserPostLoop()." << endl;

	cout << "Last event #: " << fLastEvent << " Total events: " << fEvents << endl;
	fMbsEvent = 0; // reset to avoid invalid pointer if analysis is changed in between
	fEvents = 0;

	return 0;
}

ClassImp(UserAnalysis)
