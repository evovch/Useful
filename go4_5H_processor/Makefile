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

# See README.txt for information
