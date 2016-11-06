#include <stdint.h>
#include <map>
#include <string>

#include "TString.h"

typedef std::pair<uint64_t, uint64_t> triggerWindow_t;

class analysisParameters
{

public:
   analysisParameters();
   analysisParameters(TString filename);
   ~analysisParameters();

   void Import(TString filename="TriggerWindows.cfg");

   // Identical methods, jut one using ROOT TString, another - std::string
   triggerWindow_t GetWindow(TString filename);
   triggerWindow_t GetWindow(std::string filename);

private:

   // Identical methods, jut one using ROOT TString, another - std::string
   void AddWindow(TString filename, uint64_t leftWin, uint64_t rightWin);
   void AddWindow(std::string filename, uint64_t leftWin, uint64_t rightWin);

   // Map of trigger windows: Filename - [left;right]
   std::map<std::string, triggerWindow_t> fWindowMap;

};
