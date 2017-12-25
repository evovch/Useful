#pragma once

struct stc_VandC {
    float v[3];
    float c[3];
};

enum action_type {
    ACT_NO_ACT,
    ACT_PAN,
    ACT_ZOOM,
    ACT_ROTATE,
    ACT_TILT,
    ACT_SECTIONMOVE,
    ACT_SECTIONROTATE,
    ACT_SECTIONTILT
};

void ValueToColor(float p_inVal, float p_inValMin, float p_inValMax, stc_VandC* o_destination);

void IntToColor(unsigned int p_inVal, stc_VandC* o_destination);

unsigned int ColorToInt(stc_VandC* p_source);

unsigned int PixelColorToInt(unsigned char* p_source);
