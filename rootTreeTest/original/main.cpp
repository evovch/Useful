#include <TFile.h>
#include <TTree.h>
#include <TRandom3.h>

#include "classes.h"

#include <iostream>
using std::cout;
using std::endl;

int main(int /*argc*/, char** /*argv*/)
{
	TFile theTreeFile("outputRootTreeFile.root", "RECREATE");
	TTree theTree("theTree", "The ROOT tree of events");

	cls_event* theEvent = new cls_event();

	theTree.Branch("theBranch", "cls_event", theEvent);

	UInt_t nEvents = 10;

	Double_t meanNhitsPerEvent = 20.;
	Double_t sigmaNhitsPerEvent = 5.;

	TRandom3 rndGen;

	for (UInt_t i=0; i<nEvents; i++) {

		Double_t nHits = rndGen.Gaus(meanNhitsPerEvent, sigmaNhitsPerEvent);

		cout << "Event (" << (UInt_t)nHits << " hits)" << endl;

		for (UInt_t j=0; j<(UInt_t)nHits; j++) {
			theEvent->AddHit(i, i, i);
		}

		theTree.Fill();
		theEvent->Clear();

	}

	delete theEvent;	// cen I do it here o only after writing to file?

	theTreeFile.Write();
	theTree.Print();

	return 0;
}
