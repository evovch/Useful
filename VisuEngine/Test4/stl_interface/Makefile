COMPILER=g++
TARGET=prog

FLAGS=-std=c++11 -Wall -Wextra -Wpedantic

.PHONY: all clean

all: main.o cls_stl_file.o cls_stl_interface.o stl_parsing.flex.o stl_parsing.tab.o
	$(COMPILER) $(FLAGS) $^ -o $(TARGET)

main.o: main.cpp
	$(COMPILER) $(FLAGS) -c $^

cls_stl_file.o: cls_stl_file.cpp
	$(COMPILER) $(FLAGS) -c $^ -I../

cls_stl_interface.o: cls_stl_interface.cpp
	$(COMPILER) $(FLAGS) -c $^ -I../

stl_parsing.flex.o: stl_parsing.flex.cpp
	$(COMPILER) $(FLAGS) -c $^

stl_parsing.tab.o: stl_parsing.tab.cpp
	$(COMPILER) $(FLAGS) -c $^

stl_parsing.flex.cpp: stl_parsing.l stl_parsing.tab.cpp
	flex --header-file=stl_parsing.flex.h -o $@ stl_parsing.l

stl_parsing.tab.cpp: stl_parsing.y
	bison --defines=stl_parsing.tab.h -o $@ stl_parsing.y

clean:
	rm -fv $(TARGET)
	rm -fv *.o
	rm -fv stl_parsing.tab.h stl_parsing.tab.cpp
	rm -fv stl_parsing.flex.h stl_parsing.flex.cpp
