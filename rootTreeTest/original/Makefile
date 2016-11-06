FLAGS=-std=c++11 -Wall -Wextra -Wpedantic
INCFLAGS=-I`root-config --incdir`
LDFLAGS=`root-config --libs`
TARGET=prog

.PHONY: all clean

all: main.o classes.o classesdict.o
	g++ main.o classes.o classesdict.o -o $(TARGET) $(FLAGS) $(LDFLAGS)

main.o :
	g++ -c main.cpp $(FLAGS) $(INCFLAGS)

classes.o :
	g++ -c classes.cpp $(FLAGS) $(INCFLAGS)

classesdict.o : classesdict.cpp
	g++ -c classesdict.cpp $(FLAGS) $(INCFLAGS)

classesdict.cpp :
	rootcint -f classesdict.cpp -c classes.h LinkDef.h

clean :
	rm -fv *.o classesdict.cpp classesdict_rdict.pcm *.root $(TARGET)
