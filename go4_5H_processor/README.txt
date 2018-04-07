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



/**
	Rather official ROOT coding convention:
	Variables starting with 'f' are the class data members.
	They are known everywhere inside the class methods.
	Do not create local variables starting with 'f'.
	This will make your code so hard to understand and create
	potential naming clash...

	Unofficial coding convention:
	variables starting with 'f' or 'm' are the class data members;
	variables starting with 'p_' are the parameters of the the method;
		in case you want to specify explicitly:
		starting with 'i_' are the input parameters (usually byval);
		starting with 'o_' are the output parameters (usually byref);
	variables starting with v_ are the local variables.
	variables without any identifiable prefix are usually the local ones -
		thid is usually the case for simple one-letter names:
		i, j, k, x, y, z, n, etc...
*/
