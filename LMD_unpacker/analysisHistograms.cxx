#include "analysisHistograms.h"

analysisHistograms::analysisHistograms()
{
   TString histoName;
   TString histoTitle;

   hTriggerCorrelation = new TH1D("hTriggerCorrelation", "hTriggerCorrelation", 400/2, 800., 1200.);
   hRemovedHitsBelowWin = new TH1D("hRemovedHitsBelowWin", "hRemovedHitsBelowWin", 1000, 0., 1000.);
   hNumOfHitsInEvent = new TH1D("hNumOfHitsInEvent", "hNumOfHitsInEvent", 100, 0., 100.);
   hNumOfHitsInChannel = new TH1D("hNumOfHitsInChannel", "hNumOfHitsInChannel", 128, 0., 128.);

   for (unsigned int i=0; i<128; i++) {
      histoName.Form("hADC_ch%d", i);
      histoTitle.Form("hADC_ch%d", i);
      hAdcSpectrum[i] = new TH1D(histoName, histoTitle, 4096/4, 0., 4096.);
   }
}

analysisHistograms::~analysisHistograms()
{
}

void analysisHistograms::Write()
{
   TFile* rootFile = new TFile("res.root", "UPDATE");

   TString motherFolderName;
   motherFolderName.Form("Histograms_%s", fSuffix.Data());
   rootFile->mkdir(motherFolderName);
   rootFile->cd(motherFolderName);

   hTriggerCorrelation->Write();
   hRemovedHitsBelowWin->Write();
   hNumOfHitsInEvent->Write();
   hNumOfHitsInChannel->Write();

   TString folderName;
   folderName.Form("Histograms_%s/ADC", fSuffix.Data());
   rootFile->mkdir(folderName);
   rootFile->cd(folderName);
   for (unsigned int i=0; i<128; i++) {
      hAdcSpectrum[i]->Write();
   }
   rootFile->cd("..");
   rootFile->cd("..");

   rootFile->Close();
}

