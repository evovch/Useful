FLAGS=-std=c++11 -Wall -Wextra -Wpedantic
INCFLAGS=-I`root-config --incdir`
LDFLAGS=`root-config --libs`
TARGET=prog

SOURCES=matrix.cpp matrixTest.C

.PHONY: all clean

all:
	g++ $(SOURCES) -o $(TARGET) $(FLAGS) $(LDFLAGS) $(INCFLAGS)

clean:
	rm -f $(TARGET) *.d *.so *.pcm
