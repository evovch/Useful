TARGET=prog

FLAGS=-std=c++11 -Wall -Wextra -Wpedantic

.PHONY: all clean

all: main.o cls_stl_file.o cls_stl_interface.o stl_parsing.flex.o stl_parsing.tab.o
	g++ $(FLAGS) main.o cls_stl_file.o cls_stl_interface.o stl_parsing.flex.o stl_parsing.tab.o -o $(TARGET)

main.o:
	g++ $(FLAGS) main.cpp -c

cls_stl_file.o:
	g++ $(FLAGS) cls_stl_file.cpp -c

cls_stl_interface.o:
	g++ $(FLAGS) cls_stl_interface.cpp -c

stl_parsing.flex.o: stl_parsing.flex.cpp
	g++ $(FLAGS) stl_parsing.flex.cpp -c

stl_parsing.tab.o: stl_parsing.tab.cpp
	g++ $(FLAGS) stl_parsing.tab.cpp -c

stl_parsing.flex.cpp: stl_parsing.tab.cpp
	flex --header-file=stl_parsing.flex.h -o stl_parsing.flex.cpp stl_parsing.l

stl_parsing.tab.cpp:
	bison --defines=stl_parsing.tab.h -o stl_parsing.tab.cpp stl_parsing.y

clean:
	rm -fv $(TARGET)
	rm -fv *.o
	rm -fv stl_parsing.tab.h stl_parsing.tab.cpp
	rm -fv stl_parsing.flex.h stl_parsing.flex.cpp
