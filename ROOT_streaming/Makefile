FLAGS=-std=c++11 -Wall -Wextra -Wpedantic -fPIC
INCFLAGS=-I`root-config --incdir`
LDFLAGS=-L`root-config --libdir`
LDFLAGS+=-lCore -lRIO

LIBNAME=libClasses

.PHONY: clean

prog: main.o functions.o $(LIBNAME).so
	g++ main.o functions.o -o $@ $(LDFLAGS) -L. -lClasses

main.o: main.cpp
	g++ -c $^ $(FLAGS) $(INCFLAGS)

functions.o: functions.cpp
	g++ -c $^ $(FLAGS) $(INCFLAGS)

$(LIBNAME).so: classesdict.o
	g++ $^ -o $@ -fPIC -shared

classesdict.o: classesdict.cpp
	g++ -c $^ $(FLAGS) $(INCFLAGS)

classesdict.cpp:
	rootcling -f $@ -s $(LIBNAME).so -rml $(LIBNAME).so -rmf $(LIBNAME).rootmap structs.h SetupConfigLinkDef.h

clean:
	rm -fv *.o
	rm -fv classesdict.cpp $(LIBNAME)_rdict.pcm $(LIBNAME).rootmap
	rm -fv $(LIBNAME).so
	rm -fv prog
	rm -fv output.root
