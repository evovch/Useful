#!/bin/bash

# Cleanup compile results =====================================================

unlink libSetupConfig.so
#unlink libSetupConfig_rdict.pcm
#unlink libSetupConfig.rootmap
unlink setup_config_test

unlink libUserAnalysis.so
unlink libUserAnalysis_rdict.pcm
unlink libUserAnalysis.rootmap

rm -fv compile.out compile.err

if [ -d "build" ]; then
	rm -rf build/*
fi

# Cleanup analysis results ====================================================

rm -fv Go4AnalysisASF.root
rm -fv out.txt err.txt summary.txt
rm -fv outputUnpacking.root outputMonitoring.root outputStep2.root
rm -fv setupCfg.root

echo -e "\e[1m\e[32mCleanup finished.\e[0m"
