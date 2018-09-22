#!/bin/bash

# Cleanup

if [ ! -d "build" ]; then
	mkdir build
else
	rm -rfv build/*
fi

rm -fv output.root

unlink prog
unlink libUserClasses_rdict.pcm
unlink libUserClasses.rootmap
unlink libUserClasses.so
