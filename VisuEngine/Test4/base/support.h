#ifndef SUPPORT_H
#define SUPPORT_H

// STD
#include <cmath>
#include <string>

// OpenGL
#include <GL/glew.h>

struct stc_VandC {
	float v[3];
	float c[3];
    float c_unique[3];
};

enum action_type {
    ACT_NO_ACT,
    ACT_PAN,
    ACT_ZOOM,
    ACT_ROTATE,
    ACT_TILT
    /*ACT_SECTIONMOVE,
    ACT_SECTIONROTATE,
    ACT_SECTIONTILT*/
};

/**
*/
float UnitToScaleFactor(const char* p_unit);

/**
    Transform the value of some field into the color
    Blue is minimum, green is 50% and red is maximum
*/
void ValueToColor(float p_inVal, float p_inValMin, float p_inValMax, stc_VandC* o_destination);

/**
    Transform unique number of the triangle into unique color
    Color is coded by 3 floats, each [0.0; 1.0]
*/
void IntToColor(unsigned int p_inVal, stc_VandC* o_destination);

/**
    Transform unique number of the triangle into unique color
    Color is coded by 3 floats, each [0.0; 1.0]
    Write into the c_unique fields
*/
void IntToColorUnique(unsigned int p_inVal, stc_VandC* o_destination);

/**
    Transform unique color into unique number of the triangle
    Color is coded by 3 floats, each [0.0; 1.0]
*/
unsigned int ColorToInt(stc_VandC* p_source);

/**
    Transform unique color into unique number of the triangle
    Color is coded by 3 floats, each [0.0; 1.0]
    Read from the c_unique fields
*/
unsigned int ColorUniqueToInt(stc_VandC* p_source);

/**
    Transform unique color into unique number of the triangle
    Color is coded by 3 bytes, each [0; 255]
*/
unsigned int PixelColorToInt(GLubyte* p_source);

/**
    Lillte-endian
*/
unsigned int FourCharsToUintLE(const unsigned char* p_mem);

/**
    Big-endian
*/
//unsigned int FourCharsToUintBE(const unsigned char* p_mem); //TODO test

/**
*/
float FourCharsToFloat(const unsigned char* p_mem);

/**
*/
unsigned int Factorial(unsigned int p);

/**
*/
unsigned int BinomCoef(unsigned int n, unsigned int k);

/**
*/
/*template <typename TYPE>
TYPE BernsteinCoef(TYPE t, unsigned int n, unsigned int i);*/

float BernsteinCoef(float t, unsigned int n, unsigned int i);
double BernsteinCoef(double t, unsigned int n, unsigned int i);

/**
 * Assumes little endian //TODO not sure
 * size in bytes
 */
std::string GetBinaryRepresentation(size_t const size, void const * const ptr);

/**
 * size in bytes
 */
std::string GetHexRepresentation(size_t const size, void const * const ptr);

#endif // SUPPORT_H
