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

$(LIBNAME): G__UserAnalysis.o UserAnalysis.o UserEvent.o UserProc.o
	g++ $(FLAGS) G__UserAnalysis.o UserAnalysis.o UserEvent.o UserProc.o -shared -o $(LIBNAME)

G__UserAnalysis.o: G__UserAnalysis.cxx
	g++ $(FLAGS) -c G__UserAnalysis.cxx -I$(GO4SYS)/include -I`root-config --incdir`

G__UserAnalysis.cxx: UserAnalysis.h UserEvent.h UserProc.h UserAnalysisLinkDef.h
	rootcling -f G__UserAnalysis.cxx -I$(GO4SYS)/include UserAnalysis.h UserEvent.h UserProc.h UserAnalysisLinkDef.h

UserAnalysis.o: UserAnalysis.cxx
	g++ $(FLAGS) -c UserAnalysis.cxx -I$(GO4SYS)/include -I`root-config --incdir`

UserEvent.o: UserEvent.cxx
	g++ $(FLAGS) -c UserEvent.cxx -I$(GO4SYS)/include -I`root-config --incdir`

UserProc.o: UserProc.cxx
	g++ $(FLAGS) -c UserProc.cxx -I$(GO4SYS)/include -I`root-config --incdir`

clean:
	rm -fv *.o $(LIBNAME) G__UserAnalysis.cxx G__UserAnalysis_rdict.pcm
