#include "UserAnalysisHistos.h"

#include <TH1D.h>
#include <TH2D.h>
//#include <TGraph.h> // Does not work by now

#include <TGo4Analysis.h>

UserAnalysisHistos::UserAnalysisHistos()
{
	TGo4Analysis* a = TGo4Analysis::Instance();

	mHisto1 = a->MakeTH1('D', "mHisto1", "mHisto1", 100, 0., 100.);
	mHisto2 = a->MakeTH2('D', "mHisto2", "mHisto2", 100, 0., 100., 100, 0., 100.);
	//mGraph = new TGraph(); // Does not work by now
}

UserAnalysisHistos::~UserAnalysisHistos()
{
	/**
	 * Not 100% sure, but it looks as if you do not need to delete your histograms yourself.
	 * TGo4AnalysisObjectManager, as part of the singleton TGo4Analysis, will take care.
	 */
}

ClassImp(UserAnalysisHistos)
