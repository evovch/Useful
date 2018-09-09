#include <iostream>
#include <math.h>

// Kernel function to add the elements of two arrays
__global__
void add(int n, float *x, float *y)
{
	for (int i = 0; i < n; i++) {
		y[i] = x[i] + y[i];
	}
}

int main(void)
{
	int N = 1<<18;
	float *x, *y;

	cudaError_t rez1;
	cudaError_t rez2;

	// Allocate Unified Memory – accessible from CPU or GPU
	rez1 = cudaMallocManaged((void**)&x, N*sizeof(float));
	rez2 = cudaMallocManaged((void**)&y, N*sizeof(float));

	std::cout << "rez1=" << rez1 << std::endl;
	std::cout << "rez2=" << rez2 << std::endl;

	// initialize x and y arrays on the host
	for (int i = 0; i < N; i++) {
		x[i] = 1.0f;
		y[i] = 2.0f;
	}

	// Run kernel on 1M elements on the GPU
	add<<<1, 1>>>(N, x, y);

	// Wait for GPU to finish before accessing on host
	cudaDeviceSynchronize();

	// Check for errors (all values should be 3.0f)
	float maxError = 0.0f;
	for (int i = 0; i < N; i++)
	maxError = fmax(maxError, fabs(y[i]-3.0f));
	std::cout << "Max error: " << maxError << std::endl;

	// Free memory
	cudaFree(x);
	cudaFree(y);

	return 0;
}