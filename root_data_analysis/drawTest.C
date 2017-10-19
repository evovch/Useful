void drawTest(TString filename="He8/he8_0004.root", TString outFilename="output/out_he8_0004.root")
{
	gStyle->SetOptStat(111111);

	TH1::AddDirectory(kFALSE);

	Double_t MTDC[32];
	Double_t energyX;

	TFile* inFile = new TFile(filename, "READ");
	TTree* theTree = (TTree*) inFile->Get("AnalysisxTree");
	theTree->SetMakeClass(1);

	theTree->SetBranchAddress("LiEvent.MTDC[32]", MTDC);
	theTree->SetBranchAddress("LiEvent.energyX", &energyX);

	/*TBranch* theBranchMTDC = theTree->GetBranch("LiEvent.MTDC[32]");
	TBranch* theBranchEnergyX = theTree->GetBranch("LiEvent.energyX");
	printf("%p\n", theBranchMTDC);
	printf("%p\n", theBranchEnergyX);
	*/

	TH2D* histo = new TH2D("histo", "histo", 400, -200., 200., 250, -50., 200.);

	// Loop over events

	Long64_t nEvents= theTree->GetEntries();
	cout << filename << "\t\t" << nEvents << " entries in the file." << endl;
	//nEvents = 2;
	for (Long64_t iEvent=0; iEvent<nEvents; iEvent++) {

		//if ( !(iEvent%1000) ) std::cout << iEvent << " entries processed." << std::endl;

		theTree->GetEntry(iEvent);
		Double_t ToF;

		ToF = (MTDC[4]+MTDC[5])/2 - (MTDC[2]+MTDC[3])/2;

		//cout << energyX << "\t" << ToF << endl;

		histo->Fill(ToF, energyX);
	}

	TCanvas* canv = new TCanvas("canv", "canv");

	histo->Draw("SURF");
	gPad->SetGrid(1, 1);


	// ==================================================================
	// Export into the output file

    TDirectory* prevDir = gDirectory;
    TFile* prevFile = gFile;

    // Open output file
	TFile v_outputFile(outFilename, "RECREATE");

	// Check that the output file has been successfully opened
    if (v_outputFile.IsZombie()) {
        cerr << "Error opening file " << outFilename.Data() << endl;
        gDirectory = prevDir;
        gFile = prevFile;
        return; // FAIL
	}


	// Write histograms
	histo->Write();

	// Close output file
    v_outputFile.Close();

    //cout << "Successfully exported analysis histograms into " << outFilename.Data() << "." << endl;

    gDirectory = prevDir;
	gFile = prevFile;

}