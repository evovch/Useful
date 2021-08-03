/**
Small test of two options to calculate
nmul: -x*y
 fma: x*y*+z
*/

#include <immintrin.h>
#include <cstdio>

class float4
{
public:
	__m128 _data_simd;
public:
	explicit float4(const __m128& data_simd) : _data_simd(data_simd) {}
	explicit float4(const float& v0, const float& v1, const float& v2,
		const float& v3) : _data_simd(_mm_set_ps(v0, v1, v2, v3)) {}
	~float4(void) = default;
public:
	void Print(void) const {
		float x_[4];
		_mm_store_ps(x_, _data_simd);
		printf("%-0.4f\t%-0.4f\t%-0.4f\t%-0.4f\n", x_[3], x_[2], x_[1], x_[0]);
	}
public:
	float4 operator+(const float4& op2) const {
		return float4(_mm_add_ps(_data_simd, op2._data_simd)); }
	float4 operator*(const float4& op2) const {
		return float4(_mm_mul_ps(_data_simd, op2._data_simd)); }
public:
	friend float4 Nmul(const float4& x, const float4& y);
	friend float4 Nmul2(const float4& x, const float4& y);
	friend float4 Fmadd(const float4& x, const float4& y, const float4& z);
	friend float4 Fmadd2(const float4& x, const float4& y, const float4& z);
};

float4 Nmul(const float4& x, const float4& y) {
	return float4(_mm_fnmadd_ps(x._data_simd, y._data_simd, _mm_set1_ps(0.0f)));
}

float4 Nmul2(const float4& x, const float4& y) {
	return float4(_mm_mul_ps(_mm_xor_ps(x._data_simd, _mm_set1_ps(-0.f)), y._data_simd));
}

float4 Fmadd(const float4& x, const float4& y, const float4& z) {
	return float4(_mm_fmadd_ps(x._data_simd, y._data_simd, z._data_simd));
}

float4 Fmadd2(const float4& x, const float4& y, const float4& z) {
	return x * y + z;
}

int main(int argc, char** argv)
{
	const float4 a(0.1f, 0.2f, 0.3f, 0.4f);
	const float4 b(0.5f, 0.6f, 0.7f, 0.8f);
	const float4 c(0.01f, 0.02f, 0.03f, 0.04f);
	const float4 mab = Nmul(a, b);
	const float4 mab2 = Nmul2(a, b);
	const float4 abc = Fmadd(a, b, c);
	const float4 abc2 = Fmadd2(a, b, c);
	a.Print();
	b.Print();
	mab.Print();
	mab2.Print();
	abc.Print();
	abc2.Print();
}
