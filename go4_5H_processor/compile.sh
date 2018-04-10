#!/bin/bash

if [ ! -d "build" ]; then
	mkdir build
fi

cd build
rm -rf *
cmake ../
make -j
cd -

cp -v build/*.so .
cp -v build/*.pcm .
cp -v build/*.rootmap .

echo "Finished."
