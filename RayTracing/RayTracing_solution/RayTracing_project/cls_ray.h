#pragma once

#include "cls_point.h"
#include "cls_direction.h"

class cls_ray
{
public:

    cls_ray()
    {
    }

    cls_ray(float p_ox, float p_oy, float p_oz,
        float p_dx, float p_dy, float p_dz)
    {
        m_origin.SetComponents(p_ox, p_oy, p_oz);
        m_direction.SetComponents(p_dx, p_dy, p_dz);
    }

    ~cls_ray()
    {
    }

private:

    cls_point m_origin;

    cls_direction m_direction;

};
