void gen(UInt_t Nmean=10, Double_t tau=10., UInt_t nEvents=1000000)
{
	Double_t xRange = 100.;

	// Mean number of photoelectrons in event
	//UInt_t Nmean = 10;

	// Decay time constant
	//Double_t tau = 10.;

	// Random number generator
	TRandom3* rndGen = new TRandom3();

	// Create a string for the name of the function and fill it
	TString funcName;
	funcName.Form("expFunc");

	// Create an analytical function and set its parameters
	TF1* expFunc = new TF1(funcName, "exp(-x/[0])", 0., xRange);
	expFunc->SetParameter(0, tau);

	// Define bin size
	Double_t binSize=0.5;

	// Create a string for the histogram title and fill it
	TString histoTitle;
	histoTitle.Form("Time profile;ns;1/%0.2f", binSize);

	// Create the output histogram
	TH1D* histo = new TH1D("histo", histoTitle, (UInt_t)(xRange/binSize), 0., xRange);

	// Loop over the events
	for (UInt_t iEvent=0; iEvent<nEvents; iEvent++) {

		// Get the number of photoelectrons in event from the Poisson distribution
		UInt_t Npelectrons;
		Npelectrons= rndGen->Poisson(Nmean);

		// Loop over the photoelectrons
		for (UInt_t iPelectron=0; iPelectron<Npelectrons; iPelectron++) {
			// Get actual decay time for this photoelectron from the user-created analytical function
			Double_t time = expFunc->GetRandom();

			// Fill the output histogram
			histo->Fill(time);
		}

	}

	// Scale the histogram
	histo->Scale(1./((Double_t)nEvents*(Double_t)Nmean));

	// Create a canvas
	TCanvas* canv = new TCanvas("canv", "canv", 100, 100, 1400, 800);

	// This is optional here
	canv->cd();

	// Draw the output histogram on the created canvas
	histo->Draw();

	// Switch on the grid visualization
	gPad->SetGrid(1,1);

	// Set the logarithmic scale for the Y axis
	gPad->SetLogy();

	// Create a string for the output file name and fill it
	TString outFilename;
	outFilename.Form("generated_Nmean_%d_tau_%0.2f.root", Nmean, tau);

	// Create the output file
	TFile* outFile = new TFile(outFilename, "RECREATE");
	histo->Write();

	// Close the file?
}