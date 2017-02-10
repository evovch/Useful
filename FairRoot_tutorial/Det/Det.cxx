#include "Det.h"

#include <iostream>
using namespace std;

#include "FairRootManager.h"
#include "TClonesArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TString.h"
// -----   Default constructor   -------------------------------------------
Det::Det() : FairDetector("Det", kTRUE)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
Det::Det(const char* name, Bool_t active, Int_t verbose) 
  : FairDetector(name, active,verbose)
{
}

Det::~Det(){
}

void Det::Initialize()
{
  FairDetector::Initialize();
}


Bool_t Det::ProcessHits(FairVolume* vol) {
  return kTRUE;
}

// -----   Public method EndOfEvent   -----------------------------------------
void Det::BeginEvent() {
}

void Det::EndOfEvent() {
  Print();
  Reset();
}

// -----   Public method Register   -------------------------------------------
void Det::Register() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
	Fatal("Init", "IO manager is not set");
}
// ----------------------------------------------------------------------------

// -----   Public method GetCollection   --------------------------------------
TClonesArray* Det::GetCollection(Int_t iColl) const {
    return NULL;
}
// ----------------------------------------------------------------------------

// -----   Public method Print   ----------------------------------------------
void Det::Print(Option_t *option) const
{
}
// ----------------------------------------------------------------------------

// -----   Public method Reset   ----------------------------------------------
void Det::Reset() {
  LOG(INFO) << "  Det::Reset()" << FairLogger::endl;
}
// ----------------------------------------------------------------------------

// -----   Public method CopyClones   -----------------------------------------
void Det::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
}
// ----------------------------------------------------------------------------
// -----   Public method ConstructGeometry   ----------------------------------
void Det::ConstructGeometry() {
  TString fileName = GetGeometryFileName();
  if(fileName.EndsWith(".root")) {
    cout << "Constructing Det geometry from ROOT file " << fileName.Data() << FairLogger::endl;
    ConstructRootGeometry();
  } else {
    cerr << "Geometry file name is not set" << FairLogger::endl;
  }
  
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
Bool_t Det::CheckIfSensitive(std::string name)
{
  return kFALSE;
}
// ----------------------------------------------------------------------------
ClassImp(Det)
