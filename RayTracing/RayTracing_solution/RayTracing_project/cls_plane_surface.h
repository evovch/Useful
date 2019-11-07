#pragma once

#include "cls_surface.h" // mother class

#include "cls_point.h"
#include "cls_direction.h"

class cls_plane_surface : public cls_surface
{
public:

    cls_plane_surface()
    {
    }

    cls_plane_surface(float p_px, float p_py, float p_pz,
                      float p_dx, float p_dy, float p_dz)
    {
        m_point.SetComponents(p_px, p_py, p_pz);
        m_normal.SetComponents(p_dx, p_dy, p_dz);
    }

    virtual ~cls_plane_surface() override final
    {
    }

private:

    cls_point m_point;

    cls_direction m_normal;

};
