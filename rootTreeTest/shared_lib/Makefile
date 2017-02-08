PROJNAME=prog

FLAGS=-std=c++11 -Wall -Wextra -Wpedantic -fPIC
INCFLAGS=-I`root-config --incdir`
LDFLAGS=`root-config --libs`
SOLIB=lib$(PROJNAME).so
EXEC=$(PROJNAME).exe

.PHONY: all clean exec

all: main.o classes.o classesdict.o
	g++ main.o classes.o classesdict.o $(FLAGS) $(LDFLAGS) -shared -o $(SOLIB)

main.o :
	g++ -c main.cpp $(FLAGS) $(INCFLAGS)

classes.o :
	g++ -c classes.cpp $(FLAGS) $(INCFLAGS)

classesdict.o : classesdict.cpp
	g++ -c classesdict.cpp $(FLAGS) $(INCFLAGS)

# Use rootcint for ROOT5 and rootcling for ROOT6
classesdict.cpp :
	rootcling -f classesdict.cpp -c classes.h LinkDef.h

clean :
	rm -fv *.o classesdict.cpp classesdict_rdict.pcm *.root $(EXEC) $(SOLIB)

exec :
	g++ exeMain.cpp -o $(EXEC) -L. -l$(PROJNAME)
