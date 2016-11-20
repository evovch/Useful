#!/bin/bash

root -l -b -q "gen.C(10, 10.)"
root -l -b -q "gen.C(20, 5.)"
root -l -b -q "gen.C(40, 2.5)"
root -l -b -q "gen.C(80, 1.25)"

hadd generated_sum.root generated_Nmean*.root

root -l "fit.C(\"generated_sum.root\", 4)"
