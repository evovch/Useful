#include "TString.h"
#include "TH1.h"
#include "TFile.h"

class analysisHistograms
{
public:
   analysisHistograms();
   ~analysisHistograms();

   void Write();
   void SetOutputRootFileName(TString filename) { fOutputRootFileName = filename; }
   void SetSuffix(TString suf) { fSuffix = suf; }

public:
   TH1D* hTriggerCorrelation;
   TH1D* hRemovedHitsBelowWin;
   TH1D* hNumOfHitsInEvent;
   TH1D* hNumOfHitsInChannel;
   TH1D* hAdcSpectrum[128];

private:
   TString fOutputRootFileName;
   TFile* fOutputRootFile;

   TString fSuffix;

};

