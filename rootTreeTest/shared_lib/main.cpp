#include <TFile.h>
#include <TTree.h>
#include <TRandom3.h>

#include "classes.h"

#include <iostream>
using std::cout;
using std::endl;

void main_so(void)
{
	TFile theTreeFile("outputRootTreeFile.root", "RECREATE");
	TTree theTree("theTree", "The ROOT tree of events");

	cls_event* theEvent = new cls_event();

	theTree.Branch("theBranch", "cls_event", theEvent);

	UInt_t nEvents = 1000;
	Double_t meanNhitsPerEvent = 20.;

	TRandom3 rndGen;

	for (UInt_t i=0; i<nEvents; i++) {

		Double_t nHits = rndGen.Poisson(meanNhitsPerEvent);

		Double_t eventTimestamp = rndGen.Gaus(0., 10.);

		//cout << "Event " << i << "\t\tTs: " << (i+1)*1000.+eventTimestamp << "\tnHits: " << (UInt_t)nHits << endl;

		Double_t tsInPs = (Double_t)(i+1) * 1000. + eventTimestamp;
		theEvent->SetTimestamp(tsInPs);

		for (UInt_t j=0; j<(UInt_t)nHits; j++)
		{
			Double_t hitTs = rndGen.Exp(5000.);
			UInt_t hitCh = rndGen.Integer(128);
			UInt_t hitAdc = rndGen.Integer(1024);

			theEvent->AddHit((UInt_t)hitTs, hitCh, hitAdc);

			//cout << "\t\tHit " << j << "\tTs: " << hitTs/1000. << "\tCh: " << hitCh << "\tAdc: " << hitAdc << endl;
		}

		theEvent->NextEvent();
		theTree.Fill();
		theEvent->Clear();

	}

	delete theEvent;

	theTreeFile.Write();
	//theTree.Print();

	//return 0;
}
