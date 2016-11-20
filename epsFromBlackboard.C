void epsFromBlackboard(TString p_filename="blackboard_from_2kHz_data_norm_by_1kHz.root", TString p_outFolder="pictures_png")
{
	TFile* inFile = new TFile(p_filename, "READ");

	TList* theList = inFile->GetListOfKeys();
	TIter next(theList);
	//theList->Print();

	TString outFilename;

	while (TKey* theKey = (TKey*)next()) {
		TObject* theObj = theKey->ReadObj();
		cout << theObj->GetName() << endl;
		if (theObj->InheritsFrom("TCanvas")) {
			cout << "!!!" << endl;

			outFilename.Form("%s/%s.eps", p_outFolder.Data(), theObj->GetName());
			TCanvas* curCanv = (TCanvas*)theObj;
			curCanv->SaveAs(outFilename);

		}
	}

	/*while (TObject* theObj = next()) {
		cout << theObj->GetName() << endl;
		if (theObj->InheritsFrom("TKey")) {
			cout << "!!!" << endl;
		}
	}*/

}
