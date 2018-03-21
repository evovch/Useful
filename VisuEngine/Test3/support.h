#ifndef SUPPORT_H
#define SUPPORT_H

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

void ValueToColor(float p_inVal, float p_inValMin, float p_inValMax, stc_VandC* o_destination);

void IntToColor(unsigned int p_inVal, stc_VandC* o_destination);

unsigned int ColorToInt(stc_VandC* p_source);

unsigned int PixelColorToInt(GLubyte* p_source);

#endif // SUPPORT_H
