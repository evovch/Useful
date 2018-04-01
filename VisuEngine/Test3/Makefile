MAKEFLAGS+="-j -l $(shell grep -c ^processor /proc/cpuinfo)"

COMPILER=g++
TARGET=hbrep

DEBUGFLAG=-g

FLAGS=-Wall -Wextra -Wpedantic $(DEBUGFLAG)
LIBS=-lGL -lGLEW -lglut

.PHONY: all clean

all: main.o cls_model.o cls_camera.o support.o myTypes.o cls_axis2_placement_3d.o cls_cylinder.o
	$(COMPILER) $(FLAGS) main.o cls_model.o cls_camera.o support.o myTypes.o cls_axis2_placement_3d.o cls_cylinder.o -o $(TARGET) $(LIBS)

main.o:
	$(COMPILER) $(FLAGS) main.cpp -c

cls_model.o:
	$(COMPILER) $(FLAGS) cls_model.cpp -c

cls_camera.o:
	$(COMPILER) $(FLAGS) cls_camera.cpp -c

support.o:
	$(COMPILER) $(FLAGS) support.cpp -c

myTypes.o:
	$(COMPILER) $(FLAGS) myTypes.cpp -c

cls_axis2_placement_3d.o:
	$(COMPILER) $(FLAGS) cls_axis2_placement_3d.cpp -c

cls_cylinder.o:
	$(COMPILER) $(FLAGS) cls_cylinder.cpp -c

clean:
	rm -vf *.o $(TARGET)
