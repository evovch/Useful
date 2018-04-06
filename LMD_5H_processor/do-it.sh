#!/bin/bash

# Build using cmake

cd build
rm -rf *
cmake ../
make -j
cd -
cp build/prog .

# Build using make

make clean
make

