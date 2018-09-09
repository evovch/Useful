FLAGS=-g

.PHONY : clean

example3 : example3.cu
	nvcc ${FLAGS} $^ -o $@ --gpu-architecture=sm_50

clean :
	rm -fv example3
