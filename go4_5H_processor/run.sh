#!/bin/bash

INPUTFILE=/home/evovch/Documents/h5_02_0005.lmd
#INPUTFILE=/home/evovch/Downloads/FLNR_data/he6_w3_d1-205,02_d2-182,55_0001.lmd

rm -f Go4AnalysisASF.root

echo -e "\e[1m\e[32mAnalysis started.\e[0m"

go4analysis -v -lib libUserAnalysis.so -number 100 -asf Go4AnalysisASF.root -file ${INPUTFILE} > out.txt 2> err.txt
# -number 1000

echo -e "\e[1m\e[32mAnalysis finished.\e[0m"
