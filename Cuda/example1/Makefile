FLAGS=-g

.PHONY : clean

example1 : example1.cu
	nvcc ${FLAGS} $^ -o $@ --gpu-architecture=sm_50

clean :
	rm -fv example1
