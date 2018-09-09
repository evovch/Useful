#include <stdio.h> /* for fprintf */

#include <TFile.h>

#include "structs.h"
#include "functions.h"

int main(int /*argc*/, char** /*argv*/)
{
	stc_setup_config setupConfigObj;

	InitStcSetupConfig(&setupConfigObj);

	TFile* outputFile = new TFile("output.root", "RECREATE");
	setupConfigObj.Write();
	outputFile->Close();

	// DestructStcSetupConfig(&setupConfigObj); // ?

	return 0;
}
