#!/bin/bash

rm -fv libUserAnalysis.so libUserAnalysis_rdict.pcm libUserAnalysis.rootmap
rm -fv out.txt err.txt
rm -fv output.root output2.root
rm -fv Go4AnalysisASF.root

if [ -d "build" ]; then
	rm -rf build/*
fi

echo -e "\e[1m\e[32mCleanup finished.\e[0m"
