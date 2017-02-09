PROJNAME=prog

FLAGS=-std=c++11 -Wall -Wextra -Wpedantic -fPIC
INCFLAGS=-I`root-config --incdir`
LDFLAGS=`root-config --libs`
SOLIB=libclasses.so
EXEC=$(PROJNAME).exe
EXEC2=$(PROJNAME)2.exe

.PHONY: all clean

all: $(SOLIB) $(EXEC) $(EXEC2)

$(EXEC2): main.o classes.o classesdict.o
	g++ main.o classes.o classesdict.o $(FLAGS) $(LDFLAGS) -o $(EXEC2)

$(EXEC): main.o
	g++ main.o $(FLAGS) $(LDFLAGS) -o $(EXEC) -L. -lclasses

main.o:
	g++ -c main.cpp $(FLAGS) $(INCFLAGS)


# Use rootcint for ROOT5 and rootcling for ROOT6
classesdict.cpp:
	rootcling -f classesdict.cpp -c classes.h LinkDef.h

classesdict.o: classesdict.cpp
	g++ -c classesdict.cpp $(FLAGS) $(INCFLAGS)

classes.o:
	g++ -c classes.cpp $(FLAGS) $(INCFLAGS)

$(SOLIB): classes.o classesdict.o
	g++ classes.o classesdict.o -shared $(FLAGS) -o $(SOLIB)


clean:
	rm -fv *.o classesdict.cpp classesdict_rdict.pcm *.root $(SOLIB) $(EXEC) $(EXEC2)
