{
	TString myGO4SYS("/home/evovch/soft/go4-5.3.0");

	cout << gSystem->GetIncludePath() << endl << endl;
	gSystem->Load(myGO4SYS + "/lib/libGo4Analysis.so");
	gSystem->Load("libUserAnalysis.so");
	cout << gSystem->GetIncludePath() << endl << endl;
	////gSystem->AddIncludePath(myGO4SYS + "/");
	gSystem->AddIncludePath(myGO4SYS + "/include/");
	cout << gSystem->GetIncludePath() << endl << endl;
	////gInterpreter->AddIncludePath(myGO4SYS + "/");
	gInterpreter->AddIncludePath(myGO4SYS + "/include/");
	cout << gSystem->GetIncludePath() << endl << endl;
	gROOT->ProcessLine(".include " + myGO4SYS + "/include/");
	cout << gSystem->GetIncludePath() << endl << endl;
	gROOT->ProcessLine(".I");
	cout << endl;
}
