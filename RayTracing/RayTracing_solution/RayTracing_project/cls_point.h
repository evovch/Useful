#pragma once

class cls_point
{
public:

    cls_point() :
        m_x(0.0f), m_y(0.0f), m_z(0.0f)
    {
    }

    cls_point(float p_x, float p_y, float p_z) :
        m_x(p_x), m_y(p_y), m_z(p_z)
    {
    }

    ~cls_point()
    {
    }

    void SetComponents(float p_x, float p_y, float p_z)
    {
        m_x = p_x;
        m_y = p_y;
        m_z = p_z;
    }

private:

    float m_x;

    float m_y;

    float m_z;

};
