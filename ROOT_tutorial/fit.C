void fit(TString inFilename="generated_sum.root", UInt_t nComponents=4)
{
	Double_t xRange = 100.;

	// -------------------------------------------------------------------

	// Create a string for the analytical function

	TString functionString;
	TString subString;

	UInt_t iParam=0;
	for (UInt_t iComp=0; iComp<nComponents; iComp++) {
		subString.Form("[%d]*exp(-x/[%d])", iParam, iParam+1);
		iParam += 2;
		if (iComp==0) functionString = subString;
		else functionString += " + " + subString;
	}

	cout << functionString.Data() << endl;

	// -------------------------------------------------------------------

	// Create an analytical function, set initial parameters and set fitting precision

	TF1* fitFunc = new TF1("fitFunc", functionString, 0., xRange);

	// There are two parameters per compoment - amplitude and time constant
	for (UInt_t iParam=0; iParam<nComponents*2; iParam++) {
		fitFunc->SetParameter(iParam, 5.);
	}

	TVirtualFitter::SetMaxIterations(4000);
	cout << "Default precision: " << TVirtualFitter::GetPrecision() << endl;
	TVirtualFitter::SetPrecision(1e-2);
	cout << "Forced precision: " << TVirtualFitter::GetPrecision() << endl;

	// -------------------------------------------------------------------

	// Create a canvas
	TCanvas* canv = new TCanvas("canv", "canv", 100, 100, 1400, 800);

	// Open the input file with a histogram
	TFile* inFile = new TFile(inFilename, "READ");
	if (inFile->IsZombie()) {
		cerr << "Error opening file " << inFilename.Data() << ". Aborting." << endl;
		return;
	}

	// Get ths histogram from the input file
	TH1D* inputHisto = (TH1D*)inFile->Get("histo");
	if (inputHisto == NULL) {
		cerr << "Input histogram not found. Aborting." << endl;
		return;
	}


	inputHisto->Scale(1000.);
	inputHisto->Draw();
	inputHisto->Fit(fitFunc);
	gPad->SetGrid(1,1);
	gPad->SetLogy();


	// Print the fitting result
	for (UInt_t iComp=0; iComp<nComponents; iComp++) {
		cout << std::fixed;
		cout << std::setprecision(6) << fitFunc->GetParameter(iComp*2+0) << ":\t";
		cout << std::fixed;
		cout << std::setprecision(6) << fitFunc->GetParameter(iComp*2+1) << endl;
	}

	// Just create and draw a function with given parameters
	// This works only for 4 components
	TF1* fitFunc2 = new TF1("fitFunc2", functionString, 0., xRange);
	fitFunc2->SetParameters(10., 10., 20., 5., 40., 2.5, 80., 1.25);
	TCanvas* canv2 = new TCanvas("canv2", "canv2", 100, 100, 1400, 800);
	fitFunc2->Draw();
	gPad->SetGrid(1,1);
	gPad->SetLogy();
}
