void drawLEdiff_scan(TString listFilename="analysis_results/filelist2.txt", TString picturesDir="pictures3")
{
	// Read the table from the file
	std::ifstream ifs;
	ifs.open(listFilename.Data(), std::ifstream::in);

	std::string line;

	TString histoName("LeadingEdgeDiff_ID1_00/fhLeadingEdgeDiff_ID1_00_ID2_01");

	TCanvas* canv = new TCanvas("canv", "canv", 1364, 796);

	unsigned int lineCounter=0;
	while (ifs >> line) {
		TString curFilename;
		curFilename.Form("analysis_results/%s", line.c_str());

		TFile* curFile = new TFile(curFilename, "READ");
		if (curFile->IsZombie()) {
			cerr << "Error opening file " << curFilename.Data() << ". Aborting." << endl;
			return;
		}

		TH1D* curHisto;
		curHisto = (TH1D*)curFile->Get(histoName);
		if (curHisto == NULL) {
			cerr << "Histogram " << histoName << " not found in " << curFilename.Data() << ". Aborting." << endl;
			return;
		}

		TString hTitle;
		hTitle.Form("fhLeadingEdgeDiff_ID1_00_ID2_01 for threshold %d a.u.", lineCounter);
		curHisto->Draw();
		canv->SetTitle(hTitle);
		gPad->SetGrid(1, 1);

		TString outFilename;
		outFilename.Form("%s/ID1_00_ID2_01_Threshold_%d.png", picturesDir.Data(), lineCounter);
		canv->SaveAs(outFilename);
		outFilename.Form("%s/ID1_00_ID2_01_Threshold_%d.eps", picturesDir.Data(), lineCounter);
		canv->SaveAs(outFilename);

		lineCounter++;
	}
}
