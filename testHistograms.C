#define THR 15000

// Use input ROOT file from the Analysis5/sums/ directory

void testHistograms()
{
	TFile* f1 = new TFile("analysis_sum_rings_calib_corr.root");

	UInt_t id1;
	UInt_t id2;

	TString dirName;
	TString histoName;

	UInt_t canvCounter=0;

	TH1D* curHisto = NULL;

	for (id1=0; id1<=254; id1++) {

		dirName.Form("LeadingEdgeDiff_ID1_%02d", id1);

		for (id2=id1+1; id2<=255; id2++) {

			histoName.Form("%s/fhLeadingEdgeDiff_ID1_%02d_ID2_%02d", dirName.Data(), id1, id2);

			curHisto = (TH1D*)f1->Get(histoName);

			if (curHisto == NULL) {
				cout << "Histogram not found." << endl;
			} else {

				if (curHisto->GetEntries() > THR) {
					cout << curHisto->GetName() << "\t" << curHisto->GetEntries() << "\tMean=" << curHisto->GetMean() << "\tRMS=" << curHisto->GetRMS() << endl;

					TString canvName;
					canvName.Form("canv_%d", canvCounter++);
					//TCanvas* canv = new TCanvas(canvName, canvName);
					//curHisto->Draw();

				}

				curHisto = NULL;
			}

		}
	}
}

