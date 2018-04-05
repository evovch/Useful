#ifndef SUPPORT_H
#define SUPPORT_H

// OpenGL
#include <GL/glew.h>

struct stc_VandC {
	float v[3];
	float c[3];
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

float UnitToScaleFactor(const char* p_unit);

void ValueToColor(float p_inVal, float p_inValMin, float p_inValMax, stc_VandC* o_destination);

void IntToColor(unsigned int p_inVal, stc_VandC* o_destination);

unsigned int ColorToInt(stc_VandC* p_source);

unsigned int PixelColorToInt(GLubyte* p_source);

unsigned int FourCharsToUintLE(const unsigned char* p_mem);

//unsigned int FourCharsToUintBE(const unsigned char* p_mem); //TODO test

float FourCharsToFloat(const unsigned char* p_mem);

#endif // SUPPORT_H
