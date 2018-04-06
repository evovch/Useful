GO4DIR=/home/evovch/soft/go4-5.3.0

TARGET=prog2
FLAGS=-DLinux

.PHONY: all clean

#all: main.o
#	g++ $(FLAGS) RawAPI/*.o MbsAPIbase/*.o MbsAPI/*.o main.o -o $(TARGET)

all: main.o
	g++ $(FLAGS) main.o -o $(TARGET) -L$(GO4DIR)/lib -lGo4Analysis

main.o:
	gcc -c main.c $(FLAGS) -I$(GO4DIR)/include

clean:
	rm -fv $(TARGET) main.o
