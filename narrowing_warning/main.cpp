#include <cstdio>

int main(int /*argc*/, char** /*argv*/)
{
	float x[3] = { 1,      2.,      3.f };

	float y[3] = { x[0]+1, x[1]+2., x[2]+3.f };

	printf("x: %f\t%f\t%f\n", x[0], x[1], x[2]);
	printf("y: %f\t%f\t%f\n", y[0], y[1], y[2]);

	return 0;
}
