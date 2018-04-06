#!/bin/bash

if [ ! -d "build" ]; then
	mkdir build
fi

cd build
rm -rf *
cmake ../
make -j
cd -

cp build/*.so .

echo "Finished."
