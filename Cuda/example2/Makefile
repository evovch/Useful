FLAGS=-g

.PHONY : clean

example2 : example2.cu
	nvcc ${FLAGS} $^ -o $@ --gpu-architecture=sm_50

clean :
	rm -fv example2
