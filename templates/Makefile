.PHONY: clean

all: prog1 prog2 prog3

prog1: main1.cpp
	g++ $^ -o $@

prog2: main2.cpp
	g++ $^ -o $@

prog3: main3.o libClasses.so
	g++ main3.o -o $@ -L. -lClasses

main3.o: main3.cpp
	g++ -c $^

libClasses.so: baseClass.o childClass.o
	g++ -shared $^ -o $@

baseClass.o: baseClass.cpp
	g++ -c $^ -fPIC

childClass.o: childClass.cpp
	g++ -c $^ -fPIC

clean:
	rm -rf *.so
	rm -rf *.o
	rm -rf prog1 prog2 prog3
