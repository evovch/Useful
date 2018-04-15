#!/bin/bash

#rm -fv libUserAnalysis.so libUserAnalysis_rdict.pcm libUserAnalysis.rootmap
unlink libUserAnalysis.so
unlink libUserAnalysis_rdict.pcm
unlink libUserAnalysis.rootmap

rm -fv out.txt err.txt
rm -fv summary.txt
rm -fv outputUnpacking.root outputMonitoring.root outputStep2.root
rm -fv Go4AnalysisASF.root

if [ -d "build" ]; then
	rm -rf build/*
fi

echo -e "\e[1m\e[32mCleanup finished.\e[0m"
