#pragma once

struct stc_VandC {
    float v[3];
    float c[3];
};

void ValueToColor(float p_inVal, float p_inValMin, float p_inValMax, stc_VandC* o_destination);

void IntToColor(unsigned int p_inVal, stc_VandC* o_destination);

unsigned int ColorToInt(stc_VandC* p_source);

unsigned int PixelColorToInt(unsigned char* p_source);
