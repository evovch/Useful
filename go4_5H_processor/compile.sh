#!/bin/bash

./doclean.sh

if [ ! -d "build" ]; then
	mkdir build
fi

cd build
cmake ../
make -j
cd -

#cp -v build/src/*.so .
#cp -v build/src/*.pcm .
#cp -v build/src/*.rootmap .
ln -s build/src/libUserAnalysis.so
ln -s build/src/libUserAnalysis_rdict.pcm
ln -s build/src/libUserAnalysis.rootmap

echo -e "\e[1m\e[32mCompilation finished.\e[0m"
