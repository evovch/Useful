#!/bin/bash

#rm -fv libUserAnalysis.so libUserAnalysis_rdict.pcm libUserAnalysis.rootmap
#rm -fv libSetupConfig.so libSetupConfig_rdict.pcm libSetupConfig.rootmap
unlink libUserAnalysis.so
unlink libUserAnalysis_rdict.pcm
unlink libUserAnalysis.rootmap
unlink libSetupConfig.so
unlink libSetupConfig_rdict.pcm
unlink libSetupConfig.rootmap

unlink setupconfigtest

rm -fv Go4AnalysisASF.root
rm -fv out.txt err.txt summary.txt
rm -fv outputUnpacking.root outputMonitoring.root outputStep2.root

rm -fv setupCfg.root

if [ -d "build" ]; then
	rm -rf build/*
fi

echo -e "\e[1m\e[32mCleanup finished.\e[0m"
