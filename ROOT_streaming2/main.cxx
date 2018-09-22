#include <iostream>
using std::cerr;
using std::endl;

#include <TFile.h>

#include "myClass1.h"

int main(int /*argc*/, char** /*argv*/)
{
	TString outFilename("output.root");
	TFile* outFile = new TFile(outFilename, "RECREATE");

	if (outFile->IsZombie()) {
		cerr << "Error opening file " << outFilename.Data() << ". Aborting." << endl;
		return 1;
	}

	myClass1* obj1 = new myClass1();
	obj1->Generate();
	obj1->Write();

	outFile->Close();

	return 0;
}
