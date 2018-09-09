#!/bin/bash

if [ ! -d "build" ]; then
	mkdir build
fi
rm -rf build/*

cd build
cmake ../
#-DCMAKE_EXPORT_COMPILE_COMMANDS=ON
make -j
cp -v polynom ../
cd -
