#!/bin/bash

#INPUTFILE=/home/evovch/Documents/exp201805/data/run05_0014.lmd
INPUTFILE=/home/evovch/Downloads/FLNR_data/h5_14_0001.lmd
#INPUTFILE=/home/evovch/Documents/h5_02_0001.lmd
#INPUTFILE=/home/evovch/Downloads/FLNR_data/he6_0001.lmd
#INPUTFILE=/home/evovch/Downloads/FLNR_data/he6_w3_d1-205,02_d2-182,55_0001.lmd
#INPUTFILE=/home/evovch/Downloads/FLNR_data/he6_w3_d1-206,02_d2-182,55_0001.lmd

# Cleanup analysis results ====================================================

rm -fv Go4AnalysisASF.root
rm -fv out.txt err.txt summary.txt
rm -fv outputUnpacking.root outputMonitoring.root outputStep2.root
rm -fv setupCfg.root

echo -e "\e[1m\e[32mAnalysis started.\e[0m"

#TODO check that GO4SYS is declared
${GO4SYS}/bin/go4analysis -v -lib libUserAnalysis.so -number 10 -asf Go4AnalysisASF.root -file ${INPUTFILE} > out.txt 2> err.txt
# -number 1000

echo -e "\e[1m\e[32mAnalysis finished.\e[0m"
