#!/bin/bash

if [ -z ${GO4SYS+x} ]; then
	echo "GO4SYS is unset. Aborting.";
	exit;
else
	echo "GO4SYS is set to '$GO4SYS'";
fi

# Please specify these variables to your needs ================================

INPUTLMDDIR=/home/evovch/Downloads/FLNR_data/exp201803/
OUTPUTROOTDIR=/home/evovch/Downloads/FLNR_data/exp201803/

SETUPFILE=./usr/setup_exp201803.xml
INPUTFILENAME=he6-no_mult_0001.lmd

# =============================================================================

INPUTFILE=${INPUTLMDDIR}/${INPUTFILENAME}
OUTPUTFILE=${OUTPUTROOTDIR}/${INPUTFILENAME}.root

# Cleanup analysis results ====================================================

# Remove output file if it already exists from previous analysis runs
rm -fv ${OUTPUTFILE}
# Remove autosave file if it already exists from previous analysis runs
rm -fv Go4AnalysisASF.root
# Remove textual output if it already exists from previous analysis runs
rm -fv out.txt err.txt summary.txt

# =============================================================================

echo -e "\e[1m\e[32mAnalysis started.\e[0m"

${GO4SYS}/bin/go4analysis -v -lib libUserAnalysis.so -number 100000 -asf Go4AnalysisASF.root -file ${INPUTFILE} -args ${OUTPUTFILE} ${SETUPFILE} > out.txt 2> /dev/null
# -number 1000
# 2> err.txt

echo -e "\e[1m\e[32mAnalysis finished.\e[0m"
