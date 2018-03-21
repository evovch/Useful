COMPILER=g++
TARGET=hbrep

FLAGS=-Wall -Wextra -Wpedantic
LIBS=-lGL -lGLEW -lglut

.PHONY: all clean

all: main.o cls_model.o cls_camera.o support.o
	$(COMPILER) main.o cls_model.o cls_camera.o support.o -o $(TARGET) $(LIBS)

main.o:
	$(COMPILER) main.cpp -c $(FLAGS)

cls_model.o:
	$(COMPILER) cls_model.cpp -c $(FLAGS)

cls_camera.o:
	$(COMPILER) cls_camera.cpp -c $(FLAGS)

support.o:
	$(COMPILER) support.cpp -c $(FLAGS)

clean:
	rm -vf *.o $(TARGET)
