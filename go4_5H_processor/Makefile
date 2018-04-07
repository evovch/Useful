#ifndef GO4SYS
#GO4SYS = $(shell go4-config --go4sys)
#endif
#include $(GO4SYS)/Makefile.config

# This line forces 'make' to compile in many thread
MAKEFLAGS+="-j -l $(shell grep -c ^processor /proc/cpuinfo)"

FLAGS=-std=c++11 -fPIC

# Keep 'lib' prefix and '.so' suffix
LIBNAME=libUserAnalysis.so

.PHONY: all clean

all: $(LIBNAME)

$(LIBNAME): G__UserAnalysis.o UserAnalysis.o UserEvent.o UserProc.o UserEventStep2.o UserProcStep2.o UserAnalysisHistos.o
	g++ $(FLAGS) $^ -shared -o $(LIBNAME)

G__UserAnalysis.o: G__UserAnalysis.cxx
	g++ $(FLAGS) -c $^ -I$(GO4SYS)/include -I`root-config --incdir`

G__UserAnalysis.cxx: UserAnalysis.h UserEvent.h UserProc.h UserEventStep2.h UserProcStep2.h UserAnalysisHistos.h UserAnalysisLinkDef.h
	rootcling -f $@ -I$(GO4SYS)/include $^

UserAnalysis.o: UserAnalysis.cxx
	g++ $(FLAGS) -c $^ -I$(GO4SYS)/include -I`root-config --incdir`

UserEvent.o: UserEvent.cxx
	g++ $(FLAGS) -c $^ -I$(GO4SYS)/include -I`root-config --incdir`

UserProc.o: UserProc.cxx
	g++ $(FLAGS) -c $^ -I$(GO4SYS)/include -I`root-config --incdir`

UserEventStep2.o: UserEventStep2.cxx
	g++ $(FLAGS) -c $^ -I$(GO4SYS)/include -I`root-config --incdir`

UserProcStep2.o: UserProcStep2.cxx
	g++ $(FLAGS) -c $^ -I$(GO4SYS)/include -I`root-config --incdir`

UserAnalysisHistos.o: UserAnalysisHistos.cxx
	g++ $(FLAGS) -c $^ -I$(GO4SYS)/include -I`root-config --incdir`

clean:
	rm -fv *.o $(LIBNAME) G__UserAnalysis.cxx G__UserAnalysis_rdict.pcm


# Short Makefile structure reminder:
#
# .PHONY: list of fake targets (not files)
#
# target: prerequisites
# (tab character) command to obtain the target, possibly, but not necessarily, using the prerequisites
# (tab character) command2
# ...
# (tab character) commandN
#
# Automatic variables:
# $@ - the file name of the target of the rule.
# $^ - the names of all the prerequisites, with spaces between them.
#
# See also:
# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html


# Short GCC reminder:
#
# Default compilation consists of a few steps which include per-se compilation and, after that, linkage using ld
#
# File types:
# .h - header, does not neet to be compiled directly
# .c, .cpp, cxx - source, to be compiled directly
#     Good practice is to first compile each source into a separate object file and then
#     build the desired target (exe or so) from the objects files
# .o - object file
# .exe or no suffix - executable
# .so - dynamic library (basically, same as executable but with no entry point)
# .a - static library
#
# Most used flags:
# -o output_file_name - specify explicitly the output file name
# -c - only compile up to the object file (do not run the linker)
# -I[path] - (capital I) specify additional include directory where the compiler searches for the files (usually headers) included in your sources
# -L[path] - (capital L) specify additional directory where the _linker_ searches for the libraries
# -l[lib_name] - (small l) library name atached during linkage. The 'lib' prefix and the '.so' suffix should be both omitted
# -shared - create a dynamic library, not executable
# When creating dynamic libraries the sources have to be compiled with the 'position-independent-code' flag -fPIC.
#
# A command wirtten inside the ` quotes is executed and the result is put in the place.


# For rootcling (or older rootcint) see official site, there's not so much to read:
# https://root.cern.ch/cling
