#include <immintrin.h>

#include <iostream>
using std::cout;
using std::endl;

// ----------------------------------------------------------------------------

typedef struct A_ {
	float k;
	float y;
	//int ind;
	float b;
	A_(const float& ik, const float& iy, const float& ib) {
		k = ik;
		y = iy;
		//ind = (y < 0.0f) ? 1 : 0;
		b = ib;
	}
} A;

typedef struct B_ {
	float x[2];
	B_(const float& x0, const float& x1) : x{ x0, x1 } {}
} B;

float func(const A& objA, const B& objB) {
	const int ind = (objA.y < 0.0f) ? 1 : 0;
	return objA.k * objB.x[ind] + objA.b;
}

// ----------------------------------------------------------------------------

class float4 {
public:
	__m128 _data;
	float4(void) : _data(_mm_set1_ps(0.0f)) {}
	float4(const float& val) : _data(_mm_set1_ps(val)) {}
	float4(const float* const ptr) : _data(_mm_load_ps(ptr)) {}
	float4(const __m128& data) : _data(data) {}
	friend __forceinline
	float4 Fmadd(const float4& k, const float4& x, const float4& b);
	void Print(void) const {
		float raw[4];
		_mm_store_ps(raw, _data);
		cout << raw[0] << "\t" << raw[1] << "\t" << raw[2] << "\t" << raw[3] << endl;
	}
};

__forceinline
float4 Fmadd(const float4& k, const float4& x, const float4& b) {
	return float4(_mm_fmadd_ps(k._data, x._data, b._data));
}

typedef struct A4_ {
	float4 k;
	float4 y;
	//__m128i ind;
	float4 b;
	A4_(const float* const ik, const float* const iy, const float* const ib) {
		k = float4(ik);
		y = float4(iy);
		//ind = (y < 0.0f) ? 1 : 0;
		//const float4 zero(0.0f);
		//const __m128 mask = _mm_cmplt_ps(y._data, zero._data);
		//ind = _mm_and_si128(_mm_castps_si128(mask), _mm_set1_epi32(1));
		b = float4(ib);
	}
} A4;

typedef struct B4_ {
	float4 x[2];
	B4_(const float& x0, const float& x1) {
		x[0] = float4(x0);
		x[1] = float4(x1);
	}
	/*B4_(const float* const x0, const float* const x1) {
		x[0] = float4(x0);
		x[1] = float4(x1);
	}*/
} B4;

float4 func4(const A4& objA, const B4& objB) {
	const float4 zero(0.0f);
	const __m128 mask1 = _mm_cmplt_ps(objA.y._data, zero._data);
	const __m128 a0 = _mm_andnot_ps(mask1, objB.x[0]._data);
	const __m128 a1 = _mm_and_ps(mask1, objB.x[1]._data);
	const float4 a = float4(_mm_or_ps(a0, a1));
	return Fmadd(objA.k, a, objA.b);
}

// ----------------------------------------------------------------------------

int main(int argc, char** argv)
{
	// Scalar
	const A objA1(2.0f,  1.0f, 3.3f);
	const A objA2(2.0f, -1.0f, 3.3f);
	const B objB(1.1f, 2.2f);
	const float z1 = func(objA1, objB);
	const float z2 = func(objA2, objB);

	cout << "Scalar" << endl;
	cout << "z1=" << z1 << endl;
	cout << "z2=" << z2 << endl;

	// Vector
	const float kk[4]  = { 2.0f, 2.0f, 2.0f, 2.0f };
	const float yy1[4] = { 1.0f, 0.0f,-1.0f,-2.0f };
	const float yy2[4] = {-2.0f,-1.0f, 0.0f, 1.0f };
	const float bb[4]  = { 3.3f, 3.3f, 3.3f, 3.3f };
	const float x0 = 1.1f;
	const float x1 = 2.2f;
	//const float xx0[4] = { 1.1f, 1.1f, 1.1f, 1.1f };
	//const float xx1[4] = { 2.2f, 2.2f, 2.2f, 2.2f };

	const A4 objA41(kk, yy1, bb);
	const A4 objA42(kk, yy2, bb);
	const B4 objB4(x0, x1);

	const float4 z41 = func4(objA41, objB4);
	const float4 z42 = func4(objA42, objB4);

	cout << "Vector" << endl;
	z41.Print();
	z42.Print();
}
