LIBTARGET=libAna.so
EXETARGET=analysis.exe

COMPILER=g++
FLAGS=-Wall -Wextra -Wpedantic -std=c++11

ROOTINCDIR=`root-config --incdir`
ROOTLIBS=`root-config --libs`

.PHONY: clean all

all: $(LIBTARGET) $(EXETARGET)

$(LIBTARGET): AnaTask.o AnaTaskDict.o
	$(COMPILER) AnaTask.o AnaTaskDict.o -shared $(FLAGS) -o $(LIBTARGET)

$(EXETARGET):
	$(COMPILER) main.cpp $(FLAGS) -I$(ROOTINCDIR) $(ROOTLIBS) -L. -lAna -o $(EXETARGET)

AnaTask.o:
	$(COMPILER) AnaTask.cpp -c -fPIC $(FLAGS) -I$(ROOTINCDIR)

AnaTaskDict.o: AnaTaskDict.cpp
	$(COMPILER) AnaTaskDict.cpp -c -fPIC $(FLAGS) -I$(ROOTINCDIR)

AnaTaskDict.cpp:
	rootcling -f AnaTaskDict.cpp -c AnaTask.h AnaTaskLinkDef.h

clean:
	rm -fv *.o *.pcm $(LIBTARGET) $(EXETARGET) AnaTaskDict.cpp
