ROOT and Go4 are required for this library.

First execute your go4login script in the corresponding directory:

> . ./go4login

Then build using a simple shell script which in turn calls for cmake:

> ./compile.sh

An alternative way to build the library is using the supplied Makefile:

> make

You can clean up after 'make' by:

> make clean

You can clean up after cmake (or compile.sh) by deleting the build directory, the so library and the pcm file:

> rm -rf build *.so *.pcm
