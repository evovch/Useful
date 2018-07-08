#include <cstdlib>
#include <ctime>
#include <iostream>

__device__
unsigned int Nmax = 10000;

template <typename TYPE>
__device__
TYPE myAbs(TYPE x)
{
	if (x >= static_cast<TYPE>(0.)) {
		return x;
	} else {
		return -x;
	}
}

template <typename TYPE>
__device__
TYPE func(TYPE x)
{
	return x*x*x + static_cast<TYPE>(2.3);
}

template <typename TYPE>
__device__
TYPE func_deriv(TYPE x)
{
	return static_cast<TYPE>(3.)*x*x;
}

template <typename TYPE>
__global__
void solve(TYPE epsilon, TYPE* x0, TYPE* xi)
{
	int j = blockIdx.x*blockDim.x + threadIdx.x;
	xi[j] = x0[j];
	for (unsigned int iIter=1; iIter<Nmax; iIter++) {
		TYPE diff = func(xi[j]) / func_deriv(xi[j]);
		//std::cout << "iteration " << iIter << "\t"
		//          << "diff=" << diff << std::endl;
		if (myAbs(diff) < epsilon) {
			return; // or 'break' to be more precise
		}
		xi[j] -= diff;
	}
}

template <typename TYPE>
void run(void)
{
	TYPE epsilon = static_cast<TYPE>(0.000001);
	//TYPE x0 = static_cast<TYPE>(200.)*uniRnd - static_cast<TYPE>(100.);

	TYPE* host_initVec;
	TYPE* host_solutionVec;
	TYPE* dev_initVec;
	TYPE* dev_solutionVec;

	unsigned int Nthreads = 128;

	host_initVec = (TYPE*)malloc(Nthreads*sizeof(TYPE));
	host_solutionVec = (TYPE*)malloc(Nthreads*sizeof(TYPE));
	cudaError_t err1 = cudaMalloc(&dev_initVec, Nthreads*sizeof(TYPE));
	cudaError_t err2 = cudaMalloc(&dev_solutionVec, Nthreads*sizeof(TYPE));

	if (err1 || err2) {
		std::cerr << "Failed to allocate memory." << std::endl;
		exit(EXIT_FAILURE);
	}

	for (unsigned int i=0; i<Nthreads; i++) {
		TYPE uniRnd = static_cast<TYPE>(rand()) / static_cast<TYPE>(RAND_MAX);
		host_initVec[i] = static_cast<TYPE>(200.)*uniRnd - static_cast<TYPE>(100.);
	}

	cudaMemcpy(dev_initVec, host_initVec, Nthreads*sizeof(TYPE), cudaMemcpyHostToDevice);

	solve<<<4,32>>>(epsilon, dev_initVec, dev_solutionVec);

	//cudaDeviceSynchronize();

	cudaMemcpy(host_solutionVec, dev_solutionVec, Nthreads*sizeof(TYPE), cudaMemcpyDeviceToHost);

	for (unsigned int i=0; i<Nthreads; i++) {
		std::cout << "x0[" << i << "]=" << host_initVec[i] << "\t"
		          << " x[" << i << "]=" << host_solutionVec[i]
		          << std::endl;
	}

	cudaFree(dev_initVec);
	cudaFree(dev_solutionVec);
	free(host_initVec);
	free(host_solutionVec);
}

int main(int argc, char** argv)
{
	srand(time(NULL));
	run<float>();
	return 0;
}
