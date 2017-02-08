FLAGS=-std=c++11 -Wall -Wextra -Wpedantic
INCFLAGS=-I`root-config --incdir`
LDFLAGS=`root-config --libs`
TARGET=prog

.PHONY: all clean

all:
	g++ PDF_CDF.C -o $(TARGET) $(FLAGS) $(LDFLAGS) $(INCFLAGS)

clean:
	rm -f $(TARGET) *.d *.so *.pcm
