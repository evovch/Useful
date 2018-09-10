#!/bin/bash

./asd
./planar_draw 2> err.txt
cat err.txt | grep "\[DEBUG4\] 0" | cut -d" " -f7 > testData.txt
cp testData.txt ../../B_spline_basis/
