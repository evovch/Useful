#!/bin/bash

rm -f Go4AnalysisASF.root

go4analysis -v -lib libUserAnalysis.so -number 100000 -asf Go4AnalysisASF.root -file /home/evovch/Documents/h5_02_0005.lmd > out.txt 2> err.txt