FLAGS=-Wall -Wextra -Wpedantic

FLAGS+=-g
FLAGS+=-pg

.PHONY: clean

prog: main.o
	g++ $^ -o $@ $(FLAGS)

main.o: main.cpp
	g++ -c $^ $(FLAGS)

clean:
	rm -fv prog
	rm -fv *.o
