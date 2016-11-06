#include "analysisParameters.h"

#include <fstream>
#include <iostream>

analysisParameters::analysisParameters()
{
}

analysisParameters::analysisParameters(TString filename)
{
   this->Import(filename);
}

analysisParameters::~analysisParameters()
{
}

void analysisParameters::Import(TString filename)
{
   ifstream inputStream(filename.Data());

   if (inputStream.is_open()) {
      std::string lmdFileName;
      uint64_t lWin;
      uint64_t rWin;
      while (!inputStream.eof()) {
         inputStream >> lmdFileName >> lWin >> rWin;
         if (lmdFileName[0] != '#') { // Ignore comments
            //std::cout << "Imported " << lmdFileName << " " << lWin << " " << rWin << std::endl;
            this->AddWindow(lmdFileName, lWin ,rWin);
         }
      }
      inputStream.close();
   } else {
      std::cout << "Failed importing trigger windows.\n" << std::endl;
   }
}

triggerWindow_t analysisParameters::GetWindow(TString filename)
{
   std::string filenameCPPstring(filename.Data());
   return this->GetWindow(filenameCPPstring);
}

triggerWindow_t analysisParameters::GetWindow(std::string filename)
{
   return fWindowMap[filename];
}

void analysisParameters::AddWindow(TString filename, uint64_t leftWin, uint64_t rightWin)
{
   std::string filenameCPPstring(filename.Data());
   this->AddWindow(filenameCPPstring, leftWin, rightWin);
}

void analysisParameters::AddWindow(std::string filename, uint64_t leftWin, uint64_t rightWin)
{
   triggerWindow_t curWin(leftWin, rightWin);
   fWindowMap.insert(std::pair<std::string, triggerWindow_t> (filename, curWin));
}

