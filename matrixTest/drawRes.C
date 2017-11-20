void drawRes(TString filename="res.root")
{
  TFile* inFile = new TFile(filename, "READ");
  TGraph2D* inGraph = (TGraph2D*)(inFile->Get("mGraph"));

  if (inGraph == NULL) {
    cerr << "Input graph mGraph not found!" << endl;
    return;
  }

  inGraph->Draw("P");
  inGraph->SetMarkerStyle(4);
}
