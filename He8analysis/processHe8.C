struct outputDataStructure
{
  Double_t tof;
};

void processHe8(TString inFilename="he8_0023.root", TString outFilename="out_he8_0023.root")
{
  gStyle->SetOptStat(111111);
  TH1::AddDirectory(kFALSE);

  Double_t MTDC[32];
  UShort_t MQDC[32];

  // Input file and input tree
  TFile* inFile = new TFile(inFilename, "READ");
  TTree* inTree = (TTree*) inFile->Get("AnalysisxTree");
  inTree->SetMakeClass(1);

  inTree->SetBranchAddress("LiEvent.MTDC[32]", MTDC);
  inTree->SetBranchAddress("LiEvent.MQDC[32]", MQDC);

  // Create the output root file, a tree inside and a branch
  struct outputDataStructure dataObject;
  TFile* outFile = new TFile(outFilename, "RECREATE");
  TTree* outTree = new TTree("outTree", "outTree");
  outTree->Branch("tof", &dataObject.tof, "tof/D");

  Long64_t nEvents= inTree->GetEntries();
  cout << inFilename << "\t\t" << nEvents << " entries in the file." << endl;

  TH1D* histoToF = new TH1D("histoToF", "Time-of-flight", 800, -400., 400.);
  TH1D* mqdcSpectrum = new TH1D("mqdcSpectrum", "mqdcSpectrum", 1000, 0., 4000.);

  for (Long64_t iEvent=0; iEvent<nEvents; iEvent++)
  {
    // Read event from the input tree
    inTree->GetEntry(iEvent);

    // Compute ToF
    Double_t ToF = 0.;
    ToF = (MTDC[4]+MTDC[5])/2 - (MTDC[2]+MTDC[3])/2;

    for (Int_t iChan=0; iChan<32; iChan++) {
      mqdcSpectrum->Fill(MQDC[iChan]);
    }

    // Write event into the output tree
    dataObject.tof = ToF;
    outTree->Fill();

    // Fill histo
    histoToF->Fill(ToF);
  }

  // Write output ROOT file with processed data and close it
  outTree->Write();
  outFile->Close();

  TCanvas* canv = new TCanvas("canv", "canv");
  canv->cd();
  histoToF->Draw();
  gPad->SetGrid(1, 1);

  TCanvas* canv2 = new TCanvas("canv2", "canv2");
  canv2->cd();
  mqdcSpectrum->Draw();
  gPad->SetGrid(1, 1);
}