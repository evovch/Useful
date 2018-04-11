#!/bin/bash

./doclean.sh

if [ ! -d "build" ]; then
	mkdir build
fi

cd build
cmake ../
make -j
cd -

cp -v build/*.so .
cp -v build/*.pcm .
cp -v build/*.rootmap .

echo -e "\e[1m\e[32mCompilation finished.\e[0m"
