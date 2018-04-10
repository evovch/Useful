{
	gSystem->Load("${GO4SYS}/lib/libGo4Analysis.so");
	gSystem->Load("libUserAnalysis.so");
//	gInterpreter->AddIncludePath("${GO4SYS}/");
	gInterpreter->AddIncludePath("${GO4SYS}/include/");
}
