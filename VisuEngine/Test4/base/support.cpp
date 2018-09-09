#include "support.h"

// STD
#include <cstring>

float UnitToScaleFactor(const char* p_unit)
{
	if (strcmp(p_unit, "mm") == 0) {
		return 1.;
	}
	if (strcmp(p_unit, "cm") == 0) {
		return 10.;
	}
	if (strcmp(p_unit, "m") == 0) {
		return 1000.;
	}
	/*if (strcmp(p_unit, "deg") == 0) {
		return 1.;
	}
	if (strcmp(p_unit, "rad") == 0) {
		//TOD implement
	}*/
	return 1.;
}

void ValueToColor(float p_inVal, float p_inValMin, float p_inValMax, stc_VandC* o_destination)
{
	float v_x = (p_inVal - p_inValMin) / (p_inValMax - p_inValMin);

	if (v_x >= 0.0f && v_x <= 0.5f) {
		o_destination->c[0] = 0.0f;
		o_destination->c[1] = 2.0f * v_x;
		o_destination->c[2] = 1.0f - 2.0f * v_x;
	} else {
		if (v_x <= 1.0f) {
			o_destination->c[0] = -1.0f + 2.0f * v_x;
			o_destination->c[1] = 2.0f - 2.0f * v_x;
			o_destination->c[2] = 0.0f;
		}
	}
}

void IntToColor(unsigned int p_inVal, stc_VandC* o_destination)
{
	unsigned int v_R, v_G, v_B;

	v_R = (p_inVal & 0x000000FF) >> 0;
	v_G = (p_inVal & 0x0000FF00) >> 8;
	v_B = (p_inVal & 0x00FF0000) >> 16;

	o_destination->c[0] = (float)v_R / 255.0f;
	o_destination->c[1] = (float)v_G / 255.0f;
	o_destination->c[2] = (float)v_B / 255.0f;
}

void IntToColorUnique(unsigned int p_inVal, stc_VandC* o_destination)
{
	unsigned int v_R, v_G, v_B;

	v_R = (p_inVal & 0x000000FF) >> 0;
	v_G = (p_inVal & 0x0000FF00) >> 8;
	v_B = (p_inVal & 0x00FF0000) >> 16;

	o_destination->c_unique[0] = (float)v_R / 255.0f;
	o_destination->c_unique[1] = (float)v_G / 255.0f;
	o_destination->c_unique[2] = (float)v_B / 255.0f;
}

unsigned int ColorToInt(stc_VandC* p_source)
{
	unsigned int v_R, v_G, v_B;

	v_R = (unsigned int)(p_source->c[0] * 255.0f);
	v_G = (unsigned int)(p_source->c[1] * 255.0f);
	v_B = (unsigned int)(p_source->c[2] * 255.0f);

	return v_R + v_G*256 + v_B*256*256;
}

unsigned int ColorUniqueToInt(stc_VandC* p_source)
{
	unsigned int v_R, v_G, v_B;

	v_R = (unsigned int)(p_source->c_unique[0] * 255.0f);
	v_G = (unsigned int)(p_source->c_unique[1] * 255.0f);
	v_B = (unsigned int)(p_source->c_unique[2] * 255.0f);

	return v_R + v_G*256 + v_B*256*256;
}

unsigned int PixelColorToInt(unsigned char* p_source)
{
	return p_source[0] + p_source[1]*256 + p_source[2]*256*256;
}

// Lillte-endian
unsigned int FourCharsToUintLE(const unsigned char* p_mem)
{
    ////fprintf(stderr, "%02x %02x %02x %02x\n", p_mem[0], p_mem[1], p_mem[2], p_mem[3]);
	unsigned int v_res = (p_mem[3] << 24) | (p_mem[2] << 16) | (p_mem[1] << 8) | (p_mem[0]);
	return v_res;
}

/*
// Big-endian
unsigned int FourCharsToUintBE(const unsigned char* p_mem)
{
	unsigned int v_res = (p_mem[0] << 24) | (p_mem[1] << 16) | (p_mem[2] << 8) | (p_mem[3]);
	return v_res;
}
*/

float FourCharsToFloat(const unsigned char* p_mem)
{
	unsigned int v_res = (p_mem[3] << 24) | (p_mem[2] << 16) | (p_mem[1] << 8) | (p_mem[0]);
	return *((float*)&v_res); //TODO hack!
}

unsigned int Factorial(unsigned int p)
{
	if (p==0 || p==1) return 1;
	unsigned int rez = 1;
	for (unsigned int i=2; i<=p; i++) {
		rez *= i;
	}
	return rez;
}

//TODO implement computing several coefficients simultaneously
unsigned int BinomCoef(unsigned int n, unsigned int k)
{
	return Factorial(n) / (Factorial(k) * Factorial(n-k));
}

/*template <typename TYPE>
TYPE BernsteinCoef(TYPE t, unsigned int n, unsigned int i)
{
	return BinomCoef(n, i) * mypow((static_cast<TYPE>(1.)-t), static_cast<TYPE>(n-i)) *
	                         mypow(static_cast<TYPE>(t), static_cast<TYPE>(i));
}*/

// c++11 has overloaded pow for double and float

float BernsteinCoef(float t, unsigned int n, unsigned int i)
{
	return BinomCoef(n, i) * pow((1.f-t), static_cast<float>(n-i)) *
	                         pow(static_cast<float>(t), static_cast<float>(i));
}

double BernsteinCoef(double t, unsigned int n, unsigned int i)
{
	return BinomCoef(n, i) * pow((1.-t), static_cast<double>(n-i)) *
	                         pow(static_cast<double>(t), static_cast<double>(i));
}
