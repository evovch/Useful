Go4 user library project for Mbs DAQ.


Environment
===========

ROOT and Go4 are required for this library.

First execute your go4login script in the corresponding directory:

> . ./go4login

You can tune the env.sh script to your system and use it instead:

> . ./env.sh


Building
========

Build the user library using a simple shell script which in turn calls for cmake:

> ./compile.sh


LMD file analysis
=================

Tune the run.sh script to your needs and run it:

> ./run.sh


Cleanup
=======

You can remove all generated files by:

> ./doclean.sh

Note that doclean.sh removes both build result and analysis results.


Input data structure
====================

EVENT
-----

An event consists of a header and optionally subevents.
I have no idea if an event has a footer because the MbsAPI,
provided by the Go4 framework, is used and no raw data is accessed at this level.

	event header
	subevent
	subevent
	...
	subevent
	[event footer]

SUBEVENT
--------

Each subevent consists of a header and optionally subsubevents.
Again, I have no idea if a subevent has a footer because the MbsAPI,
provided by the Go4 framework, is used and no raw data is accessed at this level.
The subevent header contains the size of the subevent data payload which is used
to loop over the words in a for-style loop. The pointer to the first word of the
first subsubevent is provided.

	subevent header
	subsubevent
	subsubevent
	...
	subsubevent
	[subevent footer]

SUBSUBEVENT
-----------

One subsubevent is a group of words of the following structure:

	subsubevent header
	data word
	data word
	...
	data word
	subsubevent footer

Subsubevent header and footer each have the length of one word (32 bits).
A data word (32 bits) can be coming from a unit by one of the vendors - CAEN or MESYTEC.
The subsubevent header is used to identify the vendor.
The footer is used only to separate the subsubevents.
It can be (and currently is) used to identify the length of the subsubevent.


Coding convention
=================

Rather official ROOT coding convention
--------------------------------------

Variables starting with 'f' are the class data members.
They are known everywhere inside the class methods.
Do not create local variables starting with 'f'.
This will make your code so hard to understand and create
potential naming clash...

Unofficial coding convention
----------------------------

+ variables starting with 'f' or 'm' are the class data members;

+ variables starting with 'p_' are the parameters of the the method;
in case you want to specify explicitly:

   - starting with 'i_' are the input parameters (usually byval);

   - starting with 'o_' are the output parameters (usually byref);

+ variables starting with v_ are the local variables;

+ variables without any identifiable prefix are usually the local ones -
this is usually the case for simple one-letter names: i, j, k, x, y, z, n, etc...


Short GCC reminder
==================

For more information refer to:

> man gcc

In a normal situation building a project is done in several steps which include per-se compilation and, after that, linkage using **ld**.

File types
----------

**.h** - header, does not neet to be compiled directly;

**.c, .cpp, cxx** - source, to be compiled directly;
    good practice is to first compile each source into a separate object file and then
    build the desired target (exe or so) from the objects files;

**.o** - object file;

**.exe** or no suffix - executable;

**.so** - dynamic library (basically, same as executable but with no entry point - no main());

**.a** - static library.

Most used flags
---------------

**-o [output_file_name]** - specify explicitly the output file name;

**-c** - only compile up to the object file (do not run the linker **ld**);

**-I[path]** - (capital I) specify additional include directory where the compiler searches for the files (usually headers) included in your sources;

**-L[path]** - (capital L) specify additional directory where the **linker** searches for the libraries;

**-l[lib_name]** - (small l) library name atached during linkage. The 'lib' prefix and the '.so' suffix of the lib_name should be both omitted;

**-shared** - create a dynamic library, not executable;

**-fPIC** - the 'position-independent-code' flag; when creating dynamic libraries the sources have to be compiled with this flag;

**-std=c++11** - enable C++11 support;

**-Wall** - enables all the warnings about constructions that some users consider questionable, and that are easy to avoid;

**-Wextra** - enables some extra warning flags that are not enabled by **-Wall**;

**-Wpedantic** - issue all the warnings demanded by strict ISO C and ISO C++;

A command wirtten inside the ` quotes is executed and its result is put in the place.


rootcint/rootcling
==================

For rootcling (or older rootcint) see official site, there's not so much to read:
https://root.cern.ch/cling


Short Makefile structure reminder
=================================

	.PHONY: list of fake targets (not files)

	target: prerequisites
	(tab character) command to obtain the target, possibly, but not necessarily, using the prerequisites
	(tab character) command2
	...
	(tab character) commandN

Automatic variables:

**$@** - the file name of the target of the rule.

**$^** - the names of all the prerequisites, with spaces between them.

See also:
https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
